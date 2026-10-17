#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\ui\\Navigation.h"
#pragma once
#include "Text.h"

// 导航仅加载当前标签内容，浏览器前进后退同步到同一内容窗口
const char html_shell_script[] PROGMEM = R"UI(
<script>
(() => {
  // 意外以普通地址打开内容页时直接纠正，避免出现嵌套导航
  if (parent !== window && parent.swsNavigate) {
    location.replace(location.pathname + '?view=1');
    return;
  }
  const frame = document.getElementById('sws-frame'),
    hint = document.getElementById('sws-load'),
    links = [...document.querySelectorAll('.t a')];
  let sequence = 0,
    timer, requestedPath = '', waiting = false, timedOut = false;
  function recover(ticket) {
    if(ticket !== sequence)return;
    clearTimeout(timer);
    waiting=false;
    hint.textContent = )UI" SWS_JS_TEXT(L_UI_SLOW_LOAD) R"UI(;
    frame.style.pointerEvents = '';
    try {
      const child=frame.contentWindow;
      if(child.document.body)child.document.body.inert=false;
      if(child.swsResumePolling)child.swsResumePolling();
    } catch(e) {}
    resize();
  }
  // 参考实心设备图标重绘剪影，望远镜保留倾斜镜筒、镜口与底座
  // 图形内嵌且继承导航颜色，离线可用并同步适配桌面和手机
  const icons = {
    index:
      '<path d="M8 5h16a3 3 0 0 1 3 3v16a3 3 0 0 1-3 3H8a3 3 0 0 1-3-3V8a3 3 0 0 1 3-3Zm3 5v12h10V10Z" fill-rule="evenodd"/><path d="M9 1h3v4H9zm6 0h3v4h-3zm6 0h3v4h-3zM9 27h3v4H9zm6 0h3v4h-3zm6 0h3v4h-3zM1 9h4v3H1zm0 6h4v3H1zm0 6h4v3H1zM27 9h4v3h-4zm0 6h4v3h-4zm0 6h4v3h-4z" opacity=".72"/><rect x="13" y="12" width="6" height="8" rx="1"/>',
    mount:
      '<path d="M13 20h5l1 5 4 4H9l4-5Z" opacity=".78"/><g transform="rotate(-28 16 14)"><path d="M12 8h13c5 0 5 12 0 12H12Zm13.5 1.8c-2.6 0-2.6 8.4 0 8.4s2.6-8.4 0-8.4Z" fill-rule="evenodd"/><path d="M7 10h4v8H7a1 1 0 0 1-1-1v-6a1 1 0 0 1 1-1Z" opacity=".8"/><path d="M2 12h3v4H2a1 1 0 0 1-1-1v-2a1 1 0 0 1 1-1Z" opacity=".65"/></g>',
    focuser:
      '<path d="M3 2h7v3H5v5H2V3a1 1 0 0 1 1-1Zm19 0h7a1 1 0 0 1 1 1v7h-3V5h-5ZM2 22h3v5h5v3H3a1 1 0 0 1-1-1Zm25 0h3v7a1 1 0 0 1-1 1h-7v-3h5Z" opacity=".72"/><path d="M16 6a10 10 0 1 0 0 20 10 10 0 0 0 0-20Zm0 4a6 6 0 1 1 0 12 6 6 0 0 1 0-12Z" fill-rule="evenodd"/><circle cx="16" cy="16" r="3"/>',
    rotator:
      '<path d="M6.2 8.4A12 12 0 0 1 27 10h4L25 17l-6-7h4a8 8 0 0 0-13.7.8Z"/><path d="M25.8 23.6A12 12 0 0 1 5 22H1l6-7 6 7H9a8 8 0 0 0 13.7-.8Z" opacity=".72"/><circle cx="16" cy="16" r="3"/>',
    auxiliary:
      '<path d="M8 3h16a6 6 0 0 1 0 12H8A6 6 0 0 1 8 3Zm0 3a3 3 0 1 0 0 6 3 3 0 0 0 0-6Z" fill-rule="evenodd"/><path d="M8 18h16a6 6 0 0 1 0 12H8a6 6 0 0 1 0-12Zm16 3a3 3 0 1 0 0 6 3 3 0 0 0 0-6Z" fill-rule="evenodd" opacity=".72"/>',
    enc: '<path d="M16 2a14 14 0 1 0 0 28 14 14 0 0 0 0-28Zm-1 2h2v4h-2ZM5.1 8l1.4-1.4 2.8 2.8L8 10.8ZM4 15h4v2H4Zm3 10-1.4-1.4 2.8-2.8L10 22.2ZM15 24h2v4h-2Zm10-1-1.4 1.4-2.8-2.8 1.4-1.4ZM24 15h4v2h-4Zm.9-7-2.8 2.8-1.4-1.4 2.8-2.8ZM16 11a5 5 0 1 0 0 10 5 5 0 0 0 0-10Z" fill-rule="evenodd"/><circle cx="16" cy="16" r="2.5" opacity=".72"/>',
    net: '<path d="M1 9a23 23 0 0 1 30 0l-3 3a19 19 0 0 0-24 0Zm5 6a15 15 0 0 1 20 0l-3 3a11 11 0 0 0-14 0Zm5 6a8 8 0 0 1 10 0l-5 6Z"/>',
    other:
      '<path d="M5 4h22a2 2 0 0 1 2 2v20a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2Zm3 5v5h5V9Zm11 0v5h5V9ZM8 19v4h5v-4Zm11 0v4h5v-4Z" fill-rule="evenodd"/>',
  };
  const nav = document.createElement('nav');
  nav.className = 'sws-nav';
  nav.setAttribute('aria-label', )UI" SWS_JS_TEXT(L_UI_NAVIGATION) R"UI();
  links.forEach((a) => {
    const text = a.textContent,
      key = a.pathname.split('/').pop().split('.')[0];
    a.innerHTML =
      '<svg viewBox="0 0 32 32" aria-hidden="true" focusable="false">' +
      (icons[key] || icons.other) +
      '</svg>';
    const label = document.createElement('span');
    label.textContent = text;
    a.appendChild(label);
    nav.appendChild(a);
  });
  const extras = document.createElement('div');
  extras.className = 'sws-extra-nav';
  links
    .filter((a) => !['/index.htm', '/mount.htm', '/net.htm'].includes(a.pathname))
    .forEach((a) => extras.appendChild(a));
  nav.appendChild(extras);
  const more = document.createElement('button');
  more.type = 'button';
  more.className = 'sws-nav-more';
  more.setAttribute('aria-expanded', 'false');
  more.innerHTML =
    '<svg viewBox="0 0 32 32" aria-hidden="true" focusable="false"><circle cx="6" cy="16" r="2.5"/><circle cx="16" cy="16" r="2.5"/><circle cx="26" cy="16" r="2.5"/></svg><span></span>';
  more.querySelector('span').textContent = )UI" SWS_JS_TEXT(L_UI_MORE) R"UI(;
  const closeMore = () => {
    nav.dataset.expanded = 'false';
    more.setAttribute('aria-expanded', 'false');
  };
  more.onclick = () => {
    const expanded = nav.dataset.expanded !== 'true';
    nav.dataset.expanded = String(expanded);
    more.setAttribute('aria-expanded', String(expanded));
  };
  if (extras.children.length) nav.appendChild(more);
  document.querySelector('.t').appendChild(nav);
  addEventListener('keydown', (e) => {
    if (e.key === 'Escape') closeMore();
  });
  document.addEventListener('click', (e) => {
    if (!nav.contains(e.target)) closeMore();
  });
  // 版本沿用原始页头的服务端输出，不额外写入品牌名称
  // 左侧配置名称保留原始页头来源，说明从当前语言资源读取
  const note = document.createElement('div');
  note.className = 'sws-sidebar-note';
  note.textContent = document.getElementById('sws-sidebar-label').textContent;
  document.querySelector('.t').appendChild(note);
  const topbar = document.createElement('header');
  topbar.className = 'sws-topbar';
  topbar.innerHTML = '<div><h1 id="sws-page-title"></h1><p id="sws-page-description"></p></div>';
  const version = document.createElement('span');
  version.className = 'sws-version';
  version.textContent = document.querySelector('.t td:last-child')?.textContent || '';
  if (version.textContent) topbar.appendChild(version);
  hint.before(topbar);
  const normalize = (path) => (path === '/' ? '/index.htm' : path);
  function resize() {
    const bottom = matchMedia('(max-width:700px)').matches
      ? document.querySelector('.t').getBoundingClientRect().height
      : 0;
    frame.style.height =
      Math.max(180, innerHeight - frame.getBoundingClientRect().top - bottom) + 'px';
  }
  window.swsNavigate = async (path, push = false) => {
    path = normalize(path);
    if (!links.some((a) => a.pathname === path)) return;
    closeMore();
    more.dataset.active = String(!['/index.htm', '/mount.htm', '/net.htm'].includes(path));
    const ticket = ++sequence;
    waiting=true; timedOut=false;
    clearTimeout(timer);
    hint.textContent = )UI" SWS_JS_TEXT(L_UI_OPENING) R"UI(;
    frame.style.pointerEvents = 'none';
    // 正在按住运动按钮时先松开并等待停止请求发送，再销毁旧内容页
    try {
      const child = frame.contentWindow;
      if (child.swsPausePolling) child.swsPausePolling();
      if (child.document.body) child.document.body.inert = true;
      const stops = (child.swsMotionStops || []).map((stop) => stop()).filter(Boolean);
      if (stops.length) {
        let deadline;
        try {
          await Promise.race([Promise.all(stops),new Promise((_,reject)=>{deadline=setTimeout(()=>reject(Error('stop timeout')),5000);})]);
        } finally { clearTimeout(deadline); }
      }
    } catch (e) { recover(ticket); return; }
    if (ticket !== sequence) return;
    if (push && normalize(location.pathname) !== path) history.pushState({}, '', path);
    links.forEach((a) => {
      a.removeAttribute('style');
      if (a.pathname === path) a.setAttribute('aria-current', 'page');
      else a.removeAttribute('aria-current');
    });
    document.getElementById('sws-page-title').textContent = links.find(
      (a) => a.pathname === path,
    ).textContent;
    document.getElementById('sws-page-description').textContent = '';
    frame.title = document.getElementById('sws-page-title').textContent;
    frame.style.visibility='hidden';
    requestedPath = path;
    frame.contentWindow.location.replace(path + '?view=1');
    resize();
    timer = setTimeout(() => {
      if(ticket!==sequence)return;
      timedOut=true;
      try { frame.contentWindow.stop(); } catch(e) {}
      recover(ticket);
    }, 12000);
  };
  frame.addEventListener('load', () => {
    if(timedOut)return;
    // 公共资源失败时不把缺少样式和交互的旧布局当成已加载完成。
    try {
      const child=frame.contentWindow, doc=child.document;
      if(child.location.pathname!==requestedPath)return;
      if(!doc.querySelector('.b') || !child.getComputedStyle(doc.documentElement).getPropertyValue('--bg').trim() ||
         typeof child.swsWatchCommand!=='function') {
        recover(sequence); return;
      }
    } catch(e) { recover(sequence); return; }
    clearTimeout(timer);
    waiting=false;
    hint.textContent = '';
    frame.style.pointerEvents = '';
    frame.style.visibility = '';
    resize();
  });
  addEventListener('online', () => { if(waiting || timedOut) recover(sequence); });
  addEventListener('resize', resize);
  new ResizeObserver(resize).observe(document.querySelector('.t'));
  links.forEach((a) =>
    a.addEventListener('click', (e) => {
      if (e.button !== 0 || e.ctrlKey || e.metaKey || e.shiftKey || e.altKey) return;
      e.preventDefault();
      swsNavigate(a.pathname, true);
    }),
  );
  addEventListener('popstate', () => swsNavigate(location.pathname));
  document.documentElement.classList.add('sws-shell');
  swsNavigate(location.pathname);
})();
</script></body></html>
)UI";
