
// 普通命令未返回时暂缓状态刷新；不重发命令，也不改变操作顺序。
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
    const result = document.createElement('p');
    result.className = 'sws-help'; result.setAttribute('role', 'status'); result.hidden = true;
    wrap.after(result);
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
          result.textContent = reply === 'rejected' ? "主控拒绝跟踪操作，请检查停放、回零及驱动状态" : reply === 'timeout' ? "主控应答超时，请检查连接" : "主控应答异常，请重试";
          result.hidden = false;
        }
      } catch (_) { result.textContent = "主控应答超时，请检查连接"; result.hidden = false; }
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

document.addEventListener('DOMContentLoaded', () => {
  // 有符号原值保留在原输入框，方向选择只提供更直观的显示。
  function signedInput(source, directions) {
    if (!source) return;
    const box = document.createElement('div');
    box.className = 'sws-signed-input';
    source.before(box);
    const value = document.createElement('input'), sign = document.createElement('select');
    value.type = 'number'; value.min = '0'; value.max = Math.max(Math.abs(Number(source.min)), Math.abs(Number(source.max)));
    value.step = source.step || '1'; value.required = true;
    value.defaultValue = String(Math.abs(Number(source.value)));
    value.setAttribute('aria-label', source.getAttribute('aria-label') || source.closest('label,fieldset').textContent.trim());
    directions.forEach(([key, text]) => sign.add(new Option(text, key)));
    sign.value = source.value.startsWith('-') ? '-' : '+';
    sign.selectedOptions[0].defaultSelected = true;
    sign.setAttribute('aria-label', value.getAttribute('aria-label'));
    source.hidden = true;
    const units = box.closest('.sws-unit-inputs');
    if (units) { box.append(source, value); units.append(sign); }
    else box.append(source, sign, value);
    const sync = () => { source.value = value.value === '' ? '' : (sign.value === '-' ? '-' : '') + value.value; };
    value.addEventListener('input', sync); sign.addEventListener('change', sync);
  }
  const site = document.getElementById('sws-site-form');
  if (site) {
    ['g1','g2','g3','t1','t2','t3'].forEach(name => {
      const input = site.elements[name];
      if (!input) return;
      const unit = input.nextSibling, label = document.createElement('label'), text = document.createElement('span');
      label.className = 'sws-site-unit';
      text.textContent = unit?.nodeType === Node.TEXT_NODE ? unit.textContent.trim() : '';
      input.before(label); label.append(input, text);
      if (unit?.nodeType === Node.TEXT_NODE) unit.remove();
    });
    signedInput(site.elements.g1, [['-', 'E'], ['+', 'W']]);
    signedInput(site.elements.t1, [['+', 'N'], ['-', 'S']]);
    const hours = site.elements.u1, minutes = site.elements.u2;
    const group = hours.closest('fieldset'), select = document.createElement('select');
    select.className = 'sws-timezone-select';
    select.setAttribute('aria-label', group.querySelector('legend').textContent);
    const current = (Math.abs(Number(hours.value))*60 + Number(minutes.value)) * (hours.value.startsWith('-') ? -1 : 1);
    const offsets = [];
    for (let west = -840; west <= 720; west += 15) if (Math.abs(west)%60 !== 15) offsets.push(west);
    if (!offsets.includes(current)) offsets.push(current);
    offsets.sort((a,b) => b-a).forEach((west) => {
      const zone = -west, magnitude = Math.abs(zone);
      const text = 'UTC' + (zone < 0 ? '−' : '+') + String(Math.floor(magnitude/60)).padStart(2,'0') + ':' + String(magnitude%60).padStart(2,'0');
      const option = new Option(text, west, west === current, west === current); select.add(option);
    });
    group.querySelector('.sws-unit-inputs').hidden = true;
    group.querySelector('.sws-help').hidden = true;
    group.append(select);
    select.onchange = () => {
      const west = Number(select.value);
      hours.value = (west < 0 ? '-' : '') + Math.floor(Math.abs(west)/60);
      minutes.value = Math.abs(west)%60;
    };
    const updateLocalTime = () => {
      const date=document.getElementById('date_ut').textContent.match(/^(\d{1,2})\/(\d{1,2})\/(\d{2,4})$/), time=document.getElementById('time_ut').textContent.match(/^(\d{2}):(\d{2}):(\d{2})$/);
      const output=document.getElementById('sws-local-time');
      if(!date||!time){output.value='—';return;}
      const h=hours.defaultValue,m=[...minutes.options].find(o=>o.defaultSelected)?.value || '0',west=(Math.abs(Number(h))*60+Number(m))*(h.startsWith('-')?-1:1);
      const d=new Date(Date.UTC(Number(date[3])+(date[3].length===2?2000:0),Number(date[1])-1,Number(date[2]),Number(time[1]),Number(time[2]),Number(time[3]))-west*60000);
      output.value=d.toISOString().slice(0,19).replace('T',' ');
    };
    for(const id of ['date_ut','time_ut']) new MutationObserver(updateLocalTime).observe(document.getElementById(id),{childList:true,characterData:true,subtree:true});
    updateLocalTime();
  }
  const limits = document.getElementById('sws-limits-form');
  if (limits) ['hl','el','wl'].forEach(name => signedInput(limits.elements[name], [['+','+'], ['-','−']]));
  document.querySelectorAll('.sws-settings-editor').forEach((form) => {
    const card = form.closest('.sws-editor'), edit = card.querySelector('[data-edit-form]');
    const fields = [...form.querySelectorAll('input,select')];
    const remember = () => fields.forEach(field => {
      if (field.tagName === 'SELECT') [...field.options].forEach(option => option.defaultSelected = option.selected);
      else field.defaultValue = field.value;
    });
    remember();
    form.addEventListener('submit', remember);
    const setEditing = (editing) => {
      card.dataset.editing = String(editing);
      fields.forEach(field => { if (field.tagName === 'SELECT') field.disabled = !editing; else field.readOnly = !editing; });
      edit.disabled = editing; edit.setAttribute('aria-expanded', String(editing));
    };
    edit.setAttribute('aria-controls', form.id);
    edit.onclick = () => { setEditing(true); form.querySelector('input:not([hidden]),select:not(:disabled)').focus(); };
    form.addEventListener('reset', () => setTimeout(() => setEditing(false), 0));
    // 表单数据构造完毕后再锁定下拉框，避免 disabled 导致原参数丢失。
    form.addEventListener('formdata', () => queueMicrotask(() => setEditing(false)));
    setEditing(false);
  });
  const park = document.getElementById('park'), unpark = document.getElementById('unpark');
  if (park && unpark) {
    const sync = () => { park.hidden = !unpark.disabled; unpark.hidden = unpark.disabled; };
    new MutationObserver(sync).observe(unpark, {attributes:true,attributeFilter:['disabled']});
    sync();
  }
});

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
