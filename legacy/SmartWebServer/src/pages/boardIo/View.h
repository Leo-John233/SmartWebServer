#pragma once
#include "../ui/Text.h"

// 复用 Wi-Fi 页面的开关造型；硬件状态经原来的控制器轮询返回。
const char html_board_io_styles[] PROGMEM = R"IO(
<style>
.sws-io-controls { display:grid; gap:0; margin:0 0 20px; }
.sws-io-control { display:grid; grid-template-columns:minmax(0,1fr) auto; gap:12px 20px; align-items:center; padding:18px 0; border-bottom:1px solid var(--border); }
.sws-io-control h3 { margin:0 0 6px; font-size:17px; line-height:1.5; }
.sws-io-control p { margin:0; color:var(--muted); font-size:14px; line-height:1.7; }
.sws-io-control button.sws-switch { margin:0; width:44px; min-width:44px; height:26px; min-height:26px; padding:3px; background:#494949; flex-shrink:0; }
.sws-io-control button.sws-switch::after { width:18px; height:18px; background:#fff; }
.sws-io-control button.sws-switch[aria-checked='true'] { background:#8b2020; }
.sws-io-control button.sws-switch[aria-checked='true']::after { transform:translateX(18px); }
.sws-io-status { grid-column:1 / -1; color:var(--text); font-size:14px; line-height:1.6; }
.sws-io-error { margin:10px 0; color:var(--alert,#d6b786); font-size:14px; }
@media(max-width:480px) { .sws-io-control { gap:10px 12px; padding:16px 0; } .sws-io-control h3 { font-size:16px; } }
</style>
)IO";

const char html_board_io_script[] PROGMEM = R"IO(
<script>
document.addEventListener('DOMContentLoaded', () => {
  const controls = document.querySelector('.sws-io-controls');
  if (!controls) return;
  const buttons = [...controls.querySelectorAll('[role=switch]')];
  let pending = false;
  buttons.forEach((button) => {
    const sync = () => button.setAttribute('aria-checked', String(button.dataset.selected === 'true'));
    new MutationObserver(sync).observe(button, {attributes:true,attributeFilter:['data-selected']});
    sync();
    button.addEventListener('click', async () => {
      if (pending) return;
      const enabled = button.dataset.selected !== 'true';
      if (button.dataset.setting === 'protection' && !enabled && !confirm(controls.dataset.confirm)) return;
      pending = true;
      window.swsPendingCommands = (window.swsPendingCommands || 0) + 1;
      buttons.forEach((b) => b.disabled = true);
      const error = controls.querySelector('[role=alert]');
      error.textContent = '';
      const abort = new AbortController(), timer = setTimeout(() => abort.abort(), 8000);
      try {
        const response = await fetch('/board-io', {
          method:'POST', cache:'no-store', signal:abort.signal,
          headers:{'Content-Type':'application/x-www-form-urlencoded'},
          body:new URLSearchParams({setting:button.dataset.setting, enabled:enabled ? '1' : '0'}),
        });
        if (!response.ok) throw Error();
        const state = await response.json();
        buttons.forEach((b) => b.dataset.selected = String(state[b.dataset.setting]));
        const status = document.getElementById('sws-io-brake-state');
        if (status) status.textContent = state.message;
      } catch {
        error.textContent = controls.dataset.error;
      } finally {
        clearTimeout(timer);
        pending = false;
        window.swsPendingCommands--;
        buttons.forEach((b) => b.disabled = false);
      }
    });
  });
});
</script>
)IO";

const char html_board_io_begin[] PROGMEM =
  "<section class='sws-io-controls' data-confirm=\"" L_IO_RELEASE_CONFIRM "\" data-error=\"" L_IO_SAVE_FAILED "\">";
const char html_board_io_activity[] PROGMEM =
  "<div class='sws-io-control'><div><h3>" L_IO_ACTIVITY_TITLE "</h3><p>" L_IO_ACTIVITY_HELP "</p></div>"
  "<button type='button' id='sws-io-activity' class='sws-switch' role='switch' aria-label=\"" L_IO_ACTIVITY_TITLE "\" aria-checked='%s' data-selected='%s' data-setting='activity'></button></div>";
const char html_board_io_brake[] PROGMEM =
  "<div class='sws-io-control'><div><h3>" L_IO_BRAKE_TITLE "</h3><p>" L_IO_BRAKE_HELP "</p></div>"
  "<button type='button' id='sws-io-protection' class='sws-switch' role='switch' aria-label=\"" L_IO_BRAKE_TITLE "\" aria-checked='%s' data-selected='%s' data-setting='protection'></button>"
  "<div id='sws-io-brake-state' class='sws-io-status' role='status'>%s</div></div>";
