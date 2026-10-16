#pragma once
#include "../../ui/Text.h"

// 浏览器扫描、连接与状态显示；只调用 NetworkWifi.cpp 提供的接口。
const char html_network_wifi_script[] PROGMEM = R"NET(
<script>
(() => {
  const $ = (id) => document.getElementById(id),
    sta = $('nw-sta'),
    ap = $('nw-ap'),
    dialog = $('nw-dialog');
  let loaded = false,
    scanning = false,
    working = false,
    submitting = false,
    scanTries = 0,
    lastNotice = '',
    statusBusy = false,
    current = null,
    networks = [],
    autoScanned = false,
    listKey = '',
    enableDraft = false;
  function message(text, error = false) {
    $('nw-message').textContent = text;
    $('nw-message').className = error ? 'nw-error' : '';
  }
  function sync() {
    $('nw-static').hidden = sta.elements.dhcp.checked;
    $('nw-password').disabled = sta.elements.open.checked;
    $('nw-password-fields').hidden = sta.elements.open.checked;
    $('nw-open-note').hidden = !sta.elements.open.checked;
    $('nw-available').hidden = !$('nw-enabled').checked;
    $('nw-scan').disabled = !loaded || scanning || working || !$('nw-enabled').checked;
    $('nw-enabled').disabled = !loaded || scanning || working;
    $('nw-manual').disabled = !loaded || scanning || working;
    $('nw-connect').disabled = !loaded || scanning || working;
    document
      .querySelectorAll('.nw-row')
      .forEach((b) => (b.disabled = !loaded || working || scanning));
    ap.querySelector('[type=submit]').disabled = !loaded || working;
  }
  function lock(value) {
    working = value || submitting;
    sync();
  }
  // 请求设置超时，断网时停止等待，避免不断堆积连接
  async function api(action, params) {
    const controller = new AbortController(),
      timer = setTimeout(() => controller.abort(), 8000);
    try {
      const options = { cache: 'no-store', signal: controller.signal };
      if (params) {
        options.method = 'POST';
        options.headers = { 'Content-Type': 'application/x-www-form-urlencoded' };
        options.body = new URLSearchParams({ ...params, action });
      }
      const response = await fetch('/net-wifi?' + new URLSearchParams({ action }), options),
        data = await response.json();
      if (!response.ok) throw Error(data.error || )NET" SWS_JS_TEXT(L_UI_FAILED) R"NET();
      return data;
    } finally {
      clearTimeout(timer);
    }
  }
  function fields(form, data) {
    Object.keys(data).forEach((k) => {
      const e = form.elements[k];
      if (!e) return;
      if (e.type === 'checkbox') e.checked = !!data[k];
      else e.value = data[k];
    });
  }
  // 行内图标使用固定内容，网络名称只通过纯文本写入
  function row(n, connected) {
    const b = document.createElement('button');
    b.type = 'button';
    b.className = 'nw-row';
    b.setAttribute(
      'aria-label',
      n.ssid +
        (connected
          ? )NET" SWS_JS_TEXT(L_UI_CONNECTED_DETAILS) R"NET(
          : ' ' + n.rssi + ' dBm ' + (n.open ? )NET" SWS_JS_TEXT(L_UI_OPEN) R"NET( : )NET" SWS_JS_TEXT(L_UI_SECURED) R"NET()),
    );
    b.innerHTML =
      '<span class="nw-network-copy"><span class="nw-network-name"></span><span class="nw-network-meta"></span></span>' +
      (n.open
        ? ''
        : '<svg class="nw-lock" viewBox="0 0 16 20" aria-hidden="true"><rect x="3" y="8" width="10" height="9" rx="2"/><path d="M5 8V5a3 3 0 0 1 6 0v3"/></svg>') +
      '<svg class="nw-wifi" viewBox="0 0 32 32" aria-hidden="true"><path d="M3 10a19 19 0 0 1 26 0M7 15a13 13 0 0 1 18 0M11 20a7 7 0 0 1 10 0"/><circle cx="16" cy="25" r="1.6" fill="currentColor" stroke="none"/></svg>' +
      (connected
        ? '<span class="nw-selected" aria-hidden="true">✓</span>'
        : '<span class="nw-info" aria-hidden="true">i</span>');
    b.querySelector('.nw-network-name').textContent = n.ssid;
    b.querySelector('.nw-network-meta').textContent = n.rssi + ' dBm';
    b.onclick = () => openNetwork(n, false);
    return b;
  }
  // 列表不变时保留节点，避免轮询反复打断用户选择
  function renderNetworks() {
    if (!current) return;
    const key = JSON.stringify([
      current.connected,
      current.ssid,
      current.rssi,
      current.sta.open,
      networks,
    ]);
    if (key === listKey) return;
    listKey = key;
    $('nw-current').replaceChildren();
    if (current.connected)
      $('nw-current').appendChild(
        row({ ssid: current.ssid, rssi: current.rssi, open: current.sta.open }, true),
      );
    $('nw-results').replaceChildren();
    networks
      .filter((n) => !current.connected || n.ssid !== current.ssid)
      .forEach((n) => $('nw-results').appendChild(row(n, false)));
  }
  // 弹窗按所选网络初始化，取消不会改变连接，密码也不会留到下次打开
  function openNetwork(n, manual) {
    if (!loaded || working || scanning) return;
    fields(sta, current.sta);
    sta.elements.enabled.checked = true;
    const same = !manual && n.ssid === current.sta.ssid,
      connected = !manual && current.connected && n.ssid === current.ssid;
    sta.elements.ssid.value = manual ? '' : n.ssid;
    sta.elements.open.checked = manual ? false : n.open;
    if (!same) sta.elements.dhcp.checked = true;
    $('nw-manual-fields').hidden = !manual;
    $('nw-ssid').readOnly = !manual;
    $('nw-security').value = sta.elements.open.checked ? 'open' : 'password';
    $('nw-dialog-title').textContent = manual ? )NET" SWS_JS_TEXT(L_UI_ADD_NETWORK) R"NET( : n.ssid;
    $('nw-dialog-note').textContent = connected
      ? )NET" SWS_JS_TEXT(L_UI_CONNECTED_NOTE) R"NET(
      : manual
        ? )NET" SWS_JS_TEXT(L_UI_MANUAL_NOTE) R"NET(
        : n.rssi + ' dBm';
    $('nw-password').value = '';
    $('nw-password').type = 'password';
    $('nw-password').placeholder = same ? )NET" SWS_JS_TEXT(L_UI_SAVED_PASSWORD) R"NET( : )NET" SWS_JS_TEXT(L_UI_PASSWORD_PROMPT) R"NET(;
    $('nw-password').required = !(same && !current.sta.open) && !sta.elements.open.checked;
    $('nw-password').minLength = 8;
    $('nw-show').textContent = )NET" SWS_JS_TEXT(L_UI_SHOW) R"NET(;
    $('nw-show').setAttribute('aria-pressed', 'false');
    $('nw-dialog-error').textContent = '';
    $('nw-advanced').open = connected;
    $('nw-connection-details').hidden = !connected;
    $('nw-connect').textContent = connected ? )NET" SWS_JS_TEXT(L_UI_SAVE) R"NET( : )NET" SWS_JS_TEXT(L_UI_CONNECT) R"NET(;
    sync();
    dialog.showModal();
    if (manual) $('nw-ssid').focus();
    else if (!sta.elements.open.checked && !connected) $('nw-password').focus();
  }
  dialog.addEventListener('close', () => {
    $('nw-password').value = '';
    $('nw-password').type = 'password';
  });
  $('nw-close').onclick = $('nw-cancel').onclick = () => dialog.close();
  // 轮询只更新设备状态，不覆盖弹窗中正在编辑的内容
  async function status() {
    if (statusBusy) return;
    statusBusy = true;
    try {
      const s = await api('status');
      current = s;
      if (!loaded) {
        fields(ap, s.ap);
        loaded = true;
      }
      if (!submitting) $('nw-enabled').checked = s.sta.enabled || enableDraft;
      $('nw-sta-badge').textContent = s.connected
        ? )NET" SWS_JS_TEXT(L_UI_CONNECTED) R"NET(
        : s.connecting
          ? )NET" SWS_JS_TEXT(L_UI_CONNECTING) R"NET( + s.sta.ssid
          : s.sta.enabled || enableDraft
            ? )NET" SWS_JS_TEXT(L_UI_NOT_CONNECTED) R"NET(
            : )NET" SWS_JS_TEXT(L_UI_WIFI_OFF) R"NET(;
      // 使用实时连接地址，避免把配置中的静态地址当作实际访问地址
      $('nw-access').hidden = !s.connected;
      $('nw-access-ip').textContent = s.connected ? s.ip : '—';
      $('nw-sta-ip').textContent = s.connected ? s.ip : )NET" SWS_JS_TEXT(L_UI_NO_IP) R"NET(;
      $('nw-sta-signal').textContent = s.connected ? s.rssi + ' dBm' : '—';
      $('nw-ap-badge').textContent = s.apActive ? )NET" SWS_JS_TEXT(L_UI_ENABLED) R"NET( : )NET" SWS_JS_TEXT(L_UI_DISABLED) R"NET(;
      $('nw-ap-badge').dataset.connected = String(s.apActive);
      $('nw-ap-name').textContent = s.apSsid;
      $('nw-ap-ip').textContent = s.apActive ? s.apIp : '—';
      renderNetworks();
      if (s.message && s.message !== lastNotice) {
        lastNotice = s.message;
        message(s.message, s.failed);
      }
      lock(s.connecting || s.pending);
      if (!autoScanned && s.sta.enabled && !working) {
        autoScanned = true;
        startScan();
      }
    } catch (e) {
      lastNotice = '';
      message()NET" SWS_JS_TEXT(L_UI_CONNECTION_LOST) R"NET(, true);
    } finally {
      statusBusy = false;
    }
  }
  async function scan() {
    try {
      const r = await api('scan', {});
      if (r.running) {
        if (++scanTries > 30) throw Error()NET" SWS_JS_TEXT(L_UI_SCAN_TIMEOUT) R"NET();
        setTimeout(scan, 1000);
        return;
      }
      const seen = new Set();
      networks = r.networks
        .sort((a, b) => b.rssi - a.rssi)
        .filter((n) => {
          if (!n.ssid || seen.has(n.ssid)) return false;
          seen.add(n.ssid);
          return true;
        });
      renderNetworks();
      $('nw-scan-state').textContent = networks.length ? )NET" SWS_JS_TEXT(L_UI_PICK_NETWORK) R"NET( : )NET" SWS_JS_TEXT(L_UI_NO_NETWORKS) R"NET(;
    } catch (e) {
      $('nw-scan-state').textContent = e.message;
    }
    scanning = false;
    sync();
  }
  // 进入已开启 Wi-Fi 的页面和打开开关时自动搜索，也保留手动刷新入口
  function startScan() {
    if (scanning || working || !loaded) return;
    scanning = true;
    scanTries = 0;
    $('nw-scan-state').textContent = )NET" SWS_JS_TEXT(L_UI_SEARCHING) R"NET(;
    sync();
    scan();
  }
  $('nw-scan').onclick = startScan;
  $('nw-manual').onclick = () => openNetwork(null, true);
  $('nw-security').onchange = () => {
    sta.elements.open.checked = $('nw-security').value === 'open';
    $('nw-password').required = !sta.elements.open.checked;
    $('nw-password').value = '';
    sync();
  };
  sta.addEventListener('change', sync);
  $('nw-show').onclick = () => {
    const show = $('nw-password').type === 'password';
    $('nw-password').type = show ? 'text' : 'password';
    $('nw-show').textContent = show ? )NET" SWS_JS_TEXT(L_UI_HIDE) R"NET( : )NET" SWS_JS_TEXT(L_UI_SHOW) R"NET(;
    $('nw-show').setAttribute('aria-pressed', String(show));
  };
  // 关闭连接要确认，开启时先展示列表，选好网络后才发送连接请求
  $('nw-enabled').onchange = async () => {
    enableDraft = $('nw-enabled').checked;
    if (enableDraft) {
      startScan();
      return;
    }
    if (!current.sta.enabled) {
      sync();
      return;
    }
    if (!confirm()NET" SWS_JS_TEXT(L_UI_DISCONNECT_CONFIRM) R"NET()) {
      $('nw-enabled').checked = true;
      return;
    }
    submitting = true;
    lock(true);
    try {
      const p = {
        ...current.sta,
        enabled: '0',
        open: current.sta.open ? '1' : '0',
        dhcp: current.sta.dhcp ? '1' : '0',
        password: '',
      };
      const r = await api('connect', p);
      message(r.message);
    } catch (e) {
      $('nw-enabled').checked = true;
      message(e.message, true);
    } finally {
      submitting = false;
      await status();
    }
  };
  for (const [form, action] of [
    [sta, 'connect'],
    [ap, 'ap'],
  ])
    form.onsubmit = async (e) => {
      e.preventDefault();
      if (working || scanning) return;
      if (action === 'ap' && !confirm()NET" SWS_JS_TEXT(L_UI_AP_CONFIRM) R"NET()) return;
      const p = Object.fromEntries(new FormData(form));
      ['enabled', 'dhcp', 'open'].forEach((k) => {
        if (form.elements[k]) p[k] = form.elements[k].checked ? '1' : '0';
      });
      submitting = true;
      lock(true);
      $('nw-dialog-error').textContent = '';
      try {
        const r = await api(action, p);
        message(r.message);
        form.elements.password.value = '';
        if (action === 'connect') {
          enableDraft = false;
          dialog.close();
        }
      } catch (err) {
        if (action === 'connect') $('nw-dialog-error').textContent = err.message;
        else message(err.message, true);
      } finally {
        submitting = false;
        lock(false);
        await status();
      }
    };
  sync();
  status();
  setInterval(status, 2500);
})();
</script>
)NET";