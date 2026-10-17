const output=require('./test-output.cjs');
const path=require('node:path'),assert=require('node:assert/strict');
const {chromium}=require('playwright');
const root=path.join(__dirname,'..');
const templates=require('./page-templates.cjs')(root);
const card=require('./board-io-backend.cjs')({preview:true});
const hiddenCard=require('./board-io-backend.cjs')({preview:true,brakeEnabled:false});
const mountForm=templates['index/StatusTile.h'].html_indexMountType.replace('%s',' selected').replace('%s','').replace('%s','');
const render=(controls,columns)=>'<!doctype html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width"></head><body>'+templates['Appearance.h'].html_shared_appearance+'<main class="sws-controller"><div class="sws-overview-grid"><div class="b1"><div>工作状态</div><hr>'+templates['index/StatusTile.h'].html_indexSettingsBegin.replace('%d',columns)+controls+mountForm+'</div></div></div></main></body></html>';
const html=render(card,3),hiddenHtml=render(hiddenCard,2);
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try{
  const page=await browser.newPage({viewport:{width:1440,height:1000}}),errors=[],posts=[];
  let state={activity:true,protection:true,locked:false,message:'状态正常，已松闸'},fail=false,confirmation=true,dialogs=0,rejectAction=false;
  page.on('pageerror',e=>errors.push(e.message));
  page.on('dialog',async dialog=>{dialogs++;assert(dialog.message().includes('确认停止后抱闸'));if(confirmation)await dialog.accept();else await dialog.dismiss();});
  await page.route('http://board.test/**',async route=>{
   const request=route.request(),url=new URL(request.url());
   if(url.pathname==='/board-io'){
    if(request.method()==='POST'){
     const values=Object.fromEntries(new URLSearchParams(request.postData()));posts.push(values);
     if(fail)return route.fulfill({status:500,body:'{}',contentType:'application/json'});
     if(values.action){if(rejectAction)return route.fulfill({status:409,body:'{}',contentType:'application/json'});state.message=values.action==='reset'?'状态正常，已松闸':'正在停止，等待确认';if(values.action==='reset')state.locked=false;}else{state[values.setting]=values.enabled==='1';if(values.setting==='protection')state.locked=true;state.message=state.protection?'已抱闸，请手动解除锁定':'正在停止，等待确认';}
    }
    return route.fulfill({body:JSON.stringify(state),contentType:'application/json'});
   }
   return route.fulfill({body:url.pathname==='/disabled'?hiddenHtml:html,contentType:'text/html; charset=utf-8'});
  });
  await page.goto('http://board.test/');
  assert.equal(await page.locator('[data-action=reset]').isVisible(),false,'正常时隐藏故障操作');
  const led=page.locator('#sws-io-activity'),brake=page.locator('#sws-io-protection');
  const toggle=await led.evaluate(e=>{const r=e.getBoundingClientRect(),k=getComputedStyle(e,'::after');return{width:r.width,height:r.height,knob:k.width,color:k.backgroundColor};});
  assert.deepEqual(toggle,{width:44,height:26,knob:'18px',color:'rgb(255, 255, 255)'},'开关与 Wi-Fi 开关尺寸、白色圆钮一致');
  await page.mouse.move(0,0);
  assert.equal(await led.evaluate(e=>getComputedStyle(e).backgroundColor),'rgb(118, 27, 27)','开启颜色使用暗红主题');
  assert.equal(await led.evaluate(e=>getComputedStyle(e).backgroundImage),'none','选中开关不叠加常驻灰色遮罩');
  const cards=page.locator('.sws-io-control, .sws-mount-type');
  const bounds=await cards.evaluateAll(items=>items.map(e=>{const r=e.getBoundingClientRect();return{x:r.x,y:r.y,width:r.width,height:r.height};}));
  assert.equal(bounds.length,3);assert(bounds.every(r=>r.y===bounds[0].y),'桌面三个设置并排');
  assert(Math.max(...bounds.map(r=>r.width))-Math.min(...bounds.map(r=>r.width))<2,'三个卡片等宽');
  assert(!(await cards.allInnerTexts()).join('').match(/[。·]/),'说明和状态不带句号与中点');
  assert.equal(await page.locator('[name=mountt]').inputValue(),'1');
  assert.deepEqual(await page.locator('[name=mountt] option').evaluateAll(items=>items.map(e=>e.value)),['1','2','3']);
  await page.locator('[name=mountt]').selectOption('3');
  const submitted=await page.evaluate(()=>Object.fromEntries(new FormData(document.querySelector('.sws-mount-type'))));
  assert.deepEqual(submitted,{mountt:'3'},'赤道仪类型保持原始数字接口');
  assert.equal(await page.locator('.sws-mount-type').getAttribute('action'),'/index.htm');
  assert.equal(await page.locator('[name=revert]').getAttribute('value'),'0','保留原恢复按钮');
  await led.click();await page.waitForFunction(()=>document.getElementById('sws-io-activity').getAttribute('aria-checked')==='false');
  assert.deepEqual(posts[0],{setting:'activity',enabled:'0'});
  confirmation=false;await brake.click();assert.equal(posts.length,1,'取消关闭保护时不发送请求');
  confirmation=true;await brake.click();await page.waitForFunction(()=>document.getElementById('sws-io-protection').getAttribute('aria-checked')==='false');
  assert.equal(await page.locator('#sws-io-brake-state').innerText(),'正在停止，等待确认');assert.equal(dialogs,2);
  await brake.click();await page.waitForFunction(()=>document.getElementById('sws-io-protection').getAttribute('aria-checked')==='true');
  assert.equal(await page.locator('#sws-io-brake-state').innerText(),'已抱闸，请手动解除锁定');
  assert.equal(await page.getByRole('button',{name:'解除锁定',exact:true}).isVisible(),true);
  assert.equal(await page.getByRole('button',{name:'再次停止',exact:true}).isVisible(),true);
  await brake.locator('..').screenshot({path:path.join(output,'board-io-locked.png')});
  fail=true;await led.click();await page.locator('[role=alert]').filter({hasText:'保存'}).waitFor();
  assert.equal(await led.getAttribute('aria-checked'),'false','保存失败不虚假切换');
  assert.equal(await led.isEnabled(),true);assert.equal(await brake.isEnabled(),true);
  assert.equal(await page.evaluate(()=>window.swsPendingCommands),0,'错误时恢复正常刷新');
  const failedBounds=await cards.evaluateAll(items=>items.map(e=>e.getBoundingClientRect().y));
  assert(failedBounds.every(y=>y===failedBounds[0]),'错误提示不挤开三个并排设置');
  fail=false;await led.click();await page.waitForFunction(()=>document.getElementById('sws-io-activity').getAttribute('aria-checked')==='true');
  rejectAction=true;await page.locator('[data-action=reset]').click();await page.locator('[role=alert]').filter({hasText:'暂时无法操作'}).waitFor();
  assert.deepEqual(posts.at(-1),{action:'reset'});
  rejectAction=false;await page.locator('[data-action=retry-stop]').click();await page.waitForFunction(()=>document.getElementById('sws-io-brake-state').textContent==='正在停止，等待确认');assert.deepEqual(posts.at(-1),{action:'retry-stop'});
  await page.locator('[data-action=reset]').click();await page.waitForFunction(()=>document.getElementById('sws-io-brake-state').textContent==='状态正常，已松闸');
  assert.deepEqual(posts.at(-1),{action:'reset'});
  assert.equal(await page.locator('[data-action=reset]').isVisible(),false,'解除锁定后隐藏故障操作');
  await page.locator('[name=mountt]').selectOption('1');await page.mouse.move(0,0);
  for(const width of [1440,390,320]){
   await page.setViewportSize({width,height:1000});
   assert(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),'窄屏无水平溢出');
   for(const button of [led,brake])assert((await button.boundingBox()).width===44);
   assert(await page.locator('.sws-mount-type').evaluate(e=>e.getBoundingClientRect().bottom<=e.closest('.b1').getBoundingClientRect().bottom),'设置表单完整显示在卡片内');
   if(width<600){const boxes=await cards.evaluateAll(items=>items.map(e=>e.getBoundingClientRect().y));assert(boxes[0]<boxes[1]&&boxes[1]<boxes[2],'窄屏三个设置自然纵向排列');}
   await page.screenshot({path:path.join(output,'board-io-'+width+'.png'),fullPage:true});
  }
  await page.goto('http://board.test/disabled');
  assert.equal(await page.locator('#sws-io-protection,#sws-io-brake-state').count(),0,'编译关闭后没有抱闸开关和状态');
  assert.equal(await page.locator('#sws-io-activity').count(),1,'活动指示灯不受影响');
  for(const width of [1440,390,320]){
   await page.setViewportSize({width,height:1000});
   assert(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),'关闭抱闸功能后布局不溢出');
   if(width===1440){const boxes=await page.locator('.sws-io-control,.sws-mount-type').evaluateAll(items=>items.map(e=>{const r=e.getBoundingClientRect();return{y:r.y,width:r.width};}));assert.equal(boxes.length,2);assert.equal(boxes[0].y,boxes[1].y);assert(Math.abs(boxes[0].width-boxes[1].width)<2,'隐藏后剩余两项等宽，无空白占位');}
   await page.screenshot({path:path.join(output,'board-io-disabled-'+width+'.png'),fullPage:true});
  }
  assert.deepEqual(errors,[]);
  console.log('PASS: theme switches, original mount type interface, disable-to-lock confirmation, save failure recovery, config OFF hides brake controls, responsive two/three-card layouts');
 }finally{await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1;});
