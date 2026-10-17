const output=require('../test-output.cjs');
const path=require('node:path'),assert=require('node:assert/strict');
const {chromium}=require('../../../webui/node_modules/playwright');
const t=require('../page-templates.cjs')(path.join(__dirname,'../../SmartWebServer'));
const cards=require('../mount-page.cjs')(),l=t['ui/Layouts.h'],scripts=t['htmlScripts.h'];
const body=l.html_layout_mount_begin+cards.Guide+l.html_layout_mount_tracking+cards.Tracking+l.html_layout_mount_location+cards.Site+cards.Limits+l.html_layout_mount_target+cards.Goto+l.html_layout_mount_home+cards.HomePark+l.html_layout_mount_library+cards.Library+l.html_layout_mount_advanced+cards.Align+cards.Pec+l.html_layout_mount_end;
const clock=scripts.html_script_ajax_date_time_return+'<script>'+scripts.html_script_ajax.match(/function pad[\s\S]*?(?=var autoTimer)/)[0]+'</script>';
const html='<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">'+t['Appearance.h'].html_shared_appearance+'<script>window.commands=[];function s(k,v){commands.push([k,v])}function g(v){s("dr",v)}function autoFastRun(){}</script>'+clock+'<div class="b">'+body+'</div>'+scripts.html_script_collapsible+'<div style="height:1200px"></div>';
(async()=>{const browser=await chromium.launch({channel:'msedge',headless:true});try{
 const page=await browser.newPage({viewport:{width:1440,height:900}}),errors=[];
 page.on('pageerror',e=>errors.push(e.message));await page.setContent(html);
 assert.deepEqual(await page.locator('.sws-site-unit > span').allTextContents(),['°','′','″','°','′','″']);
 assert.deepEqual(await page.locator('.sws-coordinate-unit > span').allTextContents(),['h','m','s','°','′','″']);
 const field=page.locator('[name=ra_m]');await field.fill('30');await field.hover();await field.focus();
 const before=await page.evaluate(()=>scrollY);await page.mouse.wheel(0,240);await page.waitForTimeout(250);
 assert.equal(await field.inputValue(),'30','actual wheel must not step the number');
 assert(await page.evaluate(y=>scrollY>y,before),'wheel must still scroll the page');
 await field.focus();await field.press('ArrowUp');assert.equal(await field.inputValue(),'31','keyboard stepping remains available');
 await page.evaluate(()=>{const n=document.createElement('input');n.id='wheel-dynamic';n.type='number';n.value='10';document.body.lastElementChild.before(n);});
 const dynamic=page.locator('#wheel-dynamic');await dynamic.hover();await dynamic.focus();await page.mouse.wheel(0,-160);await page.waitForTimeout(200);assert.equal(await dynamic.inputValue(),'10','dynamically created numbers are protected');
 assert.deepEqual(await page.evaluate(()=>commands),[],'wheel does not send device commands');
 for(const [name,value]of Object.entries({ra_h:'5',ra_m:'35',ra_s:'17',dec_d:'5',dec_m:'23',dec_s:'28'}))await page.locator('[name='+name+']').fill(value);
 await page.locator('[name=dec_sign]').selectOption('-');await page.locator('[name=dec_s]').evaluate(e=>e.blur());
 for(const width of [1440,390,320]){
  await page.setViewportSize({width,height:1000});
  assert(await page.locator('.sws-coordinate-unit span, .sws-site-unit > span').evaluateAll(es=>es.every(e=>parseFloat(getComputedStyle(e).fontSize)>=18)));
  for(const [label,locator]of [['position',page.locator('.sws-site')],['coordinates',page.locator('#sws-coordinate-form')]]){
   assert(await locator.evaluate(e=>e.scrollWidth<=e.clientWidth+1),label+' must fit '+width);
   await locator.screenshot({path:path.join(output,'units-'+label+'-'+width+'.png')});
  }
 }
 assert.deepEqual(errors,[]);console.log('PASS: visible unit symbols, 320/390/1440 layouts, actual mouse wheel does not alter values or send commands, native page scrolling and keyboard stepping retained, dynamic inputs protected');
}finally{await browser.close();}})().catch(e=>{console.error(e);process.exit(1)});
