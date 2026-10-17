
require('../test-output.cjs');
const assert=require('node:assert/strict'),path=require('node:path');
const {chromium}=require('../../../webui/node_modules/playwright');
(async()=>{const browser=await chromium.launch({channel:'msedge',headless:true});try{
 const page=await browser.newPage({viewport:{width:1440,height:1100}}),errors=[];page.on('pageerror',e=>errors.push(e.message));
 await page.goto('http://127.0.0.1:8765/mount.htm');const f=page.frameLocator('#sws-frame');
 await f.locator('.sws-site-limits').waitFor();
 assert.equal(await f.locator('#park').isVisible(),true);assert.equal(await f.locator('#unpark').isVisible(),false);
 await f.locator('.sws-site-limits').screenshot({path:path.join(__dirname,'mount-demo-new-cards.png')});
 await f.locator('.sws-home-actions').screenshot({path:path.join(__dirname,'mount-demo-new-park.png')});
 await f.locator('#park').click();await f.locator('#unpark').waitFor();assert.equal(await f.locator('#park').isVisible(),false);
 await f.locator('#unpark').click();await f.locator('#park').waitFor();
 await f.locator('[data-edit-form=sws-site-form]').click();
 const raw=f.locator('[name=g1]'),value=raw.locator('..').locator('input:not([hidden])');
 await value.fill('45');await f.locator('#sws-site-form button[type=submit]').click();
 await page.waitForTimeout(50);await f.locator('[data-edit-form=sws-site-form]').click();await value.fill('30');
 await f.locator('#sws-site-form button[type=reset]').click();await page.waitForTimeout(50);assert.equal(await value.inputValue(),'45');
 assert.deepEqual(errors,[]);console.log('PASS: live mount demo, card order, park/unpark state and save/cancel persistence');
}finally{await browser.close();}})().catch(e=>{console.error(e);process.exit(1)});
