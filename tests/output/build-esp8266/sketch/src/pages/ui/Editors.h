#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\ui\\Editors.h"
#pragma once

// 位置、限位卡片的显示与编辑状态；表单仍提交原来的参数。
const char html_editor_styles[] PROGMEM = R"UI(
<style>
.sws-control-grid { grid-template-columns:repeat(2,minmax(0,1fr)); grid-template-areas:'tracking guide'; }
.sws-mount > .sws-target { grid-area:auto; }
.sws-site-limits { display:grid; grid-template-columns:repeat(2,minmax(0,1fr)); gap:18px; align-items:stretch; }
.sws-mount .sws-editor { width:100%; min-height:0; padding:20px; margin:0; }
.sws-editor [hidden], .sws-park-toggle [hidden] { display:none !important; }
.sws-editor-heading { display:flex; align-items:center; justify-content:space-between; gap:12px; padding-bottom:16px; margin-bottom:20px; border-bottom:1px solid var(--border); }
.sws-editor-heading h2 { margin:0; font-size:20px; line-height:1.4; }
.sws-mount .sws-edit-button { display:inline-flex; align-items:center; justify-content:center; gap:6px; flex-shrink:0; width:auto; min-height:36px; margin:0; padding:8px 12px; font-size:14px; }
.sws-edit-button svg { width:16px; height:16px; }
.sws-editor .sws-site-fields { display:grid; grid-template-columns:1fr; gap:22px; }
.sws-editor .sws-site-fields fieldset { margin:0; padding:0; border:0; background:transparent; }
.sws-editor legend, .sws-editor .sws-field-label { display:block; padding:0; margin:0 0 10px; font-size:15px; font-weight:600; line-height:1.6; color:var(--text); }
.sws-editor .sws-unit-inputs { display:flex; align-items:center; gap:7px; width:100%; max-width:none; }
.sws-editor .sws-unit-inputs input { flex:1; min-width:0; width:0 !important; margin:0; }
.sws-editor .sws-unit-inputs select { margin:0; min-width:0; width:64px; }
.sws-editor input, .sws-editor select { height:44px; font-size:17px; color:var(--text); font-weight:500; }
.sws-editor input[readonly], .sws-editor select:disabled { opacity:1; cursor:default; color:var(--text); background-color:var(--field); -webkit-text-fill-color:var(--text); }
.sws-editor .sws-unit-inputs > .sws-signed-input { flex:1; min-width:0; }
.sws-site-unit { position:relative; display:block; flex:1; min-width:0; }
.sws-editor .sws-site-unit input:not([hidden]) { width:100% !important; padding:8px 26px 8px 6px; margin:0; appearance:textfield; -moz-appearance:textfield; }
.sws-site-unit input::-webkit-inner-spin-button, .sws-site-unit input::-webkit-outer-spin-button { -webkit-appearance:none; margin:0; }
.sws-editor .sws-unit-inputs > select { padding:0 4px; text-align:center; }
.sws-site-unit > span { position:absolute; right:8px; top:50%; transform:translateY(-50%); color:var(--text); font-size:20px; font-weight:600; line-height:1; pointer-events:none; }
.sws-signed-input { display:flex; align-items:center; gap:8px; }
.sws-signed-input > input:not([hidden]) { width:0 !important; flex:1; min-width:0; margin:0; }
.sws-editor .sws-signed-input > select { width:62px; flex-shrink:0; }
.sws-editor .sws-timezone-select { width:100%; margin:0; }
.sws-site .sws-unit-inputs { display:grid; grid-template-columns:repeat(4,minmax(0,1fr)); gap:8px; }
.sws-site .sws-unit-inputs > select { width:100%; height:44px; }
.sws-site .sws-site-fields { gap:26px; }
.sws-site .sws-clock-action button { background:var(--accent); width:100%; height:44px; margin:0; }
.sws-site .sws-clock-controls { display:grid; grid-template-columns:minmax(0,3fr) minmax(90px,1fr); align-items:end; border:0; padding-top:0; margin-top:26px; gap:10px; }
.sws-site #sws-local-time { width:100%; margin:0; font-size:16px; text-align:left; padding:8px 12px; }
.sws-site.sws-editor .sws-clock-action small { display:block; width:100%; }
.sws-site.sws-editor .sws-clock-action button { width:100%; white-space:normal; overflow-wrap:anywhere; padding:6px; }
.sws-site .sws-clock-controls > form { min-width:0; }
.sws-editor .sws-help { margin:10px 0 0; font-size:12px; line-height:1.7; color:var(--muted); }
.sws-editor .sws-limits-form { display:grid; gap:22px; }
.sws-editor .sws-number-field { display:block; margin:0; }
.sws-editor .sws-number-field > span { display:block; margin-bottom:10px; font-size:15px; font-weight:600; line-height:1.6; }
.sws-editor .sws-number-field > input { width:100% !important; text-align:center; margin:0; }
.sws-editor .sws-number-field .sws-signed-input > input { width:0 !important; }
.sws-editor-footer { display:flex; justify-content:flex-end; gap:10px; margin-top:20px; }
.sws-editor-footer button { min-width:95px; margin:0; }
.sws-editor[data-editing='false'] .sws-editor-footer { display:none; }
.sws-backlash-editor { padding-top:16px; border-top:1px solid var(--border); }
.sws-backlash-editor summary { cursor:pointer; font-size:14px; color:var(--muted); }
.sws-backlash-editor .sws-number-field { margin-top:16px; }
.sws-clock-controls { display:flex; align-items:center; justify-content:space-between; gap:12px; margin-top:22px; padding-top:18px; border-top:1px solid var(--border); }
.sws-editor .sws-site-clock { min-width:0; }
.sws-clock-value strong { display:block; font-size:19px; line-height:1.6; font-weight:600; font-variant-numeric:tabular-nums; }
.sws-sidereal-clock { display:flex; gap:10px; margin-top:6px; font-size:13px; color:var(--muted); }
.sws-sidereal-clock strong { font-weight:400; }
.sws-site .sws-astronomy-time { margin-top:22px; padding-top:16px; border-top:1px solid var(--border); }
.sws-site .sws-astronomy-time .sws-field-label { margin-bottom:6px; }
.sws-site .sws-astronomy-time .sws-clock-value strong { font-size:20px; line-height:1.5; font-weight:500; font-variant-numeric:tabular-nums; }
.sws-site .sws-astronomy-time #date_ut, .sws-site .sws-astronomy-time #time_ut { font-size:inherit; }
.sws-site .sws-astronomy-time .sws-sidereal-clock { margin-top:10px; font-size:14px; }
.sws-editor .sws-clock-action { margin:0; display:flex; justify-content:flex-end; }
.sws-editor .sws-clock-action button { width:auto; min-height:42px; margin:0; font-size:14px; white-space:normal; }
.sws-browser-clock { display:flex; justify-content:space-between; flex-wrap:wrap; gap:6px; margin:14px 0 0; font-size:12px; color:var(--muted); }
.sws-browser-clock strong { font-weight:400; }
.sws-park-toggle { display:flex; flex:1; min-width:0; }
.sws-park-toggle > button { width:100%; margin:0; }
.sws-library-section > .b1 { width:100%; }
@media(max-width:700px) {
  .sws-control-grid { grid-template-columns:1fr; grid-template-areas:'tracking' 'guide'; }
  .sws-site-limits { grid-template-columns:1fr; gap:14px; }
  .sws-mount .sws-editor { padding:16px; }
  .sws-editor-heading { gap:8px; }
  .sws-editor-heading h2 { font-size:18px; }
  .sws-mount .sws-edit-button { font-size:13px; padding:6px 10px; }
  .sws-editor .sws-unit-inputs { gap:4px; font-size:12px; }
  .sws-editor .sws-unit-inputs > select { width:48px; }
  .sws-editor .sws-unit-inputs input, .sws-editor select { font-size:16px; }
  .sws-editor .sws-site-unit input:not([hidden]) { padding-right:20px; padding-left:4px; }
  .sws-site-unit > span { right:5px; font-size:18px; }
  .sws-clock-value strong { font-size:17px; }
  .sws-clock-controls { align-items:flex-start; }
}
@media(max-width:360px) {
  .sws-clock-controls { flex-wrap:wrap; }
  .sws-clock-controls > form { width:100%; }
  .sws-clock-value strong { white-space:nowrap; }
  .sws-editor .sws-clock-action button { white-space:nowrap; }
}
</style>
)UI";

const char html_settings_editors[] PROGMEM = R"UI(
<script>
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
    ['g1','g2','g3','t1','t2','t3'].forEach((name, index) => {
      const input = site.elements[name];
      if (!input) return;
      const unit = input.nextSibling, label = document.createElement('label'), text = document.createElement('span');
      label.className = 'sws-site-unit';
      text.textContent = ['°', '′', '″'][index % 3];
      text.setAttribute('aria-hidden', 'true');
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
</script>
)UI";
