
const assert=require('node:assert/strict');
const path=require('node:path');
require('../test-output.cjs');
const {chromium}=require('../../../webui/node_modules/playwright');
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try{
  const page=await browser.newPage({viewport:{width:1440,height:1000}}),errors=[];
  page.on('pageerror',e=>errors.push(e.message));
  await page.goto('http://127.0.0.1:8765/net.htm');
  const frame=page.frameLocator('#sws-frame');
  await frame.locator('#nw-current button').waitFor();
  await frame.locator('#nw-results button').nth(2).waitFor();
  for(const name of ['ccto','wcto','otapwd','webpwd','logout'])assert.equal(await frame.locator('[name='+name+']').count(),1,name);
  assert.equal(await frame.locator('#nw-current .nw-wifi path[opacity="1"]').count(),1);
  assert.deepEqual((await frame.locator('#nw-results .nw-wifi').evaluateAll(nodes=>nodes.map(n=>n.querySelectorAll('path[opacity="1"]').length))).sort(),[1,2,3]);
  await frame.locator('#nw-sta-card').screenshot({path:path.join(__dirname,'wifi-signal-demo.png')});
  await frame.locator('.sws-network-layout').screenshot({path:path.join(__dirname,'network-complete-demo.png')});
  await page.locator('.t a[href="/mount.htm"]').click();
  await frame.locator('.sws-pad').waitFor();
  for(const id of ['guide_sta','track','guide_rate'])assert.equal(await frame.locator('#'+id).isVisible(),false,id);
  await frame.locator('.sws-control-grid').screenshot({path:path.join(__dirname,'mount-complete-demo.png')});
  assert.deepEqual(errors,[]);
  console.log('PASS: live demo, complete network settings, varied signal icons and compact mount controls');
 }finally{await browser.close();}
})().catch(e=>{console.error(e);process.exit(1)});

