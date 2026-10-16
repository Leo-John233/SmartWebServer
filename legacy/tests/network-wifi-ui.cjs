// 运行方式 node legacy/tests/network-wifi-ui.cjs 输出目录
// 使用仓库 webui 已安装的 Playwright 验证真实页面脚本与手机布局
const fs = require('node:fs');
const path = require('node:path');
const assert = require('node:assert/strict');
const { chromium } = require('../../webui/node_modules/playwright');
const output = path.resolve(process.argv[2] || require('node:os').tmpdir());
const source = fs.readFileSync(path.join(__dirname, '../SmartWebServer/src/pages/network/NetworkWifi.h'), 'utf8');
const css = source.match(/html_network_css\[\].*?R"NET\(([\s\S]*?)\)NET"/)[1];
const ui = source.match(/html_network_wifi\[\].*?R"NET\(([\s\S]*?)\)NET"/)[1];
const html = `<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><style>body{background:#26262a}.t,.b{background:#111;border:1px solid #551111}.t a{display:inline-block;padding:8px;color:#bb9198;border:1px solid #844}h1{margin:0}</style>${css}<div class="t"><table><tr><td><h1>OnStepX</h1></td><td>Smart Web Server 2.08l</td></tr></table><nav><a>控制器</a><a>赤道仪</a><a>网络</a></nav></div><div class="b"><b>通信设置：</b><p>命令等待时间：<input style="width:4em" value="200"> ms</p><p>网页等待时间：<input style="width:4em" value="200"> ms</p><button>上传</button>${ui}<h3>修改管理密码</h3><label>密码：<input type="password"></label><button>上传</button><button>退出</button><p class="nw-note">外观预览 · Wi-Fi 数据为模拟值</p></div>`;
const state = {sta:{enabled:true,ssid:'Home',dhcp:true,ip:'192.168.1.55',gw:'192.168.1.1',sn:'255.255.255.0'},ap:{enabled:true,ssid:'ONSTEPX',channel:7,ip:'192.168.0.1',gw:'192.168.0.1',sn:'255.255.255.0'},connected:false,connecting:false,pending:false,failed:false,apActive:true,ssid:'',ip:'0.0.0.0',rssi:-55,apIp:'192.168.0.1',apSsid:'ONSTEPX',message:''};
const networks=[{ssid:'Home Wi-Fi',rssi:-43,open:false},{ssid:'Observatory',rssi:-58,open:false},{ssid:'Guest',rssi:-70,open:true},{ssid:'Home Wi-Fi',rssi:-80,open:false},{ssid:'<img src=x onerror=alert(1)>',rssi:-90,open:false}];
const previewMock=`<script>const mockState=${JSON.stringify(state)};window.fetch=async(url,opts)=>{const p=new URLSearchParams(opts&&opts.body||'');const action=p.get('action')||new URL(url,location.href).searchParams.get('action');let data=mockState;if(action==='scan')data={running:false,networks:${JSON.stringify(networks.filter(n=>!n.ssid.startsWith('<')))}};if(action==='connect'||action==='ap')data={message:'这是外观预览，未连接真实硬件'};return {ok:true,json:async()=>data};};</script>`;
fs.mkdirSync(output,{recursive:true});fs.writeFileSync(path.join(output,'network-preview.html'),html.replace('<script>',previewMock+'<script>'));
(async()=>{
const browser=await chromium.launch({channel:'msedge',headless:true});
try {
const page=await browser.newPage({viewport:{width:1440,height:1080}});const errors=[];page.on('pageerror',e=>errors.push(e.message));
let scans=0, submitted,includeHostile=true;
await page.route('http://sws.test/**',async route=>{const req=route.request(),url=new URL(req.url());if(url.pathname==='/')return route.fulfill({contentType:'text/html',body:html});const p=new URLSearchParams(req.postData()||''),action=p.get('action')||url.searchParams.get('action');let response=state;
if(action==='scan')response=++scans===1?{running:true}:{running:false,networks:includeHostile?networks:networks.filter(n=>!n.ssid.startsWith('<'))};
if(action==='connect'){submitted=p;response={message:'正在连接…'};state.connected=true;state.ssid=p.get('ssid');state.ip='192.168.1.55';state.message='连接成功';}
return route.fulfill({contentType:'application/json',body:JSON.stringify(response)});});
await page.goto('http://sws.test/');await page.waitForFunction(()=>!document.querySelector('#nw-scan').disabled);
await page.locator('#nw-scan').click();await page.waitForFunction(()=>document.querySelectorAll('#nw-results button').length===4);
assert.equal(await page.locator('#nw-results img').count(),0,'SSID must be rendered as text');
// 特殊名称仅用于安全测试，预览截图使用常见的网络名称
includeHostile=false;await page.locator('#nw-scan').click();await page.waitForFunction(()=>document.querySelectorAll('#nw-results button').length===3);

await page.getByRole('button',{name:/Home Wi-Fi/}).click();assert.equal(await page.locator('#nw-ssid').inputValue(),'Home Wi-Fi');
assert.equal(await page.getByRole('button',{name:/Home Wi-Fi/}).getAttribute('aria-pressed'),'true');assert.equal(await page.getByRole('button',{name:/Home Wi-Fi/}).locator('.nw-selected').innerText(),'✓');assert.equal(await page.getByRole('button',{name:/Home Wi-Fi/}).locator('.nw-network-meta').innerText(),'-43 dBm');
await page.screenshot({path:path.join(output,'network-desktop.png'),fullPage:true});
await page.locator('#nw-results').screenshot({path:path.join(output,'network-list.png')});
await page.locator('#nw-password').fill('test-only-pass');await page.locator('#nw-show').check();assert.equal(await page.locator('#nw-password').getAttribute('type'),'text');
await page.locator('#nw-show').uncheck();await page.getByRole('button',{name:'保存并连接'}).click();await page.waitForFunction(()=>document.querySelector('#nw-message').textContent.includes('正在连接'));
assert.equal(submitted.get('ssid'),'Home Wi-Fi');assert.equal(submitted.get('password'),'test-only-pass');assert.equal(submitted.get('dhcp'),'1');
await page.waitForFunction(()=>document.querySelector('#nw-sta-state').textContent.includes('已连接'));assert.equal(await page.locator('#nw-sta-ip').innerText(),'192.168.1.55');await page.locator('#nw-sta-state').screenshot({path:path.join(output,'network-status.png')});
await page.getByRole('button',{name:/Guest/}).click();assert.equal(await page.locator('#nw-password').isDisabled(),true);
await page.locator('#nw-sta input[name=open]').uncheck();await page.locator('#nw-sta input[name=dhcp]').uncheck();await page.locator('#nw-sta summary').click();assert.equal(await page.locator('#nw-static').isVisible(),true);
for(const width of [390,320]){await page.setViewportSize({width,height:844});assert.equal(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),true,`overflow at ${width}px`);}
await page.locator('#nw-sta summary').click();await page.getByRole('button',{name:/Home Wi-Fi/}).click();await page.setViewportSize({width:390,height:844});await page.screenshot({path:path.join(output,'network-mobile.png'),fullPage:true});
assert.equal((await page.locator('body').innerText()).includes('。'),false,'页面文案不使用句号');assert.deepEqual(errors,[]);console.log('PASS: async scan, deduplication, safe SSIDs, selection, password visibility, connection submission/status, open networks, static settings, 320/390px layout.');
}finally{await browser.close();}
})().catch(e=>{console.error(e);process.exit(1)});
