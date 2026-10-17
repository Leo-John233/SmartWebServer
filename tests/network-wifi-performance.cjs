require('./test-output.cjs');
const path=require('path'),assert=require('assert/strict');
const {chromium}=require('playwright');
const t=require('./page-templates.cjs')(path.join(__dirname,'..'));
const html='<meta charset="utf-8">'+t['network/NetworkWifi.h'].html_network_css+t['Appearance.h'].html_shared_appearance+'<div class="b">'+require('./network-page.cjs')({fragment:true})+'</div>';
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try {
  const page=await browser.newPage();const errors=[];page.on('pageerror',e=>errors.push(e.message));
  await page.clock.install();
  await page.addInitScript(()=>{
   window.sim={delay:20,scanRunning:false,statusError:false,hidden:false,calls:[],active:0,maxActive:0,state:{sta:{enabled:true,ssid:'Home',open:false,dhcp:true,ip:'192.168.1.2',gw:'192.168.1.1',sn:'255.255.255.0'},ap:{enabled:true,ssid:'SWS',channel:7,ip:'192.168.0.1',gw:'192.168.0.1',sn:'255.255.255.0'},connected:true,connecting:false,pending:false,failed:false,apActive:true,ssid:'Home',ip:'192.168.1.2',rssi:-55,apIp:'192.168.0.1',apSsid:'SWS',message:''}};
   Object.defineProperty(document,'hidden',{configurable:true,get:()=>sim.hidden});
   window.fetch=(url,options={})=>new Promise((resolve,reject)=>{
    const action=new URL(url,location.href).searchParams.get('action');
    sim.calls.push(action);sim.maxActive=Math.max(sim.maxActive,++sim.active);
    let done=false;const finish=fn=>{if(done)return;done=true;sim.active--;clearTimeout(timer);fn();};
    const timer=setTimeout(()=>finish(()=>{
     if(action==='status'&&sim.statusError)return reject(Error('offline'));
     const data=action==='scan'?{running:sim.scanRunning,networks:[]}:
      action==='status'?structuredClone(sim.state):{message:'accepted'};
     resolve({ok:true,json:async()=>data});
    }),sim.delay);
    options.signal?.addEventListener('abort',()=>finish(()=>reject(new DOMException('aborted','AbortError'))),{once:true});
   });
  });
  await page.route('http://wifi-performance.test/**',route=>route.fulfill({contentType:'text/html',body:html}));
  await page.goto('http://wifi-performance.test/');await page.clock.runFor(1000);
  assert.equal(await page.locator('#nw-scan').isEnabled(),true);
  await page.evaluate(()=>{sim.delay=7000;sim.calls=[];sim.maxActive=0;swsResumePolling();});
  await page.clock.runFor(6000);
  assert.equal(await page.evaluate(()=>sim.calls.length),1,'慢状态读取不会不断叠加请求');
  assert.equal(await page.evaluate(()=>sim.maxActive),1);
  await page.clock.runFor(1100);
  // 慢连接也只保留一个读取请求，扫描期间不给 ESP 叠加状态读取。
  await page.evaluate(()=>{sim.scanRunning=true;sim.delay=1000;sim.calls=[];sim.maxActive=0;document.getElementById('nw-scan').click();document.getElementById('nw-scan').click();});
  await page.clock.runFor(31000);
  assert.match(await page.locator('#nw-scan-state').innerText(),/超时/);
  assert.equal(await page.locator('#nw-scan').isEnabled(),true);
  assert(await page.evaluate(()=>!sim.calls.slice(0,-1).includes('status')),'扫描等待期间不叠加状态请求');
  assert.equal(await page.evaluate(()=>sim.maxActive),1);
  await page.evaluate(()=>{sim.scanRunning=false;sim.delay=20;});
  await page.locator('#nw-scan').click();await page.clock.runFor(100);
  assert.equal(await page.locator('#nw-scan').isEnabled(),true);
  // 标签隐藏或切页时停止读取，回到页面后恢复，不重发连接和设置。
  await page.evaluate(()=>{sim.hidden=true;document.dispatchEvent(new Event('visibilitychange'));});
  const hidden=await page.evaluate(()=>sim.calls.length);await page.clock.runFor(120000);
  assert.equal(await page.evaluate(()=>sim.calls.length),hidden);
  await page.evaluate(()=>{sim.hidden=false;document.dispatchEvent(new Event('visibilitychange'));});await page.clock.runFor(50);
  assert(await page.evaluate(()=>sim.calls.length>0&&sim.calls.at(-1)==='status'));
  await page.evaluate(()=>{swsPausePolling();sim.calls=[];});await page.clock.runFor(120000);
  assert.equal(await page.evaluate(()=>sim.calls.length),0);
  await page.evaluate(()=>swsResumePolling());await page.clock.runFor(50);
  // 连接中途状态接口断开也不能一直锁住编辑入口。
  await page.evaluate(()=>{sim.state.connecting=true;swsResumePolling();});await page.clock.runFor(50);
  assert.equal(await page.locator('#nw-manual').isDisabled(),true);
  // 持续离线时逐步降低状态查询频率，重新联网立即读取且不重放配置。
  await page.evaluate(()=>{sim.statusError=true;sim.calls=[];swsResumePolling();});await page.clock.runFor(120000);
  assert(await page.evaluate(()=>sim.calls.length<=8),'离线两分钟不能继续按固定频率查询');
  const beforeOnline=await page.evaluate(()=>sim.calls.length);
  await page.evaluate(()=>{sim.statusError=false;sim.state.connecting=false;sim.state.background=false;dispatchEvent(new Event('online'));});await page.clock.runFor(50);
  assert.equal(await page.evaluate(()=>sim.calls.length),beforeOnline+1);
  assert.equal(await page.locator('#nw-manual').isEnabled(),true);
  await page.evaluate(()=>{sim.statusError=true;});await page.clock.runFor(2600);
  assert.equal(await page.locator('#nw-manual').isEnabled(),true);
  await page.evaluate(()=>{sim.statusError=false;sim.state.connecting=false;dispatchEvent(new Event('online'));});await page.clock.runFor(50);
  assert.equal(await page.locator('#nw-manual').isEnabled(),true);
  assert(await page.evaluate(()=>sim.calls.every(x=>x==='status'||x==='scan')),'所有恢复操作都没有重发连接命令');
  // 后台重连不锁住配网入口，用户操作可发出；手动连接仍防止重复提交。
  await page.evaluate(()=>{sim.state.connecting=true;sim.state.background=true;swsResumePolling();});await page.clock.runFor(50);
  assert.equal(await page.locator('#nw-manual').isEnabled(),true);
  assert.equal(await page.locator('#nw-scan').isEnabled(),true);
  await page.locator('#nw-scan').click();await page.clock.runFor(100);
  assert.equal(await page.evaluate(()=>sim.calls.at(-1)),'status');
  await page.evaluate(()=>{sim.state.background=false;swsResumePolling();});await page.clock.runFor(50);
  assert.equal(await page.locator('#nw-manual').isDisabled(),true);
  await page.evaluate(()=>{sim.state.connecting=false;swsResumePolling();});await page.clock.runFor(50);
  await page.evaluate(()=>{sim.delay=1000;sim.maxActive=0;sim.calls=[];swsResumePolling();});await page.clock.runFor(10);
  await page.locator('#nw-scan').click();await page.clock.runFor(50);
  assert.equal(await page.evaluate(()=>sim.maxActive),1,'用户扫描前取消旧状态请求，避免并发叠加');
  await page.clock.runFor(2100);
  assert.deepEqual(errors,[]);
  console.log('PASS: slow scan deadline, single read, duplicate clicks, hidden/navigation pause, reconnect and unlock after status failure');
 }finally{await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1;});
