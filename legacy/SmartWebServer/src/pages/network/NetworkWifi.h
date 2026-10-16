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
/* 网络列表将名称放在左侧，锁和信号放在右侧，勾选只表示已经连接 */
.nw-networks{overflow:hidden;margin-top:12px;border:1px solid #463239;border-radius:14px;background:#211b1f}
#nw-results{max-height:360px;overflow:auto}.nw-networks:empty{display:none}
.nw-row{display:flex;align-items:center;gap:12px;width:100%;min-height:72px;text-align:left;background:transparent;border:0;border-bottom:1px solid #372b31;border-radius:0;margin:0;padding:14px 16px;font-weight:400}
.nw-row:last-child{border-bottom:0}.nw-row:hover{background:#30242b}
.nw-network-copy{display:flex;flex:1;min-width:0;flex-direction:column;gap:5px}.nw-network-name{font-size:16px;overflow-wrap:anywhere;color:#eee1e5}.nw-network-meta{font-size:12px;color:#ab979f}
.nw-wifi{width:25px;height:25px;flex-shrink:0;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}.nw-lock{width:14px;height:18px;flex-shrink:0;fill:none;stroke:currentColor;stroke-width:1.8}
.nw-selected{width:20px;color:#edb0c4;font-size:22px;flex-shrink:0;text-align:center}.nw-info{border:1.5px solid #b88b9c;border-radius:50%;width:19px;height:19px;text-align:center;line-height:19px;font-size:13px;color:#dbacc0;flex-shrink:0}
.nw-list-heading{display:flex;align-items:center;justify-content:space-between;margin-top:20px;gap:12px}.nw-list-heading strong{font-size:14px;color:#bba3ad}.nw-list-heading button{margin:0;padding:7px 12px;min-height:36px;font-size:13px}
.nw-switch-line{display:flex;justify-content:space-between;align-items:center;gap:16px;margin:18px 0}.nw-switch-line label{margin:0}.nw-switch{appearance:none;position:relative;width:44px!important;height:26px!important;border-radius:18px;padding:0;background:#51464b;cursor:pointer;margin:0!important;flex-shrink:0}.nw-switch:before{content:"";position:absolute;top:3px;left:3px;width:18px;height:18px;border-radius:50%;background:#fff;transition:transform .15s}.nw-switch:checked{background:#a64861;border-color:#a64861}.nw-switch:checked:before{transform:translateX(18px)}
/* 点选网络后弹出独立连接窗口，密码和高级设置不再常驻列表 */
#nw-dialog{box-sizing:border-box;width:calc(100% - 24px);max-width:440px;max-height:85dvh;overflow:auto;color:#d2cccc;background:#211b1f;border:1px solid #67434f;border-radius:18px;padding:22px}#nw-dialog::backdrop{background:#0009}
.nw-dialog-head{display:flex;align-items:center;justify-content:space-between;gap:12px}.nw-dialog-head h2{overflow-wrap:anywhere;font-size:20px;margin:0}.nw-dialog-head button{background:transparent;border:0;font-size:22px;padding:4px 10px}
.nw-password-line{display:flex;align-items:stretch;border:1px solid #725055;border-radius:9px;overflow:hidden;background:#241d20}.nw-card .nw-password-line input{min-width:0;border:0;border-radius:0;flex:1}.nw-password-line button{background:transparent;border:0;border-radius:0;padding:8px 12px;color:#e1b3c3;font-size:13px;white-space:nowrap}
/* 使用统一的显示按钮，避免浏览器自带的密码图标重复出现 */
#nw-password::-ms-reveal{display:none}
#nw-dialog .nw-actions button{flex:1}#nw-dialog-error{color:#ffb1b1;overflow-wrap:anywhere}#nw-dialog-error:empty{display:none}
[hidden]{display:none!important}
#nw-message{min-height:1.7em;line-height:1.7;overflow-wrap:anywhere}.nw-error{color:#ffb1b1}
@media(max-width:650px){body{margin:8px}.t,.b{padding:14px}.t{margin-top:8px}.t td{display:block;text-align:left}.t font{font-size:24px}.nw-grid{grid-template-columns:1fr}.nw-card{padding:16px}.nw-actions button{flex:1}input{max-width:100%}}
</style>
)NET";

#if OPERATIONAL_MODE == WIFI
const char html_network_wifi[] PROGMEM = R"NET(
<div class="nw-grid" id="nw-panel">
<section class="nw-card" id="nw-sta-card"><h2>Wi-Fi <small>STA</small></h2>
<p class="nw-note">连接路由器后，可通过同一网络访问设备</p>
<div class="nw-switch-line"><label for="nw-enabled">连接 Wi-Fi</label><input id="nw-enabled" class="nw-switch" type="checkbox" role="switch"></div>
<div id="nw-sta-state" aria-live="polite"><span id="nw-sta-badge" class="nw-note">读取中</span><div id="nw-current" class="nw-networks"></div></div>
<div id="nw-available"><div class="nw-list-heading"><strong>其他网络</strong><button type="button" id="nw-scan" class="nw-secondary">刷新</button></div>
<p id="nw-scan-state" class="nw-note" role="status"></p><div id="nw-results" class="nw-networks"></div>
<div class="nw-networks"><button type="button" id="nw-manual" class="nw-row"><span class="nw-network-copy">其他网络…<span class="nw-network-meta">手动添加或连接隐藏 Wi-Fi</span></span><span aria-hidden="true">＋</span></button></div></div>
<dialog id="nw-dialog" class="nw-card" aria-labelledby="nw-dialog-title">
<div class="nw-dialog-head"><h2 id="nw-dialog-title">连接 Wi-Fi</h2><button type="button" id="nw-close" aria-label="关闭">×</button></div>
<p id="nw-dialog-note" class="nw-note"></p>
<form id="nw-sta"><input name="enabled" type="checkbox" hidden><input name="open" type="checkbox" hidden>
<div id="nw-manual-fields"><label for="nw-ssid">Wi-Fi 名称</label><input id="nw-ssid" name="ssid" maxlength="31" autocomplete="off" required>
<label for="nw-security">密码要求</label><select id="nw-security"><option value="password">需要密码</option><option value="open">无需密码</option></select></div>
<div id="nw-password-fields"><label for="nw-password">密码</label><div class="nw-password-line"><input id="nw-password" name="password" type="password" maxlength="31" autocomplete="new-password"><button type="button" id="nw-show" aria-pressed="false">显示</button></div></div>
<p id="nw-open-note" class="nw-note" hidden>此网络无需密码，点击连接即可加入</p>
<details id="nw-advanced"><summary>网络设置</summary><dl id="nw-connection-details" class="nw-state-values" hidden><div><dt>IP 地址</dt><dd id="nw-sta-ip">—</dd></div><div><dt>信号</dt><dd id="nw-sta-signal">—</dd></div></dl>
<div class="nw-switch-line"><label for="nw-dhcp">自动获取 IP</label><input id="nw-dhcp" class="nw-switch" name="dhcp" type="checkbox" role="switch"></div><div id="nw-static">
<label>静态 IP<input name="ip" inputmode="decimal" placeholder="192.168.1.55"></label>
<label>网关<input name="gw" inputmode="decimal" placeholder="192.168.1.1"></label>
<label>子网掩码<input name="sn" inputmode="decimal" placeholder="255.255.255.0"></label></div></details>
<p id="nw-dialog-error" role="alert"></p><div class="nw-actions"><button type="button" id="nw-cancel" class="nw-secondary">取消</button><button id="nw-connect" type="submit">连接</button></div></form></dialog></section>
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
const $=id=>document.getElementById(id),sta=$('nw-sta'),ap=$('nw-ap'),dialog=$('nw-dialog');
let loaded=false,scanning=false,working=false,submitting=false,scanTries=0,lastNotice='',statusBusy=false,current=null,networks=[],autoScanned=false,listKey='',enableDraft=false;
function message(text,error=false){$('nw-message').textContent=text;$('nw-message').className=error?'nw-error':'';}
function sync(){
$('nw-static').hidden=sta.elements.dhcp.checked;$('nw-password').disabled=sta.elements.open.checked;$('nw-password-fields').hidden=sta.elements.open.checked;$('nw-open-note').hidden=!sta.elements.open.checked;
$('nw-available').hidden=!$('nw-enabled').checked;
$('nw-scan').disabled=!loaded||scanning||working||!$('nw-enabled').checked;
$('nw-enabled').disabled=!loaded||scanning||working;
$('nw-manual').disabled=!loaded||scanning||working;
$('nw-connect').disabled=!loaded||scanning||working;
document.querySelectorAll('.nw-row').forEach(b=>b.disabled=!loaded||working||scanning);
ap.querySelector('[type=submit]').disabled=!loaded||working;
}
function lock(value){working=value||submitting;sync();}
// 请求设置超时，断网时停止等待，避免不断堆积连接
async function api(action,params){
const controller=new AbortController(),timer=setTimeout(()=>controller.abort(),8000);
try{const options={cache:'no-store',signal:controller.signal};
if(params){options.method='POST';options.headers={'Content-Type':'application/x-www-form-urlencoded'};options.body=new URLSearchParams({...params,action});}
const response=await fetch('/net-wifi?'+new URLSearchParams({action}),options),data=await response.json();
if(!response.ok)throw Error(data.error||'操作失败');return data;
}finally{clearTimeout(timer);}}
function fields(form,data){Object.keys(data).forEach(k=>{const e=form.elements[k];if(!e)return;if(e.type==='checkbox')e.checked=!!data[k];else e.value=data[k];});}
// 行内图标使用固定内容，网络名称只通过纯文本写入
function row(n,connected){const b=document.createElement('button');b.type='button';b.className='nw-row';b.setAttribute('aria-label',n.ssid+(connected?' 已连接 网络详情':' '+n.rssi+' dBm '+(n.open?'无需密码':'需要密码')));
b.innerHTML='<span class="nw-network-copy"><span class="nw-network-name"></span><span class="nw-network-meta"></span></span>'+(n.open?'':'<svg class="nw-lock" viewBox="0 0 16 20" aria-hidden="true"><rect x="3" y="8" width="10" height="9" rx="2"/><path d="M5 8V5a3 3 0 0 1 6 0v3"/></svg>')+'<svg class="nw-wifi" viewBox="0 0 32 32" aria-hidden="true"><path d="M3 10a19 19 0 0 1 26 0M7 15a13 13 0 0 1 18 0M11 20a7 7 0 0 1 10 0"/><circle cx="16" cy="25" r="1.6" fill="currentColor" stroke="none"/></svg>'+(connected?'<span class="nw-selected" aria-hidden="true">✓</span>':'<span class="nw-info" aria-hidden="true">i</span>');
b.querySelector('.nw-network-name').textContent=n.ssid;b.querySelector('.nw-network-meta').textContent=n.rssi+' dBm';b.onclick=()=>openNetwork(n,false);return b;}
// 列表不变时保留节点，避免轮询反复打断用户选择
function renderNetworks(){if(!current)return;const key=JSON.stringify([current.connected,current.ssid,current.rssi,current.sta.open,networks]);if(key===listKey)return;listKey=key;
$('nw-current').replaceChildren();if(current.connected)$('nw-current').appendChild(row({ssid:current.ssid,rssi:current.rssi,open:current.sta.open},true));
$('nw-results').replaceChildren();networks.filter(n=>!current.connected||n.ssid!==current.ssid).forEach(n=>$('nw-results').appendChild(row(n,false)));}
// 弹窗按所选网络初始化，取消不会改变连接，密码也不会留到下次打开
function openNetwork(n,manual){if(!loaded||working||scanning)return;fields(sta,current.sta);sta.elements.enabled.checked=true;
const same=!manual&&n.ssid===current.sta.ssid,connected=!manual&&current.connected&&n.ssid===current.ssid;
sta.elements.ssid.value=manual?'':n.ssid;sta.elements.open.checked=manual?false:n.open;if(!same)sta.elements.dhcp.checked=true;
$('nw-manual-fields').hidden=!manual;$('nw-ssid').readOnly=!manual;$('nw-security').value=sta.elements.open.checked?'open':'password';
$('nw-dialog-title').textContent=manual?'添加网络':n.ssid;$('nw-dialog-note').textContent=connected?'已连接，可查看或修改网络设置':manual?'输入网络名称并选择密码要求':n.rssi+' dBm';
$('nw-password').value='';$('nw-password').type='password';$('nw-password').placeholder=same?'留空使用已保存的密码':'输入 Wi-Fi 密码';$('nw-password').required=!(same&&!current.sta.open)&&!sta.elements.open.checked;$('nw-password').minLength=8;
$('nw-show').textContent='显示';$('nw-show').setAttribute('aria-pressed','false');$('nw-dialog-error').textContent='';$('nw-advanced').open=connected;$('nw-connection-details').hidden=!connected;$('nw-connect').textContent=connected?'保存设置':'连接';sync();dialog.showModal();
if(manual)$('nw-ssid').focus();else if(!sta.elements.open.checked&&!connected)$('nw-password').focus();}
dialog.addEventListener('close',()=>{$('nw-password').value='';$('nw-password').type='password';});
$('nw-close').onclick=$('nw-cancel').onclick=()=>dialog.close();
// 轮询只更新设备状态，不覆盖弹窗中正在编辑的内容
async function status(){if(statusBusy)return;statusBusy=true;try{const s=await api('status');current=s;if(!loaded){fields(ap,s.ap);loaded=true;}
if(!submitting)$('nw-enabled').checked=s.sta.enabled||enableDraft;
$('nw-sta-badge').textContent=s.connected?'已连接':s.connecting?'正在连接 '+s.sta.ssid:s.sta.enabled||enableDraft?'尚未连接':'Wi-Fi 已关闭';
$('nw-sta-ip').textContent=s.connected?s.ip:'未分配';$('nw-sta-signal').textContent=s.connected?s.rssi+' dBm':'—';
$('nw-ap-badge').textContent=s.apActive?'已开启':'未开启';$('nw-ap-badge').dataset.connected=String(s.apActive);$('nw-ap-name').textContent=s.apSsid;$('nw-ap-ip').textContent=s.apActive?s.apIp:'—';
renderNetworks();if(s.message&&s.message!==lastNotice){lastNotice=s.message;message(s.message,s.failed);}lock(s.connecting||s.pending);
if(!autoScanned&&s.sta.enabled&&!working){autoScanned=true;startScan();}
}catch(e){lastNotice='';message('无法连接设备，请重新连接热点，或使用新的 IP 地址',true);}finally{statusBusy=false;}}
async function scan(){try{const r=await api('scan',{});if(r.running){if(++scanTries>30)throw Error('扫描超时，请稍后重试');setTimeout(scan,1000);return;}
const seen=new Set();networks=r.networks.sort((a,b)=>b.rssi-a.rssi).filter(n=>{if(!n.ssid||seen.has(n.ssid))return false;seen.add(n.ssid);return true;});renderNetworks();$('nw-scan-state').textContent=networks.length?'点选网络即可连接':'未发现网络，可刷新或手动添加';
}catch(e){$('nw-scan-state').textContent=e.message;}scanning=false;sync();}
// 进入已开启 Wi-Fi 的页面和打开开关时自动搜索，也保留手动刷新入口
function startScan(){if(scanning||working||!loaded)return;scanning=true;scanTries=0;$('nw-scan-state').textContent='正在搜索附近的 Wi-Fi';sync();scan();}
$('nw-scan').onclick=startScan;$('nw-manual').onclick=()=>openNetwork(null,true);
$('nw-security').onchange=()=>{sta.elements.open.checked=$('nw-security').value==='open';$('nw-password').required=!sta.elements.open.checked;$('nw-password').value='';sync();};
sta.addEventListener('change',sync);$('nw-show').onclick=()=>{const show=$('nw-password').type==='password';$('nw-password').type=show?'text':'password';$('nw-show').textContent=show?'隐藏':'显示';$('nw-show').setAttribute('aria-pressed',String(show));};
// 关闭连接要确认，开启时先展示列表，选好网络后才发送连接请求
$('nw-enabled').onchange=async()=>{enableDraft=$('nw-enabled').checked;if(enableDraft){startScan();return;}
if(!current.sta.enabled){sync();return;}if(!confirm('关闭 Wi-Fi 后将断开路由器，请通过设备热点访问')){$('nw-enabled').checked=true;return;}
submitting=true;lock(true);try{const p={...current.sta,enabled:'0',open:current.sta.open?'1':'0',dhcp:current.sta.dhcp?'1':'0',password:''};const r=await api('connect',p);message(r.message);}catch(e){$('nw-enabled').checked=true;message(e.message,true);}finally{submitting=false;await status();}};
for(const [form,action] of [[sta,'connect'],[ap,'ap']])form.onsubmit=async e=>{e.preventDefault();if(working||scanning)return;
if(action==='ap'&&!confirm('修改后可能需要重新连接热点，请记住新的热点名称和 IP'))return;
const p=Object.fromEntries(new FormData(form));['enabled','dhcp','open'].forEach(k=>{if(form.elements[k])p[k]=form.elements[k].checked?'1':'0';});submitting=true;lock(true);$('nw-dialog-error').textContent='';
try{const r=await api(action,p);message(r.message);form.elements.password.value='';if(action==='connect'){enableDraft=false;dialog.close();}}
catch(err){if(action==='connect')$('nw-dialog-error').textContent=err.message;else message(err.message,true);}
finally{submitting=false;lock(false);await status();}};
sync();status();setInterval(status,2500);

})();
</script>
)NET";
#endif
