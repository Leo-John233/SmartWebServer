const output=require('./test-output.cjs');
const path=require('node:path'),assert=require('node:assert/strict');
const {chromium}=require('../../webui/node_modules/playwright');
const root=path.join(__dirname,'../SmartWebServer');
const templates=require('./page-templates.cjs')(root);
const card=require('./board-io-backend.cjs')({preview:true});
const html='<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width"></head><body>'+templates['Appearance.h'].html_shared_appearance+'<main class="sws-controller"><div class="b1" style="margin:16px;padding:20px">'+card+'</div></main></body></html>';
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try{
  const page=await browser.newPage({viewport:{width:1440,height:1000}}),errors=[],posts=[];
  let state={activity:true,protection:true,message:'驱动反馈正常 · 已释放'},fail=false,confirmation=true,dialogs=0;
  page.on('pageerror',e=>errors.push(e.message));
  page.on('dialog',async dialog=>{dialogs++;if(confirmation)await dialog.accept();else await dialog.dismiss();});
  await page.route('http://board.test/**',async route=>{
   const request=route.request(),url=new URL(request.url());
   if(url.pathname==='/board-io'){
    if(request.method()==='POST'){
     const values=Object.fromEntries(new URLSearchParams(request.postData()));posts.push(values);
     if(fail)return route.fulfill({status:500,body:'{}',contentType:'application/json'});
     state[values.setting]=values.enabled==='1';state.message=state.protection?'等待驱动反馈 · 已锁定':'保护关闭 · 已释放';
    }
    return route.fulfill({body:JSON.stringify(state),contentType:'application/json'});
   }
   return route.fulfill({body:html,contentType:'text/html; charset=utf-8'});
  });
  await page.goto('http://board.test/');
  const led=page.locator('#sws-io-activity'),brake=page.locator('#sws-io-protection');
  const toggle=await led.evaluate(e=>{const r=e.getBoundingClientRect(),k=getComputedStyle(e,'::after');return{width:r.width,height:r.height,knob:k.width,color:k.backgroundColor};});
  assert.deepEqual(toggle,{width:44,height:26,knob:'18px',color:'rgb(255, 255, 255)'},'开关与 Wi-Fi 开关尺寸、白色圆钮一致');
  await led.click();await page.waitForFunction(()=>document.getElementById('sws-io-activity').getAttribute('aria-checked')==='false');
  assert.deepEqual(posts[0],{setting:'activity',enabled:'0'});
  confirmation=false;await brake.click();assert.equal(posts.length,1,'取消手动释放时不发送请求');
  confirmation=true;await brake.click();await page.waitForFunction(()=>document.getElementById('sws-io-protection').getAttribute('aria-checked')==='false');
  assert.equal(await page.locator('#sws-io-brake-state').innerText(),'保护关闭 · 已释放');assert.equal(dialogs,2);
  await brake.click();await page.waitForFunction(()=>document.getElementById('sws-io-protection').getAttribute('aria-checked')==='true');
  assert.equal(await page.locator('#sws-io-brake-state').innerText(),'等待驱动反馈 · 已锁定');
  fail=true;await led.click();await page.locator('[role=alert]').filter({hasText:'设置'}).waitFor();
  assert.equal(await led.getAttribute('aria-checked'),'false','保存失败不虚假切换');
  assert.equal(await led.isEnabled(),true);assert.equal(await brake.isEnabled(),true);
  assert.equal(await page.evaluate(()=>window.swsPendingCommands),0,'错误时恢复正常刷新');
  for(const width of [1440,390,320]){
   await page.setViewportSize({width,height:1000});
   assert(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),'窄屏无水平溢出');
   for(const button of [led,brake])assert((await button.boundingBox()).width===44);
   await page.screenshot({path:path.join(output,'board-io-'+width+'.png'),fullPage:true});
  }
  assert.deepEqual(errors,[]);
  console.log('PASS: actual two switch controls, Wi-Fi style, distinct API settings, manual-release confirmation/cancel, status feedback, save failure recovery and 1440/390/320 layouts');
 }finally{await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1;});
