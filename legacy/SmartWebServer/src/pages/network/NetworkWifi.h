#pragma once

#include "../Pages.common.h"

#if OPERATIONAL_MODE == WIFI
void renderWifiNetwork(String &data);
void handleWifiNetwork();
void pollWifiNetwork();
#endif

// 网络页使用独立的响应式样式，不加载外部字体和脚本
// 新版通信与固件更新卡片限制在屏幕宽度内，避免手机页面横向滚动
const char html_network_css[] PROGMEM = R"NET(
<style>
body{font-family:system-ui,-apple-system,"Segoe UI",sans-serif;font-size:15px;color:#d2cccc}
.t,.b{box-sizing:border-box;min-width:0;max-width:1100px;margin-left:auto;margin-right:auto;border-width:1px;padding:20px}
.t{border-radius:16px 16px 0 0;margin-top:20px}.b{border-radius:0 0 16px 16px}
.b1{box-sizing:border-box;max-width:100%;margin:12px 0;padding:16px;border-radius:14px;float:none}
.t table{width:100%}.t td{overflow-wrap:anywhere}.t a{border-radius:8px;margin:8px 4px 0 0}
input,select{box-sizing:border-box;color:#eee;background:#241d20;border:1px solid #725055;border-radius:8px;padding:10px;font:inherit;max-width:100%;text-align:left}
button{color:#f7eeee;background:#842d39;border:1px solid #a34855;border-radius:8px;padding:10px 16px;font:inherit;cursor:pointer;min-height:44px}
button:disabled{color:#9b9393;background:#30282b;cursor:wait}button:focus-visible,input:focus-visible,summary:focus-visible{outline:2px solid #e9a0ab;outline-offset:3px}
.nw-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:18px;margin:22px 0}
.nw-card{border:1px solid #59343c;background:#1b171a;border-radius:14px;padding:20px;min-width:0}
.nw-card h2{margin:0 0 8px;font-size:20px}.nw-note{color:#b5a5aa;font-size:13px;line-height:1.7;margin:8px 0 16px}
.nw-card label{display:block;margin:14px 0 6px}.nw-card input:not([type=checkbox]),.nw-card select{display:block;width:100%;margin:0}
.nw-card input[type=checkbox]{width:20px;height:20px;vertical-align:middle;accent-color:#ac4254;margin:0 8px 0 0}
.nw-card .nw-toggle{display:flex;align-items:center;font-weight:600}.nw-card details{margin:16px 0;padding:12px;border:1px solid #493138;border-radius:9px}
.nw-card summary{cursor:pointer}.nw-actions{display:flex;flex-wrap:wrap;gap:8px;margin-top:16px}.nw-secondary{background:#2e252a}
.nw-state{padding:16px;background:#251e22;border:1px solid #423039;border-radius:12px;overflow-wrap:anywhere}
.nw-state-top{display:flex;align-items:center;justify-content:space-between;gap:12px;color:#ad98a2;font-size:12px}.nw-badge{padding:4px 9px;border-radius:6px;background:#352b30;color:#c6b7be;font-size:12px;white-space:nowrap}.nw-badge[data-connected=true]{background:#4c303c;color:#f1c4d0}
.nw-state-name{display:block;font-size:18px;font-weight:600;color:#efe1e7;margin:14px 0 16px}.nw-state-values{display:flex;gap:28px;flex-wrap:wrap;margin:0}.nw-state-values div{min-width:0}.nw-state-values dt{color:#a28d98;font-size:12px;margin-bottom:5px}.nw-state-values dd{margin:0;color:#d9cbd2;font-size:14px;font-variant-numeric:tabular-nums}
#nw-results{max-height:330px;overflow:auto;margin-top:12px;border:1px solid #463239;border-radius:12px;background:#211b1f}#nw-results:empty{display:none}
#nw-results button{display:flex;align-items:center;gap:14px;width:100%;min-height:66px;text-align:left;background:transparent;border:0;border-bottom:1px solid #372b31;border-radius:0;margin:0;padding:10px 16px;font-weight:400}
#nw-results button:last-child{border-bottom:0}#nw-results button:hover{background:#30242b}#nw-results button[aria-pressed=true]{background:#30232a;box-shadow:inset 3px 0 #ce819b}
.nw-network-copy{display:flex;flex:1;min-width:0;flex-direction:column;gap:5px}.nw-network-name{font-size:16px;font-weight:400;overflow-wrap:anywhere;color:#eee1e5}.nw-network-meta{font-size:12px;color:#ab979f}
.nw-wifi{width:34px;height:34px;flex-shrink:0;fill:none;stroke:#cdbbc3;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}
.nw-selected{width:20px;color:#edb0c4;font-size:20px;text-align:right;flex-shrink:0}
.nw-list-heading{display:flex;align-items:center;justify-content:space-between;margin-top:20px;gap:12px}.nw-list-heading strong{font-size:14px;color:#bba3ad}.nw-list-heading button{margin:0;padding:7px 12px;min-height:36px;font-size:13px}
#nw-message{min-height:1.7em;line-height:1.7;overflow-wrap:anywhere}.nw-error{color:#ffb1b1}
@media(max-width:650px){body{margin:8px}.t,.b{padding:14px}.t{margin-top:8px}.t td{display:block;text-align:left}.t font{font-size:24px}.nw-grid{grid-template-columns:1fr}.nw-card{padding:16px}.nw-actions button{flex:1}input{max-width:100%}}
</style>
)NET";

#if OPERATIONAL_MODE == WIFI
const char html_network_wifi[] PROGMEM = R"NET(
<div class="nw-grid" id="nw-panel">
<section class="nw-card"><h2>连接 Wi-Fi <small>STA</small></h2>
<p class="nw-note">连接家中或现场的 Wi-Fi 后，同一网络内的手机和电脑都能访问设备</p>
<div class="nw-state" id="nw-sta-state" aria-live="polite">
<div class="nw-state-top"><span>路由器连接</span><span class="nw-badge" id="nw-sta-badge">读取中</span></div>
<strong class="nw-state-name" id="nw-sta-name">—</strong>
<dl class="nw-state-values"><div><dt>IP 地址</dt><dd id="nw-sta-ip">—</dd></div><div><dt>信号</dt><dd id="nw-sta-signal">—</dd></div></dl></div>
<form id="nw-sta"><label class="nw-toggle"><input name="enabled" type="checkbox">连接路由器</label>
<div class="nw-list-heading"><strong>附近的 Wi-Fi</strong><button type="button" id="nw-scan" class="nw-secondary">搜索 Wi-Fi</button></div>
<div id="nw-scan-state" class="nw-note" role="status"></div><div id="nw-results"></div>
<label for="nw-ssid">Wi-Fi 名称</label><input id="nw-ssid" name="ssid" maxlength="31" autocomplete="off" required>
<label for="nw-password">Wi-Fi 密码</label><input id="nw-password" name="password" type="password" maxlength="31" autocomplete="new-password" placeholder="不换网络可留空，换网络请填写密码">
<label><input name="open" type="checkbox">这个 Wi-Fi 不需要密码</label>
<label><input id="nw-show" type="checkbox">显示密码</label>
<label class="nw-toggle"><input name="dhcp" type="checkbox">自动获取 IP（DHCP）</label>
<details><summary>高级网络设置</summary><div id="nw-static">
<label>静态 IP<input name="ip" inputmode="decimal" placeholder="192.168.1.55"></label>
<label>网关<input name="gw" inputmode="decimal" placeholder="192.168.1.1"></label>
<label>子网掩码<input name="sn" inputmode="decimal" placeholder="255.255.255.0"></label></div>
<p class="nw-note">自动获取 IP 时无需填写地址，隐藏 Wi-Fi 可直接输入名称</p></details>
<div class="nw-actions"><button type="submit">保存并连接</button></div></form></section>
<section class="nw-card"><h2>设备热点 <small>AP</small></h2>
<p class="nw-note">没有路由器时，手机也能直接连接设备热点</p>
<div class="nw-state" id="nw-ap-state" aria-live="polite">
<div class="nw-state-top"><span>设备热点</span><span class="nw-badge" id="nw-ap-badge">读取中</span></div>
<strong class="nw-state-name" id="nw-ap-name">—</strong>
<dl class="nw-state-values"><div><dt>访问地址</dt><dd id="nw-ap-ip">—</dd></div></dl></div>
<form id="nw-ap"><label class="nw-toggle"><input name="enabled" type="checkbox">启用设备热点</label>
<label>热点名称<input name="ssid" maxlength="31" required autocomplete="off"></label>
<label>热点密码<input name="password" type="password" minlength="8" maxlength="31" autocomplete="new-password" placeholder="留空保留当前密码"></label>
<details><summary>高级热点设置</summary><label>信道<input name="channel" type="number" min="1" max="11" required></label>
<label>热点 IP<input name="ip" inputmode="decimal" required></label>
<label>网关<input name="gw" inputmode="decimal" required></label>
<label>子网掩码<input name="sn" inputmode="decimal" required></label></details>
<p class="nw-note">配网时会保留设备热点，连上路由器后可手动关闭<br>搜索或连接 Wi-Fi 时，手机可能短暂断线，重新连接热点即可</p>
<div class="nw-actions"><button type="submit">保存热点设置</button></div></form></section>
</div><p id="nw-message" role="status" aria-live="polite"></p>
<script>
(()=>{
const $=id=>document.getElementById(id),sta=$('nw-sta'),ap=$('nw-ap');
let loaded=false,scanning=false,working=false,scanTries=0,lastNotice='',statusBusy=false;
function message(text,error=false){$('nw-message').textContent=text;$('nw-message').className=error?'nw-error':'';}
function sync(){ $('nw-static').hidden=sta.elements.dhcp.checked; $('nw-password').disabled=sta.elements.open.checked; $('nw-scan').disabled=!loaded||scanning||working||!sta.elements.enabled.checked; }
function lock(value){working=value;document.querySelectorAll('#nw-panel button[type=submit]').forEach(b=>b.disabled=value||!loaded);sync();}
// 请求设置超时，断网时停止等待，避免不断堆积连接
async function api(action,params){
const controller=new AbortController(),timer=setTimeout(()=>controller.abort(),8000);
try{const options={cache:'no-store',signal:controller.signal};
if(params){options.method='POST';options.headers={'Content-Type':'application/x-www-form-urlencoded'};options.body=new URLSearchParams({...params,action});}
const response=await fetch('/net-wifi?'+new URLSearchParams({action}),options),data=await response.json();
if(!response.ok)throw Error(data.error||'操作失败');return data;
}finally{clearTimeout(timer);}}

function fields(form,data){Object.keys(data).forEach(k=>{const e=form.elements[k];if(!e)return;if(e.type==='checkbox')e.checked=!!data[k];else e.value=data[k];});}
// 轮询只更新连接状态，不覆盖用户正在填写的名称和密码
async function status(){if(statusBusy)return;statusBusy=true;try{const s=await api('status');if(!loaded){fields(sta,s.sta);fields(ap,s.ap);loaded=true;}
// 将状态、网络名称和访问地址分开显示，避免一行文字挤在一起
$('nw-sta-badge').textContent=s.connected?'已连接':s.connecting?'连接中':s.sta.enabled?'未连接':'未开启';
$('nw-sta-badge').dataset.connected=String(s.connected);
$('nw-sta-name').textContent=s.connected?s.ssid:s.sta.ssid||'尚未选择 Wi-Fi';
$('nw-sta-ip').textContent=s.connected?s.ip:'未分配';$('nw-sta-signal').textContent=s.connected?s.rssi+' dBm':'—';
$('nw-ap-badge').textContent=s.apActive?'已开启':'未开启';$('nw-ap-badge').dataset.connected=String(s.apActive);
$('nw-ap-name').textContent=s.apSsid;$('nw-ap-ip').textContent=s.apActive?s.apIp:'—';
if(s.message&&s.message!==lastNotice){lastNotice=s.message;message(s.message,s.failed);}lock(s.connecting||s.pending);
}catch(e){lastNotice='';message('无法连接设备，请重新连接热点，或使用新的 IP 地址',true);}finally{statusBusy=false;} }
async function scan(){try{const r=await api('scan',{});if(r.running){if(++scanTries>30)throw Error('扫描超时，请稍后重试');setTimeout(scan,1000);return;}
$('nw-results').replaceChildren();const seen=new Set();r.networks.sort((a,b)=>b.rssi-a.rssi).forEach(n=>{if(!n.ssid||seen.has(n.ssid))return;seen.add(n.ssid);const b=document.createElement('button');b.type='button';b.setAttribute('aria-pressed','false');b.title=n.rssi+' dBm';
// 左侧只显示无线图标，加密网络在图标右下角加锁
b.innerHTML='<svg class="nw-wifi" viewBox="0 0 36 36" aria-hidden="true"><path d="M3 11a21 21 0 0 1 30 0M7 16a15 15 0 0 1 22 0M11 21a9 9 0 0 1 14 0"/><circle cx="18" cy="26" r="1.7" fill="currentColor" stroke="none"/>'+(n.open?'':'<rect x="25" y="23" width="9" height="9" rx="1.4" fill="#211b1f"/><path d="M27 23v-3a2.5 2.5 0 0 1 5 0v3"/>')+'</svg><span class="nw-network-copy"><span class="nw-network-name"></span><span class="nw-network-meta"></span></span><span class="nw-selected" aria-hidden="true"></span>';
// 网络名称只写入纯文本，避免把特殊名称当成网页代码
// 点选新网络时默认自动获取 IP，用户仍可在高级设置中改成固定地址
b.querySelector('.nw-network-name').textContent=n.ssid;b.querySelector('.nw-network-meta').textContent=n.rssi+' dBm';
b.onclick=()=>{document.querySelectorAll('#nw-results button').forEach(row=>{row.setAttribute('aria-pressed','false');row.querySelector('.nw-selected').textContent='';});b.setAttribute('aria-pressed','true');b.querySelector('.nw-selected').textContent='✓';sta.elements.ssid.value=n.ssid;sta.elements.password.value='';sta.elements.open.checked=n.open;sta.elements.enabled.checked=true;sta.elements.dhcp.checked=true;sync();if(!n.open)$('nw-password').focus();};$('nw-results').appendChild(b);});$('nw-scan-state').textContent=seen.size?'找到 '+seen.size+' 个网络，请点选后填写密码':'未发现网络，可重试或手动输入名称';
}catch(e){$('nw-scan-state').textContent=e.message;}scanning=false;sync();}
// 用户打开路由器连接开关时自动搜索，也可以手动再次搜索
function startScan(){scanning=true;scanTries=0;$('nw-scan-state').textContent='扫描中，请稍候…';sync();scan();}
$('nw-scan').onclick=startScan;sta.elements.enabled.onchange=()=>{if(sta.elements.enabled.checked&&loaded&&!working&&!scanning)startScan();};
$('nw-ssid').oninput=()=>{document.querySelectorAll('#nw-results button').forEach(row=>{row.setAttribute('aria-pressed','false');row.querySelector('.nw-selected').textContent='';});};
sta.addEventListener('change',sync);$('nw-show').onchange=()=>{$('nw-password').type=$('nw-show').checked?'text':'password';};
for(const [form,action] of [[sta,'connect'],[ap,'ap']])form.onsubmit=async e=>{e.preventDefault();if(scanning)return message('请等待扫描完成',true);if(action==='ap'&&!confirm('修改后可能需要重新连接热点，请记住新的热点名称和 IP'))return;
const p=Object.fromEntries(new FormData(form));['enabled','dhcp','open'].forEach(k=>{if(form.elements[k])p[k]=form.elements[k].checked?'1':'0';});lock(true);try{const r=await api(action,p);message(r.message);form.elements.password.value='';}catch(err){message(err.message,true);lock(false);} };
document.querySelectorAll('#nw-panel button').forEach(b=>b.disabled=true);status();setInterval(status,2500);
})();
</script>
)NET";
#endif
