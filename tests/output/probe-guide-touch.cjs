
require('../test-output.cjs');
const {chromium}=require('../../../webui/node_modules/playwright');
(async()=>{const browser=await chromium.launch({channel:'msedge',headless:true});try{
const context=await browser.newContext({viewport:{width:390,height:844},isMobile:true,hasTouch:true});const page=await context.newPage();const sent=[];page.on('pageerror',e=>console.log('ERROR',e.message));
await page.route('**/mount-ajax-get.txt?**',r=>{sent.push(new URL(r.request().url()).searchParams.get('dr'));return r.fulfill({body:''});});
await page.goto('http://127.0.0.1:8765/mount.htm?view=1');const pad=page.locator('.sws-pad');await pad.scrollIntoViewIfNeeded();const b=await pad.boundingBox();const x=b.x+b.width/2,y=b.y+b.height*.16;
console.log(await page.evaluate(({x,y})=>({hit:document.elementFromPoint(x,y)?.outerHTML,hasTouch:'ontouchstart' in window,gmc:typeof gmc}),{x,y}));
const cdp=await context.newCDPSession(page);await cdp.send('Input.dispatchTouchEvent',{type:'touchStart',touchPoints:[{x,y}]});await page.waitForTimeout(300);await cdp.send('Input.dispatchTouchEvent',{type:'touchEnd',touchPoints:[]});await page.waitForTimeout(800);console.log('TOUCH',sent);
}finally{await browser.close();}})().catch(e=>{console.error(e);process.exitCode=1});
