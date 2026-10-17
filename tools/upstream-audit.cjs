// 对照已集成基线检查上游增量，只读取 Git 和工作区
const fs = require('node:fs');
const path = require('node:path');
const cp = require('node:child_process');

const root = path.resolve(__dirname, '..');
const baselineRef = process.argv[3] || '193a81809f5b19b3afa2a8c58f18291c2f0b0475';
const targetRef = process.argv[2] || 'origin/SmartWebServer-Original';
const hooks = new Set([
  'SmartWebServer.ino',
  'src/lib/wifi/webServer/WebServer.h',
  'src/lib/wifi/webServer/WebServer.cpp',
  'src/lib/wifi/cmdServer/CmdServer.h',
  'src/lib/wifi/cmdServer/CmdServer.cpp',
  'src/lib/ethernet/cmdServer/CmdServer.h',
  'src/lib/ethernet/cmdServer/CmdServer.cpp',
  'src/libApp/cmd/Cmd.cpp',
  'src/libApp/status/State.h',
  'src/libApp/status/State.cpp',
  'src/libApp/status/StateController.cpp',
  'src/pages/Page.h',
  'src/pages/Page.cpp',
  'src/pages/Pages.h',
  'src/pages/htmlHeaders.h',
  'src/pages/index/Index.cpp',
  'src/pages/index/StatusTile.cpp',
  'src/pages/index/AxisTile.cpp',
  'src/pages/mount/Mount.cpp',
  'src/pages/network/Network.cpp',
  'src/pages/focuser/Focuser.cpp',
  'src/pages/rotator/Rotator.cpp',
  'src/pages/auxiliary/Auxiliary.cpp',
  'src/pages/encoders/Encoders.cpp',
]);

function git(...args) {
  return cp.execFileSync('git', ['-c', 'core.quotePath=false', '-c', 'core.safecrlf=false', ...args],
    {cwd: root, encoding: 'utf8', maxBuffer: 8 * 1024 * 1024}).trim();
}

function lines(value) {
  return value ? value.split('\n') : [];
}

function report(title, files) {
  console.log(`\n${title}（${files.length}）`);
  for (const file of files) console.log(`  ${file}`);
}

try {
  const baseline = git('rev-parse', '--verify', '--end-of-options', baselineRef + '^{commit}');
  const target = git('rev-parse', '--verify', '--end-of-options', targetRef + '^{commit}');
  // 使用确定的提交编号，避免把引用名当作命令选项
  if (git('merge-base', baseline, target) !== baseline) throw Error('目标上游不包含指定基线，请先核对分支和基线');
  const upstream = lines(git('diff', '--name-only', baseline, target, '--'));
  const local = lines(git('diff', '--name-only', baseline, '--'));
  const added = lines(git('ls-files', '--others', '--exclude-standard'));
  const custom = [...new Set([...local, ...added])].sort();
  const localFiles = new Set(custom);
  console.log(`已集成原版：${baseline.slice(0, 12)}\n待检查原版：${target.slice(0, 12)}`);
  report('上游新增或修改', upstream);
  report('双方均有改动，需重点核对', upstream.filter(file => localFiles.has(file)));
  report('原版文件中的扩展接入点', custom.filter(file => hooks.has(file)));
  report('集中维护的独立扩展', custom.filter(file => file.startsWith('src/extensions/')));
  report('其他本地差异，含配置、翻译、页面模板及原模块修复', custom.filter(file => !hooks.has(file) && !file.startsWith('src/extensions/')));
  // 直接检查包含路径，迁移遗漏在编译之前即可定位
  const sourceFiles = custom.filter(file => /\.(h|cpp|ino)$/.test(file) && fs.existsSync(path.join(root, file)));
  for (const file of sourceFiles) {
    const source = fs.readFileSync(path.join(root, file), 'utf8');
    for (const match of source.matchAll(/^\s*#include\s*"([^"]+)"/gm)) {
      const include = path.resolve(root, path.dirname(file), match[1]);
      if (!fs.existsSync(include)) throw Error(`${file} 引用了不存在的本地文件 ${match[1]}`);
    }
  }
  console.log('\n本地包含路径检查通过，清单按文件交集提示合并风险，不代替逐段审查和固件编译');
} catch (error) {
  console.error(error.message);
  process.exitCode = 1;
}
