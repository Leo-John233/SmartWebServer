require('../test-output.cjs');
const assert=require('node:assert/strict'),path=require('node:path');
const {chromium}=require('../../../webui/node_modules/playwright');
(async()=>{
 const browser=await chromium.launch({channel:'msedge',headless:true});
 try {
  const page=await browser.newPage({viewport:{width:1440,height:1000}});
  await page.goto('http://127.0.0.1:8765/index.htm?view=1');
  await page.locator('.sws-overview-grid .collapsible').click();
  const toggle=page.locator('#sws-io-protection');
  await page.waitForFunction(()=>document.querySelector('#sws-io-protection').getAttribute('aria-checked')==='true');
  page.once('dialog',async dialog=>{assert(dialog.message().includes('保持抱闸'));await dialog.accept();});
  await toggle.click();
  await page.waitForFunction(()=>document.querySelector('#sws-io-protection').getAttribute('aria-checked')==='false'&&document.querySelector('#sws-io-brake-state').textContent==='保护关闭，已抱闸');
  await page.locator('.sws-overview-grid > .b1').screenshot({path:path.join(__dirname,'brake-off-demo.png')});
  await toggle.click();await page.waitForFunction(()=>document.querySelector('#sws-io-protection').getAttribute('aria-checked')==='true');
  console.log('PASS: live demo toggle OFF reports locked brake and uses updated confirmation');
 } finally {await browser.close();}
})().catch(e=>{console.error(e);process.exitCode=1});
