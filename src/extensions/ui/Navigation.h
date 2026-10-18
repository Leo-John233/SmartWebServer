#pragma once
#include "Text.h"

// 只增强原版页头外观，普通切页保留浏览器的链接、表单及历史记录
const char html_page_navigation[] PROGMEM = R"UI(
<script>
document.addEventListener('DOMContentLoaded', () => {
  const links = [...document.querySelectorAll('.t a')];
  if (!links.length) return;
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
  document.querySelector('.t').after(topbar);
  const path = location.pathname === '/' ? '/index.htm' : location.pathname;
  const selected = links.find(a => a.pathname === path) || links.find(a => a.hasAttribute('style'));
  links.forEach(a => {
    a.removeAttribute('style');
    if (a === selected) a.setAttribute('aria-current', 'page');
  });
  document.getElementById('sws-page-title').textContent = selected?.textContent || '';
  more.dataset.active = String(selected && !['/index.htm', '/mount.htm', '/net.htm'].includes(selected.pathname));
  document.documentElement.classList.add('sws-page');
  let leaving = false;
  // 浏览器从历史缓存恢复原文档时，允许再次通过原生链接切页
  addEventListener('pageshow', () => { leaving = false; });
  links.forEach(a => a.addEventListener('click', async event => {
    if (event.button !== 0 || event.ctrlKey || event.metaKey || event.shiftKey || event.altKey) return;
    if (leaving) { event.preventDefault(); return; }
    const stops = (window.swsMotionStops || []).map(stop => stop()).filter(Boolean);
    // 没有待完成的运动停止时直接使用原生链接，不加入切页队列或固定等待
    if (!stops.length) return;
    event.preventDefault();
    leaving = true;
    if (window.swsPausePolling) swsPausePolling();
    let deadline;
    try {
      await Promise.race([Promise.all(stops), new Promise((_, reject) => {
        deadline = setTimeout(() => reject(Error('stop timeout')), 5000);
      })]);
      location.assign(a.href);
    } catch (_) {
      leaving = false;
      if (window.swsResumePolling) swsResumePolling();
      const hint = document.createElement('p');
      hint.setAttribute('role', 'status');
      hint.textContent = )UI" SWS_JS_TEXT(L_UI_SLOW_LOAD) R"UI(;
      topbar.appendChild(hint);
    } finally { clearTimeout(deadline); }
  }));
});
</script>
)UI";
