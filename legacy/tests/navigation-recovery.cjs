const output=require('./test-output.cjs');
const path=require('path'),assert=require('assert/strict');
const {chromium}=require('../../webui/node_modules/playwright');
const t=require('./page-templates.cjs')(path.join(__dirname,'../SmartWebServer'));
const shell='<!doctype html><meta charset="utf-8"><div class="t"><table><tr><td>OnStep</td><td>test</td></tr></table><a href="/index.htm">控制器</a><a href="/mount.htm">赤道仪</a><a href="/net.htm">网络</a></div><span id="sws-sidebar-label">test</span><p id="sws-load"></p><iframe id="sws-frame"></iframe>'+t['Appearance.h'].html_shell_script;
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try {
  const page=await browser.newPage();let broken=false,hang=false,held=[];
  await page.route('http://recovery.test/**',async route=>{
   const u=new URL(route.request().url());
   if(u.pathname==='/sws-ui.css')return broken==='css'?route.abort():route.fulfill({contentType:'text/css',body:':root{--bg:#111}'});
   if(u.pathname==='/sws-ui.js')return broken==='js'?route.abort():route.fulfill({contentType:'application/javascript',body:'window.swsWatchCommand=function(){}'});
   if(!u.searchParams.has('view'))return route.fulfill({contentType:'text/html',body:shell});
   if(hang){held.push(route);return;}
   return route.fulfill({contentType:'text/html',body:'<!doctype html><meta charset="utf-8"><link rel="stylesheet" href="/sws-ui.css"><script src="/sws-ui.js"></script><div class="b">'+u.pathname+'</div><script>window.swsPausePolling=()=>window.paused=true;window.swsResumePolling=()=>window.paused=false;</script>'});
  });
  await page.goto('http://recovery.test/index.htm');
  const loaded=async path=>{await page.waitForFunction(path=>{const f=document.getElementById('sws-frame');return f.contentDocument.querySelector('.b')?.textContent===path&&f.style.visibility!== 'hidden'&&document.getElementById('sws-load').textContent==='';},path);};
  await loaded('/index.htm');await page.clock.install();
  await page.clock.runFor(60*60*1000);
  await page.evaluate(()=>swsNavigate('/mount.htm',true));await loaded('/mount.htm');
  // 停止队列不完成时恢复当前页，不能无限锁死，也不能越过等待直接切页。
  await page.evaluate(()=>{document.getElementById('sws-frame').contentWindow.swsMotionStops=[()=>new Promise(()=>{})];window.pendingNav=swsNavigate('/net.htm',true);});
  await page.clock.runFor(5100);
  assert(await page.evaluate(()=>{const f=document.getElementById('sws-frame');return f.contentWindow.location.pathname==='/mount.htm'&&!f.contentDocument.body.inert&&!f.contentWindow.paused&&f.style.pointerEvents!=='none';}));
  await page.evaluate(()=>document.getElementById('sws-frame').contentWindow.swsMotionStops=[]);
  // 新页面请求卡住时显示重试提示，并允许下一次导航恢复。
  hang=true;await page.evaluate(()=>swsNavigate('/net.htm',true));await page.waitForTimeout(50);
  await page.clock.runFor(12100);
  assert.match(await page.locator('#sws-load').innerText(),/重试/);
  assert.equal(await page.locator('#sws-frame').evaluate(e=>e.style.pointerEvents),'');
  hang=false;for(const route of held)await route.abort().catch(()=>{});held=[];
  await page.evaluate(()=>swsNavigate('/index.htm',true));await loaded('/index.htm');
  // 资源丢失时不展示原始布局；重试后正常显示。
  for(const resource of ['css','js']) {
   broken=resource;await page.evaluate(()=>swsNavigate('/net.htm',true));
   await page.waitForFunction(()=>document.getElementById('sws-load').textContent.includes('重试'));
   assert.equal(await page.locator('#sws-frame').evaluate(e=>e.style.visibility),'hidden');
   broken=false;await page.evaluate(()=>swsNavigate('/net.htm',true));await loaded('/net.htm');
  }
  console.log('PASS: one-hour idle, stalled stop queue, stalled document, missing CSS/JS and manual navigation recovery');
 }finally{await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1;});
