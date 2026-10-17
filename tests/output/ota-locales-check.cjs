require('../test-output.cjs');
const assert=require('node:assert/strict');
const network=require('../network-page.cjs');
for(const language of ['cn','en','us','ca','de','es','fr','it','jp','ro']) {
 process.env.SWS_TEST_LANGUAGE=language;
 const html=network({fragment:true});
 assert(html.includes("class='sws-ota-guide'"));
 assert(html.includes("class='sws-ota-port'>-OTA</span>"));
 assert(!/L_NETWORK_OTA_\w+/.test(html));
 assert(html.includes("name='otaen'"));
 console.log('PASS: actual OTA page '+language);
}
