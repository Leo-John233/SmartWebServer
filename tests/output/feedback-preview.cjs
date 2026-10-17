const output = require('../test-output.cjs');
const path = require('node:path');
const {chromium} = require('../../../webui/node_modules/playwright');
const templates = require('../page-templates.cjs')(path.join(__dirname, '../../SmartWebServer'));
const html = `<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">${templates['network/NetworkWifi.h'].html_network_css}${templates['Appearance.h'].html_shared_appearance}</head><body><div class="b">${require('../network-page.cjs')({fragment:true})}</div></body></html>`;
let state = {sta:{enabled:true,ssid:'Redmi_Hotel',open:false,dhcp:true,ip:'192.168.31.229',gw:'192.168.31.1',sn:'255.255.255.0'},ap:{enabled:true,ssid:'ONSTEPX',channel:7,ip:'192.168.0.1',gw:'192.168.0.1',sn:'255.255.255.0'},connected:false,connecting:false,pending:false,failed:true,apActive:true,ssid:'',ip:'0.0.0.0',rssi:0,apIp:'192.168.0.1',apSsid:'ONSTEPX',messageTarget:'sta',message:'没有连上 Wi-Fi，请检查密码或靠近路由器后重试 设备热点仍可用于重新配网'};
const networks = [{ssid:'Redmi_Hotel',rssi:-76,open:false},{ssid:'ChinaNet-DgEf',rssi:-81,open:false},{ssid:'ChinaNet-Ed2q',rssi:-93,open:false}];
(async()=>{
 const browser = await chromium.launch({channel:'msedge',headless:true});
 try {
  const page = await browser.newPage({viewport:{width:1280,height:1120},deviceScaleFactor:1.5});
  const errors=[];page.on('pageerror',e=>errors.push(e.message));
  await page.route('http://feedback-preview.test/**',async route=>{
   const url=new URL(route.request().url());
   if(url.pathname==='/')return route.fulfill({contentType:'text/html',body:html});
   const action=url.searchParams.get('action')||new URLSearchParams(route.request().postData()||'').get('action');
   return route.fulfill({contentType:'application/json',body:JSON.stringify(action==='scan'?{running:false,networks}:state)});
  });
  await page.goto('http://feedback-preview.test/');
  await page.locator('#nw-message').waitFor({state:'visible'});
  await page.waitForFunction(()=>!document.querySelector('#nw-scan').disabled);
  await page.locator('#nw-sta-card').screenshot({path:path.join(output,'feedback-sta-error.png')});
  state={...state,connected:true,failed:false,ssid:'Redmi_Hotel',ip:'192.168.31.229',rssi:-76,messageTarget:'ap',message:'热点设置已保存'};
  await page.reload();
  await page.locator('#nw-ap-message').waitFor({state:'visible'});
  await page.locator('section.nw-card').filter({has:page.locator('#nw-ap')}).screenshot({path:path.join(output,'feedback-ap-saved.png')});
  if(errors.length)throw new Error(errors.join('\n'));
  console.log('Generated feedback-sta-error.png and feedback-ap-saved.png');
 } finally {await browser.close();}
})().catch(e=>{console.error(e);process.exit(1)});
