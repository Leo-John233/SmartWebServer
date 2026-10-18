#pragma once
#include "Text.h"

// 页头由固件直接输出，脚本只等待已发起的运动停止，不重建页面布局
const char html_page_navigation[] PROGMEM = R"UI(
<script>
// 正文可能仍在传输，使用事件委托，让首屏可见时运动停止保护就已生效。
(() => {
  let leaving = false;
  // 浏览器从历史缓存恢复原文档时，允许再次通过原生链接切页
  addEventListener('pageshow', () => { leaving = false; });
  document.addEventListener('click', async event => {
    const a = event.target instanceof Element && event.target.closest('.sws-nav a');
    if (!a) return;
    if (event.button !== 0 || event.ctrlKey || event.metaKey || event.shiftKey || event.altKey) return;
    if (leaving) { event.preventDefault(); return; }
    const stops = (window.swsMotionStops || []).map(stop => stop()).filter(Boolean);
    // 普通跳转等待新页面提交期间旧文档仍存活，先取消只读轮询，释放连接。
    // 不取消命令，也不接管链接；历史返回由原 pageshow 恢复轮询。
    if (!stops.length) {
      if (window.swsPausePolling) swsPausePolling();
      return;
    }
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
      const topbar = document.querySelector('.sws-topbar');
      if (topbar) topbar.appendChild(hint);
    } finally { clearTimeout(deadline); }
  });
})();
</script>
)UI";
