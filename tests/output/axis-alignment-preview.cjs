const output=require('../test-output.cjs');
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process'),assert=require('node:assert/strict');
const {chromium}=require('../../../webui/node_modules/playwright');
const templates=require('../page-templates.cjs')(path.join(__dirname,'../../SmartWebServer'));
const axisPath=path.join(output,'axis-centered.html');
cp.execFileSync(process.execPath,[path.join(__dirname,'../axis-page.cjs'),axisPath]);
const h=templates['htmlHeaders.h'];
const html=h.html_head_begin+h.html_main_css_begin+h.html_main_css_core+h.html_main_css_control+h.html_main_css_collapse+h.html_main_css_end+'</head><body>'+templates['Appearance.h'].html_shared_appearance+'<div class="sws-axis-grid">'+fs.readFileSync(axisPath,'utf8')+'</div>'+templates['htmlScripts.h'].html_script_collapsible+'</body></html>';
(async()=>{const browser=await chromium.launch({channel:'msedge',headless:true});try{const page=await browser.newPage();await page.setContent(html);await page.locator('.collapsible').first().click();for(const width of [1440,390]){await page.setViewportSize({width,height:1000});const controls=await page.locator('.sws-parameter > input, .sws-parameter > select').evaluateAll(es=>es.map(e=>({align:getComputedStyle(e).textAlign,value:e.value})));assert(controls.length>0);assert(controls.every(e=>e.align==='center'));await page.locator('.b1').first().screenshot({path:path.join(output,'axis-centered-'+width+'.png')});console.log('PASS '+width+': '+controls.length+' axis parameter controls centered');}}finally{await browser.close();}})().catch(e=>{console.error(e);process.exit(1)});
