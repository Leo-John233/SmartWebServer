// 验证真实辅助页的四类通道、原有接口、滑块转换和窄屏布局
const fs=require('node:fs'),path=require('node:path'),assert=require('node:assert/strict');
const {chromium}=require('../../webui/node_modules/playwright');
const preview=require('./auxiliary-page.cjs')(),device=require('../demo/auxiliary.cjs')(preview);
const original=require('./auxiliary-page.cjs')({sourceRef:'193a81809f5b19b3afa2a8c58f18291c2f0b0475'});
const entries=text=>Object.fromEntries(text.trim().split('\n').map(line=>{const split=line.indexOf('|');return [line.slice(0,split),line.slice(split+1).trim()];}));
assert.deepEqual(entries(device.poll()),entries(preview.ajax),'模拟初始状态与实际 C++ 输出一致');
const command=values=>device.command(new URLSearchParams(values));
assert(command({x8v1:'1'})===false,'禁用通道不能操作');
assert(command({x3v2:'41'})===false,'除露范围限制');
assert(command({x4v3:'9'})===false,'拍摄间隔范围限制');
assert(command({x2v1:'256'})===false,'输出范围限制');
assert(command({x1v1:'0',x2v1:'255',x3v2:'4',x3v3:'8',x4v2:'86',x4v3:'26',x4v4:'20'}));
let state=entries(device.poll());assert.equal(state.sw1_off,'selected');assert.equal(state.x2v1,'100');assert.equal(state.x3v2,'2.0');assert.equal(state.x3v3,'4.0');assert.equal(state.x4v2,'30');assert.equal(state.x4v3,'5.00');assert.equal(state.x4v4,'20');
const appearance=require('./page-templates.cjs')(path.join(__dirname,'../SmartWebServer'))['Appearance.h'].html_shared_appearance;
const out=process.argv[2]||require('node:os').tmpdir();fs.mkdirSync(out,{recursive:true});
(async()=>{const browser=await chromium.launch({channel:'msedge',headless:true});try{
 const page=await browser.newPage({viewport:{width:1440,height:1100}}),errors=[],requests=[];page.on('pageerror',e=>errors.push(e.stack));
 await page.route('http://auxiliary.test/**',route=>{const u=new URL(route.request().url());if(u.pathname==='/auxiliary-ajax-get.txt'){requests.push(Object.fromEntries(u.searchParams));return route.fulfill({status:device.command(u.searchParams)?200:400,body:''});}if(u.pathname==='/auxiliary-ajax.txt')return route.fulfill({body:device.poll()});return route.fulfill({contentType:'text/html; charset=utf-8',body:'<!doctype html><head><meta charset="utf-8">'+appearance+'</head><body>'+preview.body+'</body>'});});
 await page.goto('http://auxiliary.test/auxiliary.htm');const cards=page.locator('.sws-aux-grid>.b1');assert.equal(await cards.count(),4);
 // 同一组设备数据分别运行原始和当前页面，排除空白及布局分隔符后文字必须一致
 const texts=await page.evaluate(({before,after})=>{const normalize=s=>s.replace(/[\s@:]/g,'');const values=html=>[...new DOMParser().parseFromString(html,'text/html').querySelectorAll('.b1')].map(e=>({text:normalize(e.textContent),controls:[...e.querySelectorAll('button,input')].map(c=>Object.fromEntries(['id','type','min','max','value','onchange','onpointerdown'].map(a=>[a,c.getAttribute(a)])))}));return {before:values(before),after:values(after)};},{before:original.body,after:preview.body});
 assert.deepEqual(texts.after,texts.before,'文字、按钮、初始读数、取值范围和请求参数与原始页面一致');
 assert.equal(await cards.nth(0).locator('button').count(),2);assert.equal(await cards.nth(0).locator('input').count(),0);
 assert.equal(await cards.nth(1).locator('button').count(),0);assert.equal(await cards.nth(1).locator('input[type=range]').count(),1);
 assert.equal(await cards.nth(2).locator('button').count(),2);assert.equal(await cards.nth(2).locator('input[type=range]').count(),2);
 assert.equal(await cards.nth(3).locator('button').count(),2);assert.equal(await cards.nth(3).locator('input[type=range]').count(),3);
 await page.locator('#sw1_on').click();await page.locator('#sw3_off').click();await cards.nth(3).locator('button').first().click();
 for(const [card,index,value]of [[1,0,'64'],[2,0,'6'],[2,1,'10'],[3,0,'12'],[3,1,'96'],[3,2,'30']]){const input=cards.nth(card).locator('input[type=range]').nth(index);const inputName=(await input.getAttribute('onchange')).match(/sz\('([^']+)'/)[1];const sent=page.waitForResponse(response=>{const url=new URL(response.url());return url.pathname==='/auxiliary-ajax-get.txt'&&url.searchParams.get(inputName)===value;});
 await input.fill(value);await input.dispatchEvent('change');await sent;}
 await page.waitForFunction(()=>['x2v1','x3v2','x4v4','x4v2','x4v3'].every((id,i)=>document.getElementById(id).textContent===['25','3.0','12','40','6.00'][i])).catch(async error=>{
   console.error({errors, requests, values: await page.evaluate(()=>Object.fromEntries(['x2v1','x3v2','x4v4','x4v2','x4v3'].map(id=>[id,document.getElementById(id).textContent])))});
   throw error;
 });
 for(const [key,value]of Object.entries({x1v1:'1',x3v1:'0',x4v1:'1',x2v1:'64',x3v2:'6',x3v3:'10',x4v4:'12',x4v2:'96',x4v3:'30'}))assert(requests.some(r=>r[key]===value),key+' 使用原有请求参数');
 assert.equal(await page.locator('#x3v2').innerText(),'3.0');assert.equal(await page.locator('#x4v4').innerText(),'12');assert.equal(await page.locator('#x4v2').innerText(),'40');assert.equal(await page.locator('#x4v3').innerText(),'6.00');
 for(const width of [1440,390,320]){await page.setViewportSize({width,height:1100});assert(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),'页面无横向溢出 '+width);assert(await cards.evaluateAll(nodes=>nodes.every(e=>e.scrollWidth<=e.clientWidth+1)),'通道卡片无横向溢出 '+width);await page.screenshot({path:path.join(out,'auxiliary-complete-'+width+'.png'),fullPage:true});}
 assert.deepEqual(errors,[]);console.log('PASS: auxiliary C++/demo polling parity, disabled channels, four independent feature types, request parameters, conversions and 1440/390/320 layouts');
 }finally{await browser.close();}})().catch(e=>{console.error(e);process.exit(1)});
