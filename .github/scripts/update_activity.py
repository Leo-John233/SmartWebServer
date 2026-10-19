from __future__ import annotations

import fnmatch
import hashlib
import html
import json
import os
import re
import subprocess
from collections import Counter
from datetime import date, datetime, timedelta
from functools import lru_cache
from pathlib import Path
from zoneinfo import ZoneInfo

# ============================================================
# 配置
# ============================================================

ROOT = Path(__file__).resolve().parents[2] if ".github" in str(Path(__file__).resolve()) else Path.cwd()
OUTPUT_DIR = ROOT / "site"

BASE_BRANCH = os.getenv(
    "ACTIVITY_BASE_BRANCH",
    "SmartWebServer-FeatureRelease",
)

BRANCH_PATTERN = os.getenv(
    "ACTIVITY_BRANCH_PATTERN",
    "*",
)
TIMEZONE_NAME = os.getenv("ACTIVITY_TIMEZONE", "Asia/Shanghai")
TZ = ZoneInfo(TIMEZONE_NAME)

RECENT_DAYS = 7
RECENT_HISTORY_DAYS = 370
MAX_RECENT_ROWS = 12
TOP_CONTRIBUTORS = 6

IGNORED_FILES = {
    "README.md",
    "assets/repository-activity.svg",
    "assets/repository-statistics.svg",
    "assets/recent-changes.svg",
    "assets/branch-statistics.svg",
    "docs/activity.html",
    "docs/activity-data.json",
}

IGNORED_PREFIXES = (
    ".github/",
    "site/",
)

AUTO_COMMIT_MESSAGES = {
    "docs: auto-update repository activity",
}

BOT_AUTHORS = {
    "github-actions[bot]",
    "dependabot[bot]",
}

AUTHOR_ALIASES = {
    "Leo-John": "Leo-John233",
    "Leo·John": "Leo-John233",
    "Leo-John233": "Leo-John233",
}

# ============================================================
# Git 辅助
# ============================================================


def git(*args: str, check: bool = True) -> str:
    result = subprocess.run(
        ["git", "-c", "core.quotepath=false", *args],
        cwd=ROOT,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        encoding="utf-8",
    )
    if check and result.returncode != 0:
        raise RuntimeError(
            "Git command failed:\n"
            + "git "
            + " ".join(args)
            + "\n"
            + result.stderr.strip()
        )
    return result.stdout


def git_ok(*args: str) -> bool:
    result = subprocess.run(
        ["git", *args],
        cwd=ROOT,
        check=False,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return result.returncode == 0


def branch_ref(branch: str) -> str:
    remote_ref = f"refs/remotes/origin/{branch}"
    if git_ok("show-ref", "--verify", "--quiet", remote_ref):
        return f"origin/{branch}"
    return branch


def discover_branches() -> list[str]:
    raw = git(
        "for-each-ref",
        "--format=%(refname:short)",
        "refs/remotes/origin/",
    )

    branches: list[str] = []
    for line in raw.splitlines():
        value = line.strip()
        if not value or value in {"origin/HEAD", "origin"}:
            continue
        if not value.startswith("origin/"):
            continue
        branch = value[len("origin/") :]
        if branch == "HEAD":
            continue
        if fnmatch.fnmatch(branch, BRANCH_PATTERN):
            branches.append(branch)

    # 本地测试/手动运行时，如果 origin 远程引用不存在，也允许读取本地分支。
    if not branches:
        local_raw = git("for-each-ref", "--format=%(refname:short)", "refs/heads/")
        for line in local_raw.splitlines():
            branch = line.strip()
            if branch and fnmatch.fnmatch(branch, BRANCH_PATTERN):
                branches.append(branch)

    branches = sorted(set(branches))
    if BASE_BRANCH in branches:
        branches.remove(BASE_BRANCH)
        branches.insert(0, BASE_BRANCH)

    if not branches:
        raise RuntimeError(
            f"没有找到匹配 {BRANCH_PATTERN} 的分支\n"
            "请确认 Workflow 已获取全部远程分支"
        )

    return branches


# ============================================================
# 文本与 Commit 辅助
# ============================================================


def canonical_author(author: str) -> str:
    author = author.strip()
    return AUTHOR_ALIASES.get(author, author)


def should_ignore_file(path: str) -> bool:
    normalized = path.replace("\\", "/")
    return normalized in IGNORED_FILES or any(
        normalized.startswith(prefix) for prefix in IGNORED_PREFIXES
    )


def is_bot_commit(commit: dict) -> bool:
    author = canonical_author(commit.get("author", ""))
    subject = commit.get("subject", "")
    email_addr = commit.get("email", "").lower()
    return (
        author in BOT_AUTHORS
        or subject in AUTO_COMMIT_MESSAGES
        or "[bot]" in author.lower()
        or "noreply@github.com" in email_addr and "actions" in author.lower()
    )


def parse_log(raw: str) -> list[dict]:
    commits: list[dict] = []
    for record in raw.split("\x1e"):
        record = record.strip()
        if not record:
            continue
        parts = record.split("\x1f", 4)
        if len(parts) != 5:
            continue
        sha, author, email_addr, iso_time, subject = parts
        try:
            commit_time = datetime.fromisoformat(iso_time.strip()).astimezone(TZ)
        except ValueError:
            continue
        commits.append(
            {
                "sha": sha.strip(),
                "short_sha": sha.strip()[:7],
                "author": canonical_author(author),
                "email": email_addr.strip(),
                "subject": subject.strip(),
                "date": commit_time.date(),
                "datetime": commit_time,
            }
        )
    return commits


def load_log(ref: str, since_days: int | None = None) -> list[dict]:
    args = ["log", ref]
    if since_days is not None:
        args.append(f"--since={since_days} days ago")
    args.append(
        "--pretty=format:%H%x1f%aN%x1f%aE%x1f%aI%x1f%s%x1e"
    )
    return parse_log(git(*args))


@lru_cache(maxsize=None)
def get_commit_files(sha: str) -> tuple[str, ...]:
    raw = git(
        "diff-tree",
        "--root",
        "--no-commit-id",
        "--name-only",
        "-r",
        "-z",
        sha,
    )
    result: list[str] = []
    for path in raw.split("\0"):
        path = path.strip()
        if path and not should_ignore_file(path):
            result.append(path)
    return tuple(result)


def get_scope(path: str) -> str:
    parts = path.replace("\\", "/").split("/")
    if not parts:
        return "-"
    if parts[0] == "原始固件" and len(parts) >= 2:
        return parts[1]
    return parts[0]


def summarize_scope(files: list[str] | tuple[str, ...]) -> str:
    if not files:
        return "-"
    if len(files) == 1:
        path = files[0].replace("\\", "/")
        if path.startswith("原始固件/"):
            path = path[len("原始固件/") :]
        return path

    scopes: list[str] = []
    for path in files:
        scope = get_scope(path)
        if scope not in scopes:
            scopes.append(scope)

    if len(scopes) == 1:
        return scopes[0]
    if len(scopes) == 2:
        return f"{scopes[0]} 和 {scopes[1]}"
    return "多个目录"


def load_recent_history(ref: str) -> list[dict]:
    result: list[dict] = []
    for commit in load_log(ref, RECENT_HISTORY_DAYS):
        if is_bot_commit(commit):
            continue
        files = get_commit_files(commit["sha"])
        if not files:
            continue
        result.append(
            {
                **commit,
                "files": list(files),
                "scope": summarize_scope(files),
                "file_count": len(files),
            }
        )
    return result


def load_full_history(ref: str) -> list[dict]:
    return [commit for commit in load_log(ref) if not is_bot_commit(commit)]


def merge_histories(histories: list[list[dict]]) -> list[dict]:
    """合并各分支历史，同一完整提交 SHA 只保留一次。"""
    unique: dict[str, dict] = {}
    for history in histories:
        for commit in history:
            unique.setdefault(commit["sha"], commit)
    return sorted(
        unique.values(),
        key=lambda commit: (commit["datetime"], commit["sha"]),
        reverse=True,
    )


def recent_commits(history: list[dict]) -> list[dict]:
    today = datetime.now(TZ).date()
    first = today - timedelta(days=RECENT_DAYS - 1)
    return [c for c in history if first <= c["date"] <= today]


def repository_statistics(full_history: list[dict]) -> dict:
    contributors: Counter = Counter(canonical_author(c["author"]) for c in full_history)
    today = datetime.now(TZ).date()
    if full_history:
        first_date = min(c["date"] for c in full_history)
        last_date = max(c["date"] for c in full_history)
    else:
        first_date = today
        last_date = today
    return {
        "total_commits": len(full_history),
        "contributors": len(contributors),
        "first_date": first_date,
        "last_date": last_date,
        "top": contributors.most_common(TOP_CONTRIBUTORS),
    }


def branch_role(branch: str) -> str:
    if branch == BASE_BRANCH:
        return "正式版"

    name = branch.lower()

    if name.endswith("dev"):
        return "开发版"

    if name.endswith("original"):
        return "原始基线"

    if "test" in name:
        return "测试版"

    return "分支"


def ahead_behind(base_branch: str, branch: str) -> tuple[int | None, int | None, bool]:
    if branch == base_branch:
        return 0, 0, True

    base_ref = branch_ref(base_branch)
    current_ref = branch_ref(branch)

    # 没有共同祖先时，不把数千条独立历史伪装成 ahead / behind。
    if not git_ok("merge-base", base_ref, current_ref):
        return None, None, False

    raw = git("rev-list", "--left-right", "--count", f"{base_ref}...{current_ref}")
    parts = raw.strip().split()
    if len(parts) != 2:
        return None, None, False

    behind = int(parts[0])
    ahead = int(parts[1])
    return ahead, behind, True


def latest_commit(ref: str) -> dict | None:
    history = load_log(ref)
    return history[0] if history else None


def safe_slug(branch: str) -> str:
    slug = re.sub(r"[^A-Za-z0-9._-]+", "-", branch).strip("-")
    digest = hashlib.sha256(branch.encode("utf-8")).hexdigest()[:12]
    # 区分 feature/a 和 feature-a 等净化后同名的分支。
    return f"{slug[:96] or 'branch'}-{digest}"


def repository_name() -> str:
    return os.getenv("GITHUB_REPOSITORY", "Leo-John233/SmartWebServer")


def repository_url() -> str:
    return f"https://github.com/{repository_name()}"


def branch_url(branch: str) -> str:
    return f"{repository_url()}/tree/{branch}"


def commit_url(sha: str) -> str:
    return f"{repository_url()}/commit/{sha}"


# ============================================================
# SVG 公共辅助
# ============================================================


def svg_text(value: object) -> str:
    return html.escape(str(value), quote=True)


def truncate(text: str, length: int) -> str:
    text = str(text)
    if len(text) <= length:
        return text
    return text[: max(0, length - 1)] + "…"


COMMON_STYLE = """
.text{font-family:-apple-system,BlinkMacSystemFont,"Segoe UI","Noto Sans CJK SC","Microsoft YaHei","PingFang SC",Helvetica,Arial,sans-serif}
.card{fill:#fff;stroke:#d0d7de;stroke-width:1}
.title{fill:#24292f;font-size:23px;font-weight:600}
.header{fill:#57606a;font-size:14px;font-weight:600}
.normal{fill:#24292f;font-size:15px;font-weight:400}
.bold{fill:#24292f;font-size:15px;font-weight:600}
.secondary{fill:#57606a;font-size:14px;font-weight:400}
.link{fill:#0969da;font-size:15px;font-weight:600}
.divider{stroke:#d8dee4;stroke-width:1}
.badge{fill:#ddf4ff;stroke:#54aeff;stroke-width:1}
.badge-text{fill:#0969da;font-size:13px;font-weight:600}
.good{fill:#1a7f37;font-size:14px;font-weight:600}
.warn{fill:#9a6700;font-size:14px;font-weight:600}
@media(prefers-color-scheme:dark){
.card{fill:#0d1117;stroke:#30363d}.title,.normal,.bold{fill:#c9d1d9}
.header,.secondary{fill:#8b949e}.link{fill:#58a6ff}.divider{stroke:#21262d}
.badge{fill:#121d2f;stroke:#1f6feb}.badge-text{fill:#79c0ff}.good{fill:#3fb950}.warn{fill:#d29922}
}
"""


def section_header(title: str, subtitle: str, badge: str, width: int) -> list[str]:
    badge_width = max(100, 32 + sum(13 if ord(char) > 127 else 7 for char in badge))
    badge_x = width - badge_width - 1
    return [
        f'<text class="text title" x="0" y="29">{svg_text(title)}</text>',
        f'<text class="text secondary" x="0" y="57">{svg_text(subtitle)}</text>',
        f'<rect class="badge" x="{badge_x}" y="4" width="{badge_width}" height="32" rx="6" ry="6"/>',
        f'<text class="text badge-text" x="{badge_x + badge_width / 2}" y="25" text-anchor="middle">{svg_text(badge)}</text>',
    ]

def write_svg(path: Path, lines: list[str]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines), encoding="utf-8")


# ============================================================
# 最近代码修改 SVG
# ============================================================


def generate_recent_svg(history: list[dict], output: Path, branch: str) -> None:
    commits = recent_commits(history)
    visible = commits[:MAX_RECENT_ROWS]
    hidden_count = max(0, len(commits) - len(visible))

    width = 1120
    card_y = 80
    header_h = 46
    row_h = 46
    footer_h = 46
    rows = max(1, len(visible))
    card_h = header_h + rows * row_h + footer_h
    height = card_y + card_h + 1

    date_x, author_x, subject_x, sha_x, scope_x = 30, 122, 300, 800, 900

    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img">',
        "<style>", COMMON_STYLE, "</style>",
        *section_header(
            "最近代码修改",
            f"统计范围 {branch}",
            f"近 {RECENT_DAYS} 天 {len(commits):,} 次提交",
            width,
        ),
        f'<rect class="card" x="0.5" y="{card_y + 0.5}" width="1118" height="{card_h - 1}" rx="6" ry="6"/>',
        f'<text class="text header" x="{date_x}" y="{card_y + 29}">日期</text>',
        f'<text class="text header" x="{author_x}" y="{card_y + 29}">修改者</text>',
        f'<text class="text header" x="{subject_x}" y="{card_y + 29}">修改内容</text>',
        f'<text class="text header" x="{scope_x}" y="{card_y + 29}">范围</text>',
        f'<line class="divider" x1="0" y1="{card_y + header_h}" x2="1119" y2="{card_y + header_h}"/>',
    ]

    rows_top = card_y + header_h
    if visible:
        for i, commit in enumerate(visible):
            row_top = rows_top + i * row_h
            baseline = row_top + 30
            d = commit["date"]
            date_text = f"{d.month}/{d.day}"
            author = truncate(commit["author"], 19)
            subject = truncate(commit["subject"], 47)
            scope = truncate(commit["scope"], 20)
            if commit["file_count"] > 1:
                scope += f" {commit['file_count']} 个文件"
            url = commit_url(commit["sha"])
            svg.extend(
                [
                    f'<text class="text normal" x="{date_x}" y="{baseline}">{svg_text(date_text)}</text>',
                    f'<text class="text bold" x="{author_x}" y="{baseline}">{svg_text(author)}</text>',
                    f'<a href="{svg_text(url)}"><text class="text link" x="{subject_x}" y="{baseline}">{svg_text(subject)}</text></a>',
                    f'<text class="text link" x="{sha_x}" y="{baseline}">{svg_text(commit["short_sha"])}</text>',
                    f'<text class="text secondary" x="{scope_x}" y="{baseline}">{svg_text(scope)}</text>',
                ]
            )
            if i < len(visible) - 1:
                y = row_top + row_h
                svg.append(f'<line class="divider" x1="28" y1="{y}" x2="1090" y2="{y}"/>')
    else:
        svg.append(
            f'<text class="text secondary" x="30" y="{rows_top + 30}">最近 {RECENT_DAYS} 天没有有效代码提交</text>'
        )

    footer_y = rows_top + rows * row_h
    svg.append(f'<line class="divider" x1="0" y1="{footer_y}" x2="1119" y2="{footer_y}"/>')
    footer_text = "仅统计实际代码或项目文件修改"
    if hidden_count:
        footer_text += f" 另有 {hidden_count} 条未显示"
    svg.append(
        f'<text class="text secondary" x="30" y="{footer_y + 29}">{svg_text(footer_text)}</text>'
    )
    svg.append("</svg>")
    write_svg(output, svg)


# ============================================================
# Contribution Graph SVG
# ============================================================


ACTIVITY_STYLE = COMMON_STYLE + """
.month,.weekday{fill:#24292f;font-size:14px;font-weight:400}
.footer{fill:#57606a;font-size:14px;font-weight:400}
.level-0{fill:#ebedf0;stroke:rgba(27,31,35,.06);stroke-width:1}
.level-1{fill:#9be9a8}.level-2{fill:#40c463}.level-3{fill:#30a14e}.level-4{fill:#216e39}
@media(prefers-color-scheme:dark){
.month,.weekday{fill:#c9d1d9}.footer{fill:#8b949e}.level-0{fill:#161b22;stroke:#1b1f23}
.level-1{fill:#0e4429}.level-2{fill:#006d32}.level-3{fill:#26a641}.level-4{fill:#39d353}
}
"""


def contribution_counts(history: list[dict]) -> Counter:
    today = datetime.now(TZ).date()
    first = today - timedelta(days=364)
    counts: Counter = Counter()
    for commit in history:
        if first <= commit["date"] <= today:
            counts[commit["date"].isoformat()] += 1
    return counts


def contribution_level(count: int) -> int:
    if count <= 0:
        return 0
    if count == 1:
        return 1
    if count <= 3:
        return 2
    if count <= 6:
        return 3
    return 4


def generate_activity_svg(
    history: list[dict], output: Path, branch: str, *, all_branches: bool = False
) -> None:
    counts = contribution_counts(history)
    total = sum(counts.values())
    today = datetime.now(TZ).date()
    first_day = today - timedelta(days=364)

    width = 1120
    card_y, card_w, card_h = 80, 1119, 233
    height = card_y + card_h + 1
    cell, gap = 14, 5
    step = cell + gap
    grid_left, grid_top = 80, card_y + 50
    month_y, footer_y, legend_x = card_y + 40, card_y + 209, 872

    offset_to_sunday = (first_day.weekday() + 1) % 7
    grid_start = first_day - timedelta(days=offset_to_sunday)
    grid_days = (today - grid_start).days + 1

    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img">',
        "<style>", ACTIVITY_STYLE, "</style>",
        *section_header(
            "近一年代码活动",
            f"统计范围 {branch}" + (" 按提交 SHA 去重" if all_branches else ""),
            f"{total:,} 次提交",
            width,
        ),
        f'<rect class="card" x="0.5" y="{card_y + 0.5}" width="{card_w - 1}" height="{card_h - 1}" rx="6" ry="6"/>',
    ]

    prev_month = None
    prev_x = -100
    for week in range(53):
        week_date = grid_start + timedelta(days=week * 7 + 3)
        if week_date.month == prev_month:
            continue
        x = grid_left + week * step
        if x - prev_x < 45:
            continue
        prev_month = week_date.month
        prev_x = x
        svg.append(
            f'<text class="text month" x="{x}" y="{month_y}">{week_date.strftime("%b")}</text>'
        )

    for row, label in {1: "Mon", 3: "Wed", 5: "Fri"}.items():
        y = grid_top + row * step + 12
        svg.append(f'<text class="text weekday" x="35" y="{y}">{label}</text>')

    for offset in range(grid_days):
        current = grid_start + timedelta(days=offset)
        if current < first_day or current > today:
            continue
        week = offset // 7
        if week >= 53:
            continue
        row = (current.weekday() + 1) % 7
        x = grid_left + week * step
        y = grid_top + row * step
        count = counts.get(current.isoformat(), 0)
        level = contribution_level(count)
        tooltip = f"{count} contribution{'s' if count != 1 else ''} on {current.isoformat()}"
        svg.append(
            f'<rect class="level-{level}" x="{x}" y="{y}" width="{cell}" height="{cell}" rx="2" ry="2"><title>{svg_text(tooltip)}</title></rect>'
        )

    activity_note = (
        "Valid code changes deduplicated across all branches"
        if all_branches else "Valid code changes only"
    )
    svg.append(
        f'<text class="text footer" x="60" y="{footer_y}">{svg_text(activity_note)}</text>'
    )
    svg.append(f'<text class="text footer" x="{legend_x}" y="{footer_y}">Less</text>')
    for index in range(5):
        x = legend_x + 40 + index * step
        svg.append(
            f'<rect class="level-{index}" x="{x}" y="{footer_y - 13}" width="{cell}" height="{cell}" rx="2" ry="2"/>'
        )
    svg.append(f'<text class="text footer" x="{legend_x + 140}" y="{footer_y}">More</text>')
    svg.append("</svg>")
    write_svg(output, svg)


# ============================================================
# Repository Statistics SVG
# ============================================================


def generate_statistics_svg(
    full_history: list[dict], output: Path, branch: str, *, all_branches: bool = False
) -> None:
    stats = repository_statistics(full_history)
    top = stats["top"]

    width = 1120
    card_y = 80
    metric_h = 92
    contributors_title_h = 48
    row_h = 42
    footer_h = 42
    row_count = max(1, len(top))
    card_h = metric_h + contributors_title_h + row_count * row_h + footer_h
    height = card_y + card_h + 1
    divider_y = card_y + metric_h
    contributors_title_y = divider_y + 30
    rows_top = divider_y + contributors_title_h
    footer_y = rows_top + row_count * row_h

    metrics = [
        (f"{stats['total_commits']:,}", "Commits"),
        (f"{stats['contributors']:,}", "Contributors"),
        (f"Since {stats['first_date'].year}", "Repository history"),
    ]

    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img">',
        "<style>", COMMON_STYLE, "</style>",
        *section_header(
            "仓库统计",
            f"统计范围 {branch}" + (" 按提交 SHA 去重" if all_branches else ""),
            "完整提交历史",
            width,
        ),
        f'<rect class="card" x="0.5" y="{card_y + 0.5}" width="1118" height="{card_h - 1}" rx="6" ry="6"/>',
    ]

    for x, (value, label) in zip([186, 560, 934], metrics):
        svg.extend(
            [
                f'<text class="text bold" x="{x}" y="{card_y + 38}" text-anchor="middle" style="font-size:22px">{svg_text(value)}</text>',
                f'<text class="text secondary" x="{x}" y="{card_y + 64}" text-anchor="middle">{svg_text(label)}</text>',
            ]
        )
    for x in (373, 747):
        svg.append(
            f'<line class="divider" x1="{x}" y1="{card_y + 18}" x2="{x}" y2="{card_y + 74}"/>'
        )

    svg.append(f'<line class="divider" x1="0" y1="{divider_y}" x2="1119" y2="{divider_y}"/>')
    svg.append(
        f'<text class="text bold" x="28" y="{contributors_title_y}">Top contributors</text>'
    )

    if top:
        for i, (author, count) in enumerate(top):
            row_top = rows_top + i * row_h
            baseline = row_top + 27
            commit_text = "1 commit" if count == 1 else f"{count:,} commits"
            svg.append(
                f'<text class="text link" x="28" y="{baseline}">{svg_text(author)}</text>'
            )
            svg.append(
                f'<text class="text secondary" x="1090" y="{baseline}" text-anchor="end">{svg_text(commit_text)}</text>'
            )
            if i < len(top) - 1:
                y = row_top + row_h
                svg.append(f'<line class="divider" x1="28" y1="{y}" x2="1090" y2="{y}"/>')
    else:
        svg.append(
            f'<text class="text secondary" x="28" y="{rows_top + 27}">No contributor data</text>'
        )

    svg.append(f'<line class="divider" x1="0" y1="{footer_y}" x2="1119" y2="{footer_y}"/>')
    date_range = f"{stats['first_date'].isoformat()} 至 {stats['last_date'].isoformat()}"
    history_note = (
        "All branch commits deduplicated by SHA"
        if all_branches else "Based on the complete Git history of this branch"
    )
    svg.append(
        f'<text class="text secondary" x="28" y="{footer_y + 27}">{svg_text(history_note)}</text>'
    )
    svg.append(
        f'<text class="text secondary" x="1090" y="{footer_y + 27}" text-anchor="end">{svg_text(date_range)}</text>'
    )
    svg.append("</svg>")
    write_svg(output, svg)


# ============================================================
# 分支统计
# ============================================================


def collect_branch_info(
    branches: list[str],
    recent_histories: dict[str, list[dict]],
    full_histories: dict[str, list[dict]],
) -> list[dict]:
    result: list[dict] = []
    for branch in branches:
        ref = branch_ref(branch)
        latest = full_histories[branch][0] if full_histories[branch] else None
        ahead, behind, comparable = ahead_behind(BASE_BRANCH, branch)
        recent7 = recent_commits(recent_histories[branch])
        full_stats = repository_statistics(full_histories[branch])

        result.append(
            {
                "branch": branch,
                "slug": safe_slug(branch),
                "role": branch_role(branch),
                "ahead": ahead,
                "behind": behind,
                "comparable": comparable,
                "recent7": len(recent7),
                "total_commits": full_stats["total_commits"],
                "latest_date": latest["date"].isoformat() if latest else "-",
                "latest_subject": latest["subject"] if latest else "-",
                "latest_sha": latest["short_sha"] if latest else "-",
                "url": branch_url(branch),
            }
        )
    return result


def generate_branch_statistics_svg(branches: list[dict], output: Path) -> None:
    width = 1120
    card_y = 80
    header_h = 46
    row_h = 48
    footer_h = 44
    rows = max(1, len(branches))
    card_h = header_h + rows * row_h + footer_h
    height = card_y + card_h + 1

    x_branch = 28
    x_role = 305
    x_ahead = 395
    x_behind = 485
    x_recent = 585
    x_date = 700
    x_subject = 810

    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img">',
        "<style>", COMMON_STYLE, "</style>",
        *section_header(
            "分支对比",
            f"对比基准 {BASE_BRANCH}",
            f"{len(branches)} 个分支",
            width,
        ),
        f'<rect class="card" x="0.5" y="{card_y + 0.5}" width="1118" height="{card_h - 1}" rx="6" ry="6"/>',
        f'<text class="text header" x="{x_branch}" y="{card_y + 29}">分支</text>',
        f'<text class="text header" x="{x_role}" y="{card_y + 29}">类型</text>',
        f'<text class="text header" x="{x_ahead}" y="{card_y + 29}">领先提交</text>',
        f'<text class="text header" x="{x_behind}" y="{card_y + 29}">落后提交</text>',
        f'<text class="text header" x="{x_recent}" y="{card_y + 29}">近 {RECENT_DAYS} 天</text>',
        f'<text class="text header" x="{x_date}" y="{card_y + 29}">最近更新</text>',
        f'<text class="text header" x="{x_subject}" y="{card_y + 29}">最近提交</text>',
        f'<line class="divider" x1="0" y1="{card_y + header_h}" x2="1119" y2="{card_y + header_h}"/>',
    ]

    rows_top = card_y + header_h
    if branches:
        for i, info in enumerate(branches):
            row_top = rows_top + i * row_h
            baseline = row_top + 31
            ahead_text = str(info["ahead"]) if info["comparable"] else "不适用"
            behind_text = str(info["behind"]) if info["comparable"] else "不适用"
            subject = truncate(info["latest_subject"], 30)
            svg.extend(
                [
                    f'<a href="{svg_text(info["url"])}"><text class="text link" x="{x_branch}" y="{baseline}">{svg_text(truncate(info["branch"], 33))}</text></a>',
                    f'<text class="text normal" x="{x_role}" y="{baseline}">{svg_text(info["role"])}</text>',
                    f'<text class="text good" x="{x_ahead}" y="{baseline}">{svg_text(ahead_text)}</text>',
                    f'<text class="text warn" x="{x_behind}" y="{baseline}">{svg_text(behind_text)}</text>',
                    f'<text class="text normal" x="{x_recent}" y="{baseline}">{svg_text(info["recent7"])}</text>',
                    f'<text class="text secondary" x="{x_date}" y="{baseline}">{svg_text(info["latest_date"])}</text>',
                    f'<text class="text normal" x="{x_subject}" y="{baseline}">{svg_text(subject)}</text>',
                ]
            )
            if i < len(branches) - 1:
                y = row_top + row_h
                svg.append(f'<line class="divider" x1="28" y1="{y}" x2="1090" y2="{y}"/>')

    footer_y = rows_top + rows * row_h
    svg.append(f'<line class="divider" x1="0" y1="{footer_y}" x2="1119" y2="{footer_y}"/>')
    svg.append(
        f'<text class="text secondary" x="28" y="{footer_y + 28}">领先和落后按提交数计算 无共同祖先时显示不适用</text>'
    )
    svg.append("</svg>")
    write_svg(output, svg)


# ============================================================
# GitHub Pages
# ============================================================


def generate_index_html(branches: list[dict]) -> None:
    asset_version = datetime.now(TZ).strftime("%Y%m%d%H%M%S%f")
    page_data = [
        {
            "branch": item["branch"],
            "slug": item["slug"],
            "role": item["role"],
            "ahead": item["ahead"],
            "behind": item["behind"],
            "comparable": item["comparable"],
        }
        for item in branches
    ]

    options = '<option value="">所有分支</option>\n' + "\n".join(
        f'<option value="{html.escape(item["branch"], quote=True)}">'
        f'{html.escape(item["branch"])} {html.escape(item["role"])}</option>'
        for item in branches
    )

    page_data_json = json.dumps(page_data, ensure_ascii=False).replace("<", "\\u003c")
    base_branch_json = json.dumps(BASE_BRANCH, ensure_ascii=False).replace("<", "\\u003c")

    page = f"""<!doctype html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>SmartWebServer Repository Activity</title>
<style>
:root{{color-scheme:light dark}}
body{{margin:0;padding:32px;background:#fff;color:#24292f;font-family:-apple-system,BlinkMacSystemFont,"Segoe UI","Noto Sans CJK SC","Microsoft YaHei","PingFang SC",Helvetica,Arial,sans-serif}}
main{{width:min(1120px,100%);margin:0 auto}}
h1{{font-size:28px;margin:0 0 18px}}
.toolbar{{display:flex;gap:12px;align-items:center;flex-wrap:wrap;margin:0 0 26px}}
.toolbar label{{font-weight:600}}
select{{font:inherit;padding:8px 34px 8px 10px;border:1px solid #d0d7de;border-radius:6px;background:#fff;color:#24292f}}
.meta{{color:#57606a;font-size:14px;white-space:pre-line;line-height:1.6;flex-basis:100%}}
img{{display:block;width:100%;height:auto;margin:0 0 30px}}
a{{color:#0969da;text-decoration:none}}a:hover{{text-decoration:underline}}
.footer{{margin:24px 0 32px;color:#57606a;font-size:15px}}
@media(prefers-color-scheme:dark){{body{{background:#0d1117;color:#c9d1d9}}select{{background:#161b22;color:#c9d1d9;border-color:#30363d}}.meta,.footer{{color:#8b949e}}a{{color:#58a6ff}}}}
</style>
</head>
<body>
<main>
<h1>SmartWebServer Repository Activity</h1>
<div class="toolbar">
<label for="branch">统计范围</label>
<select id="branch">{options}</select>
<span id="meta" class="meta"></span>
</div>

<img id="recent" src="recent-changes.svg?v={asset_version}" alt="Recent Changes">
<img id="activity" src="repository-activity.svg?v={asset_version}" alt="Repository Activity">
<img id="statistics" src="repository-statistics.svg?v={asset_version}" alt="Repository Statistics">
<img src="branch-statistics.svg?v={asset_version}" alt="Branch Statistics">

<div class="footer">
<a href="{html.escape(repository_url(), quote=True)}">返回 GitHub Repository</a>
</div>
</main>
<script>
const branches = {page_data_json};
const baseBranch = {base_branch_json};
const assetVersion = "{asset_version}";
const select = document.getElementById('branch');
const meta = document.getElementById('meta');
const recent = document.getElementById('recent');
const activity = document.getElementById('activity');
const statistics = document.getElementById('statistics');

function assetUrl(path) {{
  return path + '?v=' + encodeURIComponent(assetVersion);
}}

function applyBranch(name) {{
  const item = branches.find(x => x.branch === name);
  if (!item) {{
    select.value = '';
    recent.src = assetUrl('recent-changes.svg');
    activity.src = assetUrl('repository-activity.svg');
    statistics.src = assetUrl('repository-statistics.svg');
    meta.textContent = `汇总 ${{branches.length}} 个分支并按提交 SHA 去重
最近 {RECENT_DAYS} 天代码修改显示 ${{baseBranch}}`;
    history.replaceState(null, '', '#scope=all');
    return;
  }}
  select.value = item.branch;
  const prefix = `branches/${{item.slug}}/`;
  recent.src = assetUrl(prefix + 'recent-changes.svg');
  activity.src = assetUrl(prefix + 'repository-activity.svg');
  statistics.src = assetUrl(prefix + 'repository-statistics.svg');
  if (item.comparable) {{
    meta.textContent = `与 ${{baseBranch}} 对比
领先 ${{item.ahead}} 次提交
落后 ${{item.behind}} 次提交`;
  }} else {{
    meta.textContent = `与 ${{baseBranch}} 无共同祖先
无法比较领先和落后提交数`;
  }}
  history.replaceState(null, '', '#branch=' + encodeURIComponent(item.branch));
}}

function applyHash() {{
  const params = new URLSearchParams(location.hash.slice(1));
  const branch = params.get('scope') === 'all' ? '' : params.get('branch') || '';
  applyBranch(branch);
}}

select.addEventListener('change', () => applyBranch(select.value));
window.addEventListener('hashchange', applyHash);
applyHash();
</script>
</body>
</html>
"""

    (OUTPUT_DIR / "index.html").write_text(page, encoding="utf-8")
    (OUTPUT_DIR / ".nojekyll").write_text("", encoding="utf-8")
    (OUTPUT_DIR / "branches.json").write_text(
        json.dumps(branches, ensure_ascii=False, indent=2, default=str),
        encoding="utf-8",
    )


# ============================================================
# 主流程
# ============================================================


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    branches = discover_branches()
    print(f"自动发现 {len(branches)} 个分支")
    print("\n".join(branches))

    if BASE_BRANCH not in branches:
        raise RuntimeError(
            f"基准分支 {BASE_BRANCH} 不在自动发现结果中\n"
            f"当前匹配规则为 {BRANCH_PATTERN}"
        )
    if len({safe_slug(branch) for branch in branches}) != len(branches):
        raise RuntimeError("分支输出目录发生冲突\n请检查分支名称")

    recent_histories: dict[str, list[dict]] = {}
    full_histories: dict[str, list[dict]] = {}

    for branch in branches:
        ref = branch_ref(branch)
        print(f"读取 {branch} 最近一年有效代码历史")
        recent_history = load_recent_history(ref)
        print(f"  有效提交 {len(recent_history)}")

        print(f"读取 {branch} 完整历史")
        full_history = load_full_history(ref)
        print(f"  完整提交 {len(full_history)}")

        recent_histories[branch] = recent_history
        full_histories[branch] = full_history

        # 每个真实分支都有独立输出，包含基准分支。
        target_dir = OUTPUT_DIR / "branches" / safe_slug(branch)
        generate_recent_svg(
            recent_history, target_dir / "recent-changes.svg", branch
        )
        generate_activity_svg(
            recent_history, target_dir / "repository-activity.svg", branch
        )
        generate_statistics_svg(
            full_history, target_dir / "repository-statistics.svg", branch
        )

    # README 的最近修改保留发布分支；两项仓库统计汇总所有分支。
    all_recent_history = merge_histories(list(recent_histories.values()))
    all_full_history = merge_histories(list(full_histories.values()))
    aggregate_label = f"所有 {len(branches)} 个分支"

    generate_recent_svg(
        recent_histories[BASE_BRANCH], OUTPUT_DIR / "recent-changes.svg", BASE_BRANCH
    )
    generate_activity_svg(
        all_recent_history,
        OUTPUT_DIR / "repository-activity.svg",
        aggregate_label,
        all_branches=True,
    )
    generate_statistics_svg(
        all_full_history,
        OUTPUT_DIR / "repository-statistics.svg",
        aggregate_label,
        all_branches=True,
    )

    branch_info = collect_branch_info(branches, recent_histories, full_histories)
    generate_branch_statistics_svg(
        branch_info, OUTPUT_DIR / "branch-statistics.svg"
    )
    generate_index_html(branch_info)

    summary = repository_statistics(all_full_history)
    scope_data = {
        "scope": "all_branches",
        "branches": branches,
        "refs": {branch: git("rev-parse", branch_ref(branch)).strip() for branch in branches},
        "generated_at": datetime.now(TZ).isoformat(),
        "deduplicate_by": "full_commit_sha",
        "total_commits": summary["total_commits"],
        "contributors": summary["contributors"],
        "first_date": summary["first_date"].isoformat(),
        "last_date": summary["last_date"].isoformat(),
        "activity_last_year": sum(contribution_counts(all_recent_history).values()),
        "recent_changes_branch": BASE_BRANCH,
        "filters": {
            "activity": "代码或项目文件修改 排除机器人及仅修改忽略路径的提交",
            "statistics": "所有分支完整提交历史 排除机器人提交",
        },
    }
    (OUTPUT_DIR / "repository-summary.json").write_text(
        json.dumps(scope_data, ensure_ascii=False, indent=2), encoding="utf-8"
    )
    print("所有分支按提交 SHA 去重汇总")
    print(f"完整提交 {summary['total_commits']}")
    print(f"贡献者 {summary['contributors']}")
    print(f"最近一年有效提交 {scope_data['activity_last_year']}")
    print(f"生成完成 {OUTPUT_DIR}")


if __name__ == "__main__":
    main()



