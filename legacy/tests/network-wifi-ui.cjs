// 运行方式 node legacy/tests/network-wifi-ui.cjs 输出目录
// 使用仓库 webui 已安装的 Playwright 验证真实页面脚本与手机布局
const fs = require('node:fs');
const path = require('node:path');
const assert = require('node:assert/strict');
const { chromium } = require('../../webui/node_modules/playwright');
const output = path.resolve(process.argv[2] || require('node:os').tmpdir());
const templates = require('./page-templates.cjs')(path.join(__dirname, '../SmartWebServer'));
const shared = templates['Appearance.h'].html_shared_appearance;
const css = templates['network/NetworkWifi.h'].html_network_css;
const ui = templates['network/NetworkWifi.h'].html_network_wifi;
const html = `<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><style>body{background:#26262a}.t,.b{background:#111;border:1px solid #551111}.t a{display:inline-block;padding:8px;color:#bb9198;border:1px solid #844}h1{margin:0}</style>${css}${shared}<div class="t"><table><tr><td><h1>OnStepX</h1></td><td>Smart Web Server 2.10h</td></tr></table><nav><a>控制器</a><a>赤道仪</a><a>网络</a></nav></div><div class="b"><b>通信设置：</b><p>命令等待时间：<input style="width:4em" value="200"> ms</p><p>网页等待时间：<input style="width:4em" value="200"> ms</p><button>上传</button>${ui}<div class="b1" style="text-align:left;width:33em;min-height:10em">无线更新固件<br><button>重启并更新固件</button></div><div class="b1" style="text-align:left;width:33em;min-height:10em">通信设置<br><input style="width:4em" value="200"> ms</div><h3>修改管理密码</h3><label>密码：<input type="password"></label><button>上传</button><button>退出</button><p class="nw-note">外观预览 · Wi-Fi 数据为模拟值</p></div>`;
const state = {sta:{enabled:true,ssid:'Home Wi-Fi',open:false,dhcp:true,ip:'192.168.1.55',gw:'192.168.1.1',sn:'255.255.255.0'},ap:{enabled:true,ssid:'ONSTEPX',channel:7,ip:'192.168.0.1',gw:'192.168.0.1',sn:'255.255.255.0'},connected:true,connecting:false,pending:false,failed:false,apActive:true,ssid:'Home Wi-Fi',ip:'192.168.1.55',rssi:-55,apIp:'192.168.0.1',apSsid:'ONSTEPX',message:''};
const networks=[{ssid:'Home Wi-Fi',rssi:-43,open:false},{ssid:'Observatory',rssi:-58,open:false},{ssid:'Guest',rssi:-70,open:true},{ssid:'Home Wi-Fi',rssi:-80,open:false},{ssid:'<img src=x onerror=alert(1)>',rssi:-90,open:false}];
const previewMock=`<script>const mockState=${JSON.stringify(state)};window.fetch=async(url,opts)=>{const p=new URLSearchParams(opts&&opts.body||'');const action=p.get('action')||new URL(url,location.href).searchParams.get('action');let data=mockState;if(action==='scan')data={running:false,networks:${JSON.stringify(networks.filter(n=>!n.ssid.startsWith('<')))}};if(action==='connect'||action==='ap')data={message:'这是外观预览，未连接真实硬件'};return {ok:true,json:async()=>data};};</script>`;
fs.mkdirSync(output,{recursive:true});fs.writeFileSync(path.join(output,'network-preview.html'),html.replace('<script>',previewMock+'<script>'));
(async()=>{
const browser=await chromium.launch({channel:'msedge',headless:true});
try {
const page=await browser.newPage({viewport:{width:1440,height:1080}});const errors=[];page.on('pageerror',e=>errors.push(e.message));
let scans=0,submitted,submissions=0,includeHostile=true,rejectConnect=false,scanFailure=false;
await page.route('http://sws.test/**',async route=>{const req=route.request(),url=new URL(req.url());if(url.pathname==='/')return route.fulfill({contentType:'text/html',body:html});const p=new URLSearchParams(req.postData()||''),action=p.get('action')||url.searchParams.get('action');let response=state;
if(action==='scan'){scans++;if(scanFailure)return route.fulfill({status:503,contentType:'application/json',body:JSON.stringify({error:'搜索失败，请重试'})});response=scans===1?{running:true}:{running:false,networks:includeHostile?networks:networks.filter(n=>!n.ssid.startsWith('<'))};}
if(action==='connect'){submissions++;submitted=p;if(rejectConnect)return route.fulfill({status:400,contentType:'application/json',body:JSON.stringify({error:'密码错误，请重试'})});response={message:'正在连接'};state.sta={...state.sta,enabled:p.get('enabled')==='1',ssid:p.get('ssid'),open:p.get('open')==='1',dhcp:p.get('dhcp')==='1'};state.connected=state.sta.enabled;state.ssid=p.get('ssid');state.ip='192.168.1.55';state.message=state.connected?'连接成功':'已断开路由器';}
return route.fulfill({contentType:'application/json',body:JSON.stringify(response)});});
await page.goto('http://sws.test/');await page.waitForFunction(()=>document.querySelectorAll('#nw-results button').length===3&&!document.querySelector('#nw-scan').disabled);
// 无需打开详情就能看到实时访问地址
assert.equal(await page.locator('#nw-access').isVisible(),true);assert.equal(await page.locator('#nw-access-ip').innerText(),'192.168.1.55');
assert(scans>=2,'进入页面自动搜索并等待扫描完成');assert.equal(await page.locator('#nw-results img').count(),0,'SSID must be rendered as text');
assert.equal(await page.locator('#nw-current .nw-selected').innerText(),'✓');assert.equal(await page.locator('#nw-current .nw-network-meta').innerText(),'-55 dBm');assert.equal(await page.locator('#nw-results .nw-selected').count(),0);
// 刷新使用真实扫描流程，预览截图去除用于安全测试的特殊名称
includeHostile=false;const scansBefore=scans;await page.locator('#nw-scan').click();await page.waitForFunction(()=>document.querySelectorAll('#nw-results button').length===2);assert(scans>scansBefore);
await page.screenshot({path:path.join(output,'network-desktop.png'),fullPage:true});await page.locator('#nw-sta-card').screenshot({path:path.join(output,'network-list.png')});
await page.locator('#nw-current button').click();assert.equal(await page.locator('#nw-sta-ip').innerText(),'192.168.1.55');assert.equal(await page.locator('#nw-connect').innerText(),'保存设置');await page.locator('#nw-cancel').click();assert.equal(submissions,0);
// 点选只打开密码窗口，取消后保持原连接且清除密码
await page.getByRole('button',{name:/Observatory/}).click();assert.equal(await page.locator('#nw-dialog').isVisible(),true);assert.equal(await page.locator('#nw-ssid').inputValue(),'Observatory');assert.equal(submissions,0);
await page.locator('#nw-password').fill('test-only-pass');await page.locator('#nw-show').click();assert.equal(await page.locator('#nw-password').getAttribute('type'),'text');await page.locator('#nw-show').click();assert.equal(await page.locator('#nw-password').getAttribute('type'),'password');
await page.keyboard.press('Escape');assert.equal(await page.locator('#nw-dialog').isVisible(),false);await page.waitForFunction(()=>document.querySelector('#nw-password').value==='');assert.equal(submissions,0);
await page.getByRole('button',{name:/Observatory/}).click();await page.locator('#nw-password').fill('test-only-pass');
// 轮询不会覆盖正在输入的密码，错误显示在弹窗中以便直接重试
await page.waitForTimeout(2800);assert.equal(await page.locator('#nw-password').inputValue(),'test-only-pass');rejectConnect=true;await page.locator('#nw-connect').click();await page.waitForFunction(()=>document.querySelector('#nw-dialog-error').textContent.includes('密码错误'));assert.equal(await page.locator('#nw-dialog').isVisible(),true);assert.equal(await page.locator('#nw-password').inputValue(),'test-only-pass');
rejectConnect=false;await page.locator('#nw-connect').click();await page.waitForFunction(()=>!document.querySelector('#nw-dialog').open&&document.querySelector('#nw-current').textContent.includes('Observatory'));assert.equal(submitted.get('ssid'),'Observatory');assert.equal(submitted.get('password'),'test-only-pass');assert.equal(submitted.get('dhcp'),'1');assert.equal(await page.locator('#nw-current .nw-selected').innerText(),'✓');
// 开放网络直接显示确认提示，不出现密码框或容易误解的复选框
await page.getByRole('button',{name:/Guest/}).click();assert.equal(await page.locator('#nw-password-fields').isVisible(),false);assert.equal(await page.locator('#nw-open-note').isVisible(),true);await page.locator('#nw-connect').click();await page.waitForFunction(()=>!document.querySelector('#nw-dialog').open);assert.equal(submitted.get('open'),'1');assert.equal(submitted.has('password'),false);
await page.locator('#nw-manual').click();assert.equal(await page.locator('#nw-security').isVisible(),true);await page.locator('#nw-ssid').fill('Hidden Wi-Fi');await page.locator('#nw-security').selectOption('open');assert.equal(await page.locator('#nw-password-fields').isVisible(),false);await page.locator('#nw-security').selectOption('password');assert.equal(await page.locator('#nw-password-fields').isVisible(),true);
await page.locator('#nw-advanced summary').click();await page.locator('#nw-dhcp').uncheck();assert.equal(await page.locator('#nw-static').isVisible(),true);
for(const width of [390,320]){await page.setViewportSize({width,height:844});assert.equal(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth&&document.querySelector('#nw-dialog').scrollWidth<=document.querySelector('#nw-dialog').clientWidth),true,'手机弹窗不横向溢出');}
await page.locator('#nw-cancel').click();
// 关闭需要确认，重新打开自动扫描，状态轮询不会把尚未连接的开关关掉
page.once('dialog',d=>d.dismiss());await page.locator('#nw-enabled').click();assert.equal(await page.locator('#nw-enabled').isChecked(),true);
page.once('dialog',d=>d.accept());await page.locator('#nw-enabled').uncheck();await page.waitForFunction(()=>!document.querySelector('#nw-enabled').disabled&&!document.querySelector('#nw-enabled').checked);assert.equal(submitted.get('enabled'),'0');assert.equal(await page.locator('#nw-access').isVisible(),false);
const beforeEnable=scans;await page.locator('#nw-enabled').check();await page.waitForFunction(()=>!document.querySelector('#nw-scan').disabled);assert(scans>beforeEnable,'打开开关自动搜索');await page.waitForTimeout(2800);assert.equal(await page.locator('#nw-enabled').isChecked(),true);
scanFailure=true;await page.locator('#nw-scan').click();await page.waitForFunction(()=>document.querySelector('#nw-scan-state').textContent.includes('搜索失败'));assert.equal(await page.locator('#nw-scan').isDisabled(),false);scanFailure=false;await page.locator('#nw-scan').click();await page.waitForFunction(()=>!document.querySelector('#nw-scan').disabled);
// 使用已连接状态生成手机预览，弹窗单独截图
state.connected=true;state.sta.enabled=true;state.sta.ssid=state.ssid='Home Wi-Fi';state.sta.open=false;state.message='';await page.waitForFunction(()=>document.querySelector('#nw-current').textContent.includes('Home Wi-Fi'));
await page.setViewportSize({width:390,height:844});await page.screenshot({path:path.join(output,'network-mobile.png'),fullPage:true});await page.locator('#nw-sta-card').screenshot({path:path.join(output,'network-list-mobile.png')});
await page.getByRole('button',{name:/Observatory/}).click();await page.locator('#nw-password').fill('example-password');await page.screenshot({path:path.join(output,'network-password-mobile.png')});
assert.equal((await page.locator('body').innerText()).includes('。'),false,'页面文案不使用句号');assert.deepEqual(errors,[]);console.log('PASS: auto/manual scan, current-network checkmark, safe SSIDs, modal cancel/retry, password toggle, open/hidden networks, static IP, enable/disable, scan recovery, mobile layout.');

}finally{await browser.close();}
})().catch(e=>{console.error(e);process.exit(1)});
