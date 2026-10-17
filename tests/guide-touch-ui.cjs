const output=require('./test-output.cjs');
const path=require('path'),assert=require('assert/strict');
const {chromium}=require('playwright');
const t=require('./page-templates.cjs')(path.join(__dirname,'..'));
const scripts=t['htmlScripts.h'];
const html='<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">'+t['Appearance.h'].html_shared_appearance+
  scripts.html_script_ajax_get.replace('%s','mount-ajax-get.txt')+
  scripts.html_script_motion_controls_1+scripts.html_script_motion_controls_2+
  '<section class="sws-guide"><div class="b1">'+t['mount/GuideTile.h'].html_guidePad+'</div></section>';
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try{
  const context=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true});
  const page=await context.newPage(),sent=[],errors=[];
  page.on('pageerror',e=>{errors.push(e.message);console.error(e.stack);});
  await page.route('http://guide.test/**',r=>{
   const url=new URL(r.request().url());
   if(url.pathname.endsWith('.txt')){sent.push(url.searchParams.get('dr')||url.searchParams.get('goto'));return r.fulfill({body:''});}
   return r.fulfill({contentType:'text/html',body:html});
  });
  await page.goto('http://guide.test');
  const pad=page.locator('.sws-pad');const b=await pad.boundingBox();
  assert.equal(await pad.evaluate(e=>getComputedStyle(e).touchAction),'none');
  const cdp=await context.newCDPSession(page);
  for(const [direction,rx,ry]of [['n',.5,.15],['e',.15,.5],['w',.85,.5],['s',.5,.85]]){
   sent.length=0;const x=b.x+b.width*rx,y=b.y+b.height*ry;
   await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{x,y}]});
   await page.waitForTimeout(100);assert.deepEqual(sent,[direction+'1'],'按住期间只发送一次开始命令');
   await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[{x:Math.min(385,b.x+b.width+25),y:b.y+b.height+50}]});
   await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
   await page.waitForTimeout(650);assert(sent.slice(1).includes(direction+'0')||sent.slice(1).includes('q'),'移出按钮后松手仍停止');
  }
  sent.length=0;
  await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{x:b.x+b.width/2,y:b.y+b.height*.15}]});
  await cdp.send('Input.dispatchTouchEvent',{type:'touchCancel',touchPoints:[]});
  await page.waitForTimeout(650);assert(sent.includes('n1')&&(sent.includes('n0')||sent.includes('q')));
  // 只提供 Pointer 事件的触摸浏览器也必须响应。
  sent.length=0;await page.locator('.sws-pad button').first().evaluate(e=>{
   e.dispatchEvent(new PointerEvent('pointerdown',{bubbles:true,pointerType:'touch',pointerId:42}));
   e.dispatchEvent(new PointerEvent('pointerup',{bubbles:true,pointerType:'touch',pointerId:42}));
  });
  await page.waitForTimeout(650);assert.equal(sent.filter(v=>v==='n1').length,1);assert(sent.includes('n0'));
  // 上次松手的延迟停止不能打断随后开始的反向长按。
  sent.length=0;await page.evaluate(()=>{gmc.t(null,'n1','n');gmc.u(null,'n');gmc.t(null,'s1','s');});
  await page.waitForTimeout(650);
  assert(!sent.slice(sent.indexOf('s1')+1).some(v=>v==='n0'||v==='q'),'旧停止重试不能打断新的反向长按');
  await page.evaluate(()=>gmc.stopAll());await page.waitForTimeout(650);
  // 真正按住超过主控常见的 5 秒时限，网页期间不能自行发停止或重发启动。
  for (const kind of ['mouse','touch']) {
   sent.length=0;
   const x=b.x+b.width/2,y=b.y+b.height*.15;
   if(kind==='mouse'){await page.mouse.move(x,y);await page.mouse.down();}
   else await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{x,y}]});
   await page.waitForTimeout(6000);assert.deepEqual(sent,['n1'],kind+' 按住 6 秒无自动停止');
   if(kind==='touch')await cdp.send('Input.dispatchTouchEvent',{type:'touchMove',touchPoints:[{x:x+4,y:y+3}]});
   await page.waitForTimeout(6000);assert.deepEqual(sent,['n1'],kind+' 按住 12 秒及轻微移动无自动停止');
   if(kind==='mouse')await page.mouse.up();
   else await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});
   await page.waitForTimeout(650);assert(sent.includes('n0'),kind+' 松手后停止');
  }
  // 无关指针的抬起或失去捕获不能打断正在按住的方向。
  sent.length=0;
  await page.locator('.sws-pad button').first().evaluate(e=>{
   e.dispatchEvent(new PointerEvent('pointerdown',{bubbles:true,pointerType:'touch',pointerId:71}));
   window.dispatchEvent(new PointerEvent('pointerup',{pointerType:'touch',pointerId:99}));
   window.dispatchEvent(new PointerEvent('lostpointercapture',{pointerType:'mouse',pointerId:98}));
   window.dispatchEvent(new Event('touchend'));
  });
  await page.waitForTimeout(650);assert.deepEqual(sent,['n1']);
  await page.locator('.sws-pad button').first().evaluate(e=>e.dispatchEvent(new PointerEvent('pointerup',{bubbles:true,pointerType:'touch',pointerId:71})));
  await page.waitForTimeout(650);assert(sent.includes('n0'));
  sent.length=0;await page.evaluate(()=>{gmc.t(null,'n1','n');gmc.stopAll();gmc.t(null,'s1','s');});
  await page.waitForTimeout(650);
  assert(!sent.slice(sent.indexOf('s1')+1).includes('q'),'旧全局停止重试不能打断新长按');
  await page.evaluate(()=>gmc.stopAll());await page.waitForTimeout(650);
  // 无 Pointer Events 的旧浏览器保留原来的 Touch 路径。
  sent.length=0;await page.evaluate(()=>{window.PointerEvent=undefined;const e=document.querySelector('.sws-pad button');e.dispatchEvent(new Event('touchstart',{bubbles:true,cancelable:true}));e.dispatchEvent(new Event('touchend',{bubbles:true,cancelable:true}));});
  await page.waitForTimeout(650);assert.equal(sent.filter(v=>v==='n1').length,1);assert(sent.includes('n0'));
  assert.deepEqual(errors,[]);
  console.log('PASS: real mobile touches in four directions, release outside pad, touch cancellation, pointer-only touch and legacy touch fallback');
 }finally{await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1;});
