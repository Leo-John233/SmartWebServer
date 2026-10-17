require('../test-output.cjs');
const assert=require('node:assert/strict'),path=require('node:path');
const {chromium}=require('../../../webui/node_modules/playwright');
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try {
  const page=await browser.newPage({viewport:{width:1440,height:1000}});
  await page.goto('http://127.0.0.1:8765/index.htm?view=1');
  await page.locator('.sws-overview-grid .collapsible').click();
  const cards=page.locator('.sws-io-control,.sws-mount-type');
  const ys=await cards.evaluateAll(items=>items.map(e=>e.getBoundingClientRect().y));
  assert.equal(ys.length,3);assert(ys.every(y=>y===ys[0]));
  await page.locator('.sws-overview-grid > .b1').screenshot({path:path.join(__dirname,'settings-demo-desktop.png')});
  await page.goto('http://127.0.0.1:8765/net.htm?view=1');
  await page.waitForFunction(()=>document.querySelector('#nw-current .nw-row'));
  assert.equal(await page.locator('#nw-message').isVisible(),false);
  await page.screenshot({path:path.join(__dirname,'network-demo-fixed.png'),fullPage:true});
  console.log('PASS: live demo displays three aligned settings and no duplicate network notice');
 } finally {await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1});
