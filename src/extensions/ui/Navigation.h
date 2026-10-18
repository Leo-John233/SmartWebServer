#pragma once
#include "Text.h"

// 页头由固件直接输出，脚本只等待已发起的运动停止，不重建页面布局
const char html_page_navigation[] PROGMEM = R"UI(
<script>
document.addEventListener('DOMContentLoaded', () => {
  const links = [...document.querySelectorAll('.sws-nav a')];
  if (!links.length) return;
  const topbar = document.querySelector('.sws-topbar');
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
