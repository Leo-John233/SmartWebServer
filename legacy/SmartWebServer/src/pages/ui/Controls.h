#pragma once

// 页面增强只代理已有控件；设备参数与命令仍由原页面处理。
const char html_page_controls[] PROGMEM = R"UI(
<script>
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
  // 方位控制标题去掉冒号，保留倍率与状态节点
  const guideTitle = document.querySelector('.sws-guide>.b1>div:first-child');
  if (guideTitle)
    guideTitle.childNodes.forEach((node) => {
      if (node.nodeType === Node.TEXT_NODE)
        node.textContent = node.textContent.replace(/[:：]\s*$/, '');
    });
  document.querySelectorAll('.sws-choice-source').forEach((source) => {
    const buttons = [...source.querySelectorAll('button')];
    if (!buttons.length) return;
    const label = document.createElement('label');
    label.className = 'sws-choice';
    const text = document.createElement('span');
    text.textContent = source.dataset.label;
    const select = document.createElement('select');
    buttons.forEach((button, i) => {
      const option = document.createElement('option');
      option.value = i;
      option.textContent = button.title || button.textContent;
      select.appendChild(option);
    });
    label.append(text, select);
    source.before(label);
    label.appendChild(source);
    source.classList.add('sws-choice-originals');
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
    if (tracking && source.classList.contains('sws-guide-choice')) {
      let collapse = tracking.querySelector('.collapsible');
      while (collapse && collapse.parentElement !== tracking) collapse = collapse.parentElement;
      tracking.insertBefore(label, collapse || null);
    }
  });
  const on = document.getElementById('trk_on'),
    off = document.getElementById('trk_off');
  if (on && off) {
    const wrap = document.createElement('div');
    wrap.className = 'sws-track-switch';
    const name = document.createElement('span');
    name.textContent =
      on.dataset.label || tracking.querySelector('div').textContent.replace(/:$/, '');
    const toggle = document.createElement('button');
    toggle.className = 'sws-switch';
    toggle.type = 'button';
    toggle.setAttribute('role', 'switch');
    toggle.setAttribute('aria-label', name.textContent);
    toggle.setAttribute('aria-checked', 'false');
    on.before(wrap);
    wrap.append(name, toggle);
    on.hidden = true;
    off.hidden = true;
    toggle.onclick = () => {
      const checked = toggle.getAttribute('aria-checked') === 'true';
      (checked ? off : on).dispatchEvent(new PointerEvent('pointerdown', { bubbles: true }));
    };
    const sync = () => {
      toggle.setAttribute('aria-checked', String(on.dataset.selected === 'true'));
      toggle.disabled = on.disabled && off.disabled;
    };
    const observer = new MutationObserver(sync);
    [on, off].forEach((b) =>
      observer.observe(b, { attributes: true, attributeFilter: ['data-selected', 'disabled'] }),
    );
    sync();
  }
});

// 内容页使用独立文档，离开时由浏览器清理轮询与事件，导航和页头保持不动
if (window.parent !== window && window.parent.swsNavigate) {
  document.documentElement.classList.add('sws-embedded');
  document.addEventListener('DOMContentLoaded', () => {
    document.querySelectorAll('form').forEach((form) => {
      if (form.method.toLowerCase() === 'get') {
        const input = document.createElement('input');
        input.type = 'hidden';
        input.name = 'view';
        input.value = '1';
        form.appendChild(input);
      }
    });
    document.querySelectorAll('.t a').forEach((a) =>
      a.addEventListener('click', (e) => {
        if (!e.ctrlKey && !e.metaKey && !e.shiftKey) {
          e.preventDefault();
          parent.swsNavigate(a.pathname, true);
        }
      }),
    );
  });
}
</script>
)UI";
