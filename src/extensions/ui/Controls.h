#pragma once

// 页面增强只代理已有控件；设备参数与命令仍由原页面处理
const char html_page_controls[] PROGMEM = R"UI(
<script>
// 滚轮经过当前数字框时先退出焦点，阻止原生步进并保留页面滚动
document.addEventListener('wheel', (event) => {
  const field = event.target;
  if (field instanceof HTMLInputElement && field.type === 'number' && document.activeElement === field) field.blur();
}, { capture: true, passive: true });
// 普通命令未返回时暂缓状态刷新；不重发命令，也不改变操作顺序
window.swsWatchCommand = (request) => {
  window.swsPendingCommands = (window.swsPendingCommands || 0) + 1;
  request.timeout = 5000;
  request.addEventListener('loadend', () => window.swsPendingCommands--, { once: true });
};
// 坐标由单次请求提交，保留错误反馈且不在连接中断时自动重发运动指令
document.addEventListener('DOMContentLoaded', () => {
  const form = document.getElementById('sws-coordinate-form');
  if (!form) return;
  let pending = false;
  form.addEventListener('submit', async (event) => {
    event.preventDefault();
    if (pending || !form.reportValidity()) return;
    const values = new FormData(form);
    const result = form.querySelector('[role=status]');
    if (
      Number(values.get('dec_d')) === 90 &&
      (Number(values.get('dec_m')) || Number(values.get('dec_s')))
    ) {
      result.textContent = form.dataset.invalid;
      return;
    }
    values.set('coord_action', event.submitter?.value || 'go');
    pending = true;
    window.swsPendingCommands = (window.swsPendingCommands || 0) + 1;
    const buttons = [...form.querySelectorAll('button')];
    buttons.forEach((b) => (b.disabled = true));
    result.textContent = form.dataset.send;
    const abort = new AbortController(),
      timer = setTimeout(() => abort.abort(), 15000);
    try {
      const response = await fetch('/mount-ajax-get.txt?' + new URLSearchParams(values), {
        signal: abort.signal,
        cache: 'no-store',
      });
      if (!response.ok) throw Error();
      const reply = (await response.text()).trim();
      result.textContent =
        reply === 'ok'
          ? form.dataset.ok
          : reply === 'invalid'
            ? form.dataset.invalid
            : form.dataset.reject;
    } catch {
      result.textContent = form.dataset.network;
    } finally {
      clearTimeout(timer);
      pending = false;
      window.swsPendingCommands--;
      buttons.forEach((b) => (b.disabled = false));
    }
  });
});

// 为卡片应用职责样式，并给度分秒输入框提供可识别的名称
document.addEventListener('DOMContentLoaded', () => {
  document
    .querySelectorAll('.sws-card-marker')
    .forEach((marker) => marker.closest('.b1').classList.add('sws-' + marker.dataset.card));
  document.querySelectorAll('.sws-field-group').forEach((group) => {
    const title = group.querySelector('legend').textContent;
    group
      .querySelectorAll('input,select')
      .forEach((field, i) =>
        field.setAttribute(
          'aria-label',
          title + ' ' + (field.nextSibling?.textContent.trim() || String(i + 1)),
        ),
      );
  });
});

// 下拉框代理原有按钮，后台状态更新仍通过原来的标识同步
document.addEventListener('DOMContentLoaded', () => {
  const tracking = document.querySelector('.sws-tracking .b1');
  document.querySelectorAll('.sws-choice-source').forEach((source) => {
    const buttons = [...source.querySelectorAll('button')];
    if (!buttons.length) return;
    const select = source.closest('.sws-choice')?.querySelector('select');
    if (!select) return;
    select.onchange = () =>
      buttons[Number(select.value)].dispatchEvent(
        new PointerEvent('pointerdown', { bubbles: true }),
      );
    const sync = () => {
      buttons.forEach((button, i) => {
        select.options[i].disabled = button.disabled;
        if (button.dataset.selected === 'true') select.value = i;
      });
      select.disabled = buttons.every((button) => button.disabled);
    };
    new MutationObserver(sync).observe(source, {
      subtree: true,
      attributes: true,
      attributeFilter: ['data-selected', 'disabled'],
    });
    sync();
  });
  const on = document.getElementById('trk_on'),
    off = document.getElementById('trk_off');
  if (on && off && tracking) {
    const toggle = tracking.querySelector('.sws-switch');
    const result = tracking.querySelector('.sws-help[role=status]');
    if (!toggle || !result) return;
    if (on.dataset.selected === undefined) {
      on.dataset.selected = toggle.getAttribute('aria-checked');
      off.dataset.selected = String(on.dataset.selected !== 'true');
    }
    let pending = false;
    toggle.onclick = async () => {
      if (pending) return;
      const checked = toggle.getAttribute('aria-checked') === 'true';
      pending = true; toggle.disabled = true; result.hidden = true;
      window.swsPendingCommands = (window.swsPendingCommands || 0) + 1;
      const controller = new AbortController(), timer = setTimeout(() => controller.abort(), 5000);
      try {
        const response = await fetch('/mount-ajax-get.txt?dr=' + (checked ? 'T0' : 'T1'), {cache:'no-store',signal:controller.signal});
        if (!response.ok) throw Error();
        const reply = (await response.text()).trim();
        if (reply === 'ok') {
          on.dataset.selected = String(!checked); off.dataset.selected = String(checked);
        } else {
          result.textContent = reply === 'rejected' ? )UI" SWS_JS_TEXT(L_TRACK_REJECTED) R"UI( : reply === 'timeout' ? )UI" SWS_JS_TEXT(L_TRACK_TIMEOUT) R"UI( : )UI" SWS_JS_TEXT(L_TRACK_INVALID) R"UI(;
          result.hidden = false;
        }
      } catch (_) { result.textContent = )UI" SWS_JS_TEXT(L_TRACK_TIMEOUT) R"UI(; result.hidden = false; }
      finally {
        clearTimeout(timer); window.swsPendingCommands--; pending = false; sync();
        if (window.autoFastRun) autoFastRun();
      }
    };
    const sync = () => {
      toggle.setAttribute('aria-checked', String(on.dataset.selected === 'true'));
      toggle.disabled = pending || (on.disabled && off.disabled);
    };
    const observer = new MutationObserver(sync);
    [on, off].forEach((b) =>
      observer.observe(b, { attributes: true, attributeFilter: ['data-selected', 'disabled'] }),
    );
    sync();
  }
});

</script>
)UI";
