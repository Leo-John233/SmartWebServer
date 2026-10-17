#pragma once
#include "../ui/Text.h"

// 复用 Wi-Fi 页面的开关造型；硬件状态经原来的控制器轮询返回
const char html_board_io_styles[] PROGMEM = R"IO(
<style>
.sws-overview-grid .content { padding:12px 0 0; background:transparent; }
.sws-overview-grid .content > br { display:none; }
.sws-controller-settings { display:grid; grid-template-columns:repeat(var(--settings-columns,3),minmax(0,1fr)); gap:14px; margin:0 0 18px; }
.sws-io-controls { display:contents; }
.sws-io-control, .sws-mount-type { min-width:0; margin:0; padding:18px; border:1px solid var(--border); border-radius:12px; background:var(--field); }
.sws-io-control { display:grid; grid-template-columns:minmax(0,1fr) auto; gap:12px 16px; align-content:start; align-items:center; }
.sws-io-control h3, .sws-mount-type > label { margin:0; font-size:17px; font-weight:600; line-height:1.5; overflow-wrap:anywhere; }
.sws-io-control p { grid-column:1 / -1; margin:0; color:var(--muted); font-size:14px; font-weight:400; line-height:1.7; }
.sws-io-control button.sws-switch { margin:0; width:44px; min-width:44px; height:26px; min-height:26px; padding:3px; background:#494949; flex-shrink:0; }
.sws-io-control button.sws-switch::after { width:18px; height:18px; background:#fff; }
.sws-io-control button.sws-switch[aria-checked='true'] { background:var(--accent); }
.sws-io-control button.sws-switch[aria-checked='true']::after { transform:translateX(18px); }
.sws-io-status { grid-column:1 / -1; padding-top:10px; border-top:1px solid var(--border); color:var(--text); font-size:14px; font-weight:400; line-height:1.6; }
.sws-io-actions, .sws-io-fault { grid-column:1 / -1; }
.sws-io-actions { display:flex; flex-wrap:wrap; gap:8px; }
.sws-controller .sws-io-actions button { min-height:36px; margin:0; padding:6px 14px; font-size:14px; }
.sws-io-fault { color:var(--alert,#d6b786); font-size:14px; line-height:1.6; }
.sws-io-fault:empty, .sws-io-actions[data-selected='false'] { display:none; }
.sws-io-error { grid-column:1 / -1; order:1; margin:0; color:var(--alert,#d6b786); font-size:14px; }
.sws-io-error:empty { display:none; }
.sws-mount-type { display:flex; flex-direction:column; gap:12px; }
.sws-controller .sws-mount-type select { width:100%; min-width:0; min-height:42px; margin:0; font-size:15px; }
.sws-setting-actions { display:flex; flex-wrap:wrap; gap:8px; margin-top:auto; }
.sws-controller .sws-setting-actions button { min-height:36px; margin:0; padding:6px 14px; font-size:14px; }
.sws-setting-actions button[name='revert'] { background:#333; }
@media(max-width:800px) { .sws-controller-settings { grid-template-columns:minmax(0,1fr); gap:10px; } .sws-io-control, .sws-mount-type { padding:16px; } }
</style>
)IO";

const char html_board_io_script[] PROGMEM = R"IO(
<script>
document.addEventListener('DOMContentLoaded', () => {
  const controls = document.querySelector('.sws-io-controls');
  if (!controls) return;
  const buttons = [...controls.querySelectorAll('[role=switch], [data-action]')];
  const switches = buttons.filter(b => b.getAttribute('role') === 'switch');
  let pending = false;
  buttons.forEach((button) => {
    if (!button.dataset.action) {
      const sync = () => button.setAttribute('aria-checked', String(button.dataset.selected === 'true'));
      new MutationObserver(sync).observe(button, {attributes:true,attributeFilter:['data-selected']});
      sync();
    }
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
          body:new URLSearchParams(button.dataset.action ? {action:button.dataset.action} : {setting:button.dataset.setting, enabled:enabled ? '1' : '0'}),
        });
        if (!response.ok) throw Error(response.status === 409 ? controls.dataset.notReady : controls.dataset.error);
        const state = await response.json();
        switches.forEach((b) => b.dataset.selected = String(state[b.dataset.setting]));
        const status = document.getElementById('sws-io-brake-state');
        if (status) status.textContent = state.message;
        const fault = document.getElementById('sws-io-fault');
        if (fault) fault.textContent = state.fault || '';
        const actions = document.getElementById('sws-io-actions');
        if (actions) actions.dataset.selected = String(state.locked);
      } catch (e) {
        error.textContent = e.message === controls.dataset.notReady ? e.message : controls.dataset.error;
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
  "<section class='sws-io-controls' data-confirm=\"" L_IO_BRAKE_OFF_CONFIRM "\" data-error=\"" L_IO_SAVE_FAILED "\" data-not-ready=\"" L_IO_ACTION_REJECTED "\">";
const char html_board_io_activity[] PROGMEM =
  "<div class='sws-io-control'><h3>" L_IO_ACTIVITY_TITLE "</h3>"
  "<button type='button' id='sws-io-activity' class='sws-switch' role='switch' aria-label=\"" L_IO_ACTIVITY_TITLE "\" aria-checked='%s' data-selected='%s' data-setting='activity'></button>"
  "<p>" L_IO_ACTIVITY_HELP "</p></div>";
const char html_board_io_brake[] PROGMEM =
  "<div class='sws-io-control'><h3>" L_IO_BRAKE_TITLE "</h3>"
  "<button type='button' id='sws-io-protection' class='sws-switch' role='switch' aria-label=\"" L_IO_BRAKE_TITLE "\" aria-checked='%s' data-selected='%s' data-setting='protection'></button>"
  "<p>" L_IO_BRAKE_HELP "</p><div id='sws-io-brake-state' class='sws-io-status' role='status'>%s</div>"
  "<div id='sws-io-fault' class='sws-io-fault'>%s</div><div id='sws-io-actions' class='sws-io-actions' data-selected='%s'>"
  "<button type='button' data-action='reset'>" L_IO_BRAKE_RESET "</button>"
  "<button type='button' data-action='retry-stop'>" L_IO_BRAKE_RETRY "</button></div></div>";
