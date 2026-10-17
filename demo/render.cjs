const testOutput = require('../tests/test-output.cjs');
// 从固件模板渲染页面，模拟设备数据并验证导航与运动停止时序
const fs=require('node:fs'),path=require('node:path'),assert=require('node:assert/strict');

const root=path.join(__dirname,'../SmartWebServer'),out=fs.mkdtempSync(path.join(testOutput,'sws-demo-'));fs.mkdirSync(out,{recursive:true});
const read=p=>fs.readFileSync(path.join(root,p),'utf8');

let shared,shellScript;
const headers=read('src/pages/htmlHeaders.h'),pageHeader=read('src/pages/Page.h'),scripts=read('src/pages/htmlScripts.h'),defs={BUTTON_N:'北',BUTTON_E:'东',BUTTON_W:'西',BUTTON_S:'南',BUTTON_SYNC:'同步'};
for(const source of [read('src/pages/ui/Palette.h'),headers,read('src/locales/Strings_cn.h')])for(const m of source.matchAll(/^#define\s+(\w+)\s+("(?:\\.|[^"\\])*")/gm))defs[m[1]]=JSON.parse(m[2]);
function constant(source,name){const m=new RegExp('const char '+name+'\\[\\] PROGMEM\\s*=').exec(source);if(!m)throw Error(name);let value='';for(const t of source.slice(m.index+m[0].length).matchAll(/"(?:\\.|[^"\\])*"|\b[A-Za-z_]\w*\b|;/g)){if(t[0]===';')return value;if(t[0][0]==='"')value+=JSON.parse(t[0]);else if(t[0] in defs)value+=defs[t[0]];else throw Error('未知宏 '+t[0]);}}
const h=n=>constant(headers,n),p=n=>constant(pageHeader,n);
// 模拟主控的配置名称，空名称与固件一样回退到 OnStep
const configName=process.env.SWS_DEMO_CONFIG_NAME??'OnStepX';
const displayName=(configName||'OnStep').replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const title=p('html_onstep_header_begin')+displayName+p('html_onstep_header_title')+'2.10h (OnStep10.28w'+p('html_onstep_header_links')+['idx','mnt','net','foc','rot','aux','enc'].map(k=>p('html_links_'+k+'_begin')+p('html_links_'+k+'_end')).join('')+p('html_onstep_header_end');
const head=h('html_head_begin')+h('html_main_css_begin')+h('html_main_css_core')+h('html_main_css_control')+h('html_main_css_collapse')+h('html_main_css_end');
const tile=(title,body)=>h('html_tile_text_beg').replace('%s','22em').replace('%s','18em').replace('%s',title)+'<br><hr>'+body+h('html_tile_end');
const motion=constant(scripts,'html_script_motion_controls_1')+constant(scripts,'html_script_motion_controls_2');
// 编译实际轴渲染函数，其他控件直接展开真实模板与中文宏
const templates=require('../tests/page-templates.cjs')(root),cp=require('node:child_process');
shared=templates['Appearance.h'].html_shared_appearance;shellScript=templates['Appearance.h'].html_shell_script;
// 与固件相同，公共样式和控件脚本按版本缓存；内容仍来自真实模板。
const resources = {
  css: templates['ui/Styles.h'].html_shared_styles.slice(8,-9) + templates['ui/Editors.h'].html_editor_styles.slice(8,-9) + templates['boardIo/View.h'].html_board_io_styles.slice(8,-9),
  js: templates['ui/Controls.h'].html_page_controls.slice(9,-10) + templates['ui/Editors.h'].html_settings_editors.slice(9,-10) + templates['boardIo/View.h'].html_board_io_script.slice(9,-10),
};
const resourceVersion = require('node:crypto').createHash('sha256').update(resources.css+resources.js).digest('hex').slice(0,12);
shared = '<link rel="stylesheet" href="/sws-ui.css?v='+resourceVersion+'"><script src="/sws-ui.js?v='+resourceVersion+'"></script>';
const axisPath=path.join(out,'axis.html');cp.execFileSync(process.execPath,[path.join(__dirname,'../tests/axis-page.cjs'),axisPath],{stdio:'pipe'});
const {controller,mount,optional,auxiliary}=require('../tests/full-page-preview.cjs')({templates,defs,tile,h,motion,axis:fs.readFileSync(axisPath,'utf8')});
const manifest=require('../tests/page-feature-manifest.cjs');for(const [file,names] of Object.entries(manifest)){const source=read('src/pages/'+file);for(const name of names)assert(source.includes(name+'('),file+' 缺少 '+name);}
const netBody=require('../tests/network-page.cjs')({fragment:true});
let loggedIn=true;const polls={},commands=[],documents=[];
const state={sta:{enabled:true,ssid:'Redmi_Hotel',open:false,dhcp:true,ip:'192.168.31.229',gw:'192.168.31.1',sn:'255.255.255.0'},ap:{enabled:true,ssid:'ONSTEPX',channel:7,ip:'192.168.0.1',gw:'192.168.0.1',sn:'255.255.255.0'},connected:true,connecting:false,pending:false,failed:false,apActive:true,ssid:'Redmi_Hotel',ip:'192.168.31.229',rssi:-74,apIp:'192.168.0.1',apSsid:'ONSTEPX',message:''};
function doc(url,embedded){if(!embedded)return head+'</head><body>'+shared+title+'<p id="sws-load" role="status"></p><iframe id="sws-frame" title="标签内容"></iframe>'+shellScript;const body=url==='/net.htm'?(loggedIn?netBody:'<form method="post" action="/net.htm"><label>管理密码 <input name="login" type="password"></label><button>登录</button></form>'):url==='/mount.htm'?mount:optional[url]||controller;return head+(url==='/net.htm'?templates['network/NetworkWifi.h'].html_network_css:'')+'</head><body>'+shared+title+'<div class="b">'+(url==='/mount.htm'?constant(scripts,'html_script_ajax_date_time_return')+'<script>'+templates['htmlScripts.h'].html_script_ajax.match(/function pad[\s\S]*?(?=var autoTimer)/)[0]+'</script>':url==='/focuser.htm'||url==='/rotator.htm'?motion:'')+body+'</div>'+constant(scripts,'html_script_collapsible')+'</body></html>';}

// 演示复用固件样式和控件，所有交互由本地模拟服务接管
module.exports={doc,state,head,shared,templates,auxiliary,resources,brakeAvailable:controller.includes("id='sws-io-protection'")};
