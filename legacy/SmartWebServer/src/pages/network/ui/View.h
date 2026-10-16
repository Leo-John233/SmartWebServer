#pragma once

// 表单的 name 和 id 对应原有配网接口。
const char html_network_wifi[] PROGMEM = R"NET(
<div class="nw-grid" id="nw-panel">
<section class="nw-card" id="nw-sta-card"><h2>Wi-Fi <small>STA</small></h2>
<p class="nw-note">)NET"
 L_UI_STATION_NOTE
 R"NET(</p>
<div class="nw-switch-line"><label for="nw-enabled">)NET"
 L_UI_WIFI_CONNECT
 R"NET(</label><input id="nw-enabled" class="nw-switch" type="checkbox" role="switch"></div>
<div id="nw-sta-state" aria-live="polite"><span id="nw-sta-badge" class="nw-note">)NET"
 L_UI_LOADING
 R"NET(</span><div id="nw-current" class="nw-networks"></div><dl id="nw-access" class="nw-access" hidden><dt>)NET"
 L_UI_ACCESS_ADDRESS
 R"NET(</dt><dd id="nw-access-ip">—</dd></dl></div>
<div id="nw-available"><div class="nw-list-heading"><strong>)NET"
 L_UI_OTHER_NETWORKS
 R"NET(</strong><button type="button" id="nw-scan" class="nw-secondary">)NET"
 L_UI_REFRESH
 R"NET(</button></div>
<p id="nw-scan-state" class="nw-note" role="status"></p><div id="nw-results" class="nw-networks"></div>
<div class="nw-networks"><button type="button" id="nw-manual" class="nw-row"><span class="nw-network-copy">)NET"
 L_UI_OTHER_NETWORK
 R"NET(<span class="nw-network-meta">)NET"
 L_UI_HIDDEN_NETWORK
 R"NET(</span></span><span aria-hidden="true">＋</span></button></div></div>
<dialog id="nw-dialog" class="nw-card" aria-labelledby="nw-dialog-title">
<div class="nw-dialog-head"><h2 id="nw-dialog-title">)NET"
 L_UI_WIFI_CONNECT
 R"NET(</h2><button type="button" id="nw-close" aria-label=")NET"
 L_UI_CLOSE
 R"NET(">×</button></div>
<p id="nw-dialog-note" class="nw-note"></p>
<form id="nw-sta"><input name="enabled" type="checkbox" hidden><input name="open" type="checkbox" hidden>
<div id="nw-manual-fields"><label for="nw-ssid">)NET"
 L_UI_SSID
 R"NET(</label><input id="nw-ssid" name="ssid" maxlength="31" autocomplete="off" required>
<label for="nw-security">)NET"
 L_UI_SECURITY
 R"NET(</label><select id="nw-security"><option value="password">)NET"
 L_UI_SECURED
 R"NET(</option><option value="open">)NET"
 L_UI_OPEN
 R"NET(</option></select></div>
<div id="nw-password-fields"><label for="nw-password">)NET"
 L_UI_PASSWORD
 R"NET(</label><div class="nw-password-line"><input id="nw-password" name="password" type="password" maxlength="31" autocomplete="new-password"><button type="button" id="nw-show" aria-pressed="false">)NET"
 L_UI_SHOW
 R"NET(</button></div></div>
<p id="nw-open-note" class="nw-note" hidden>)NET"
 L_UI_OPEN_NOTE
 R"NET(</p>
<details id="nw-advanced"><summary>)NET"
 L_UI_NETWORK_SETTINGS
 R"NET(</summary><dl id="nw-connection-details" class="nw-state-values" hidden><div><dt>)NET"
 L_UI_IP_ADDRESS
 R"NET(</dt><dd id="nw-sta-ip">—</dd></div><div><dt>)NET"
 L_UI_SIGNAL
 R"NET(</dt><dd id="nw-sta-signal">—</dd></div></dl>
<div class="nw-switch-line"><label for="nw-dhcp">)NET"
 L_UI_DHCP
 R"NET(</label><input id="nw-dhcp" class="nw-switch" name="dhcp" type="checkbox" role="switch"></div><div id="nw-static">
<label>)NET"
 L_UI_STATIC_IP
 R"NET(<input name="ip" inputmode="decimal" placeholder="192.168.1.55"></label>
<label>)NET"
 L_UI_GATEWAY
 R"NET(<input name="gw" inputmode="decimal" placeholder="192.168.1.1"></label>
<label>)NET"
 L_UI_SUBNET
 R"NET(<input name="sn" inputmode="decimal" placeholder="255.255.255.0"></label></div></details>
<p id="nw-dialog-error" role="alert"></p><div class="nw-actions"><button type="button" id="nw-cancel" class="nw-secondary">)NET"
 L_UI_CANCEL
 R"NET(</button><button id="nw-connect" type="submit">)NET"
 L_UI_CONNECT
 R"NET(</button></div></form></dialog></section>
<section class="nw-card"><h2>)NET"
 L_UI_AP_TITLE
 R"NET(<small>AP</small></h2>
<p class="nw-note">)NET"
 L_UI_AP_NOTE
 R"NET(</p>
<div class="nw-state" id="nw-ap-state" aria-live="polite">
<div class="nw-state-top"><span>)NET"
 L_UI_DEVICE_AP
 R"NET(</span><span class="nw-badge" id="nw-ap-badge">)NET"
 L_UI_LOADING
 R"NET(</span></div>
<strong class="nw-state-name" id="nw-ap-name">—</strong>
<dl class="nw-state-values"><div><dt>)NET"
 L_UI_ACCESS_ADDRESS
 R"NET(</dt><dd id="nw-ap-ip">—</dd></div></dl></div>
<form id="nw-ap"><label class="nw-toggle"><input name="enabled" type="checkbox">)NET"
 L_UI_AP_ENABLE
 R"NET(</label>
<label>)NET"
 L_UI_AP_NAME
 R"NET(<input name="ssid" maxlength="31" required autocomplete="off"></label>
<label>)NET"
 L_UI_AP_PASSWORD
 R"NET(<input name="password" type="password" minlength="8" maxlength="31" autocomplete="new-password" placeholder=")NET"
 L_UI_KEEP_PASSWORD
 R"NET("></label>
<details><summary>)NET"
 L_UI_AP_ADVANCED
 R"NET(</summary><label>)NET"
 L_UI_CHANNEL
 R"NET(<input name="channel" type="number" min="1" max="11" required></label>
<label>)NET"
 L_UI_AP_IP
 R"NET(<input name="ip" inputmode="decimal" required></label>
<label>)NET"
 L_UI_GATEWAY
 R"NET(<input name="gw" inputmode="decimal" required></label>
<label>)NET"
 L_UI_SUBNET
 R"NET(<input name="sn" inputmode="decimal" required></label></details>
<p class="nw-note">)NET"
 L_UI_AP_RECOVERY_NOTE
 R"NET(<br>)NET"
 L_UI_AP_SCAN_NOTE
 R"NET(</p>
<div class="nw-actions"><button type="submit">)NET"
 L_UI_AP_SAVE
 R"NET(</button></div></form></section>
</div><p id="nw-message" role="status" aria-live="polite"></p>
)NET";
