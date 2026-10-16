const testOutput = require('./test-output.cjs');
// 由预处理器展开真实模板和选定语言宏，避免预览自行改写描述
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process');
module.exports=function(root,language=process.env.SWS_TEST_LANGUAGE||'cn'){
  const base=path.join(root,'src/pages'),files=[];
  for(const dir of ['index','mount','focuser','rotator','encoders','auxiliary'])for(const name of fs.readdirSync(path.join(base,dir)))if(name.endsWith('.h'))files.push(dir+'/'+name);
  files.unshift('ui/Palette.h','ui/Text.h','ui/Layouts.h','htmlHeaders.h');files.push('LibraryHelp.h','htmlScripts.h','ui/Styles.h','ui/Controls.h','ui/Editors.h','ui/Navigation.h','network/ui/Styles.h','network/ui/View.h','network/ui/Script.h','network/Network.h');
  const clean=s=>s.replace(/^[ \t]*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
  let code='#define ON 1\n#define DISPLAY_SPECIAL_CHARS ON\n#define OFF 0\n#define PROGMEM\n#define DISPLAY_WEATHER ON\n#define DISPLAY_INTERNAL_TEMPERATURE ON\n#define WIFI 2\n#define OPERATIONAL_MODE WIFI\n#define DRIVE_CONFIGURATION ON\n#define DISPLAY_COORDINATE_ORIGIN ON\n#define DISPLAY_RESET_CONTROLS ON\n#define HOME_OFFSET_RANGE_AXIS1 3600\n#define HOME_OFFSET_RANGE_AXIS2 3600\n#define STR_INNER(x) #x\n#define STR(x) STR_INNER(x)\n';
  code+=require('./locale-source.cjs')(language)+'\n';
  const settings=fs.readFileSync(path.join(root,'src/Config.defaults.h'),'utf8')+'\n'+fs.readFileSync(path.join(root,'src/Constants.h'),'utf8');
  for(const name of ['AJAX_PAGE_UPDATE_RATE_MS','AJAX_PAGE_UPDATE_RATE_FAST_MS','AJAX_PAGE_UPDATE_FAST_SHED_MS','AJAX_PAGE_LAZY_GET_MS']){
    const value=settings.match(new RegExp('^#define '+name+'\\s+(\\d+)','m'));
    if(!value)throw Error('缺少刷新配置 '+name);
    code+='#define '+name+' '+value[1]+'\n';
  }
  files.forEach((file,i)=>code+='\nPREVIEW_FILE_'+i+'\n'+clean(fs.readFileSync(path.join(base,file),'utf8'))+'\n');
  const dir=fs.mkdtempSync(path.join(testOutput,'sws-templates-')),input=path.join(dir,'templates.cpp');fs.writeFileSync(input,code);
  const result=cp.spawnSync(process.env.CXX||'g++',['-E','-P',input],{encoding:'utf8',maxBuffer:8*1024*1024});if(result.status!==0)throw Error(result.stderr);
  const templates={};for(const section of result.stdout.split(/PREVIEW_FILE_/).slice(1)){
    const i=Number(section.match(/^\d+/)[0]),values={};
    Object.assign(values,require('./cpp-strings.cjs')(section));
    templates[files[i]]=values;
  }
  templates['Appearance.h'] = {
    html_shared_appearance: templates['ui/Styles.h'].html_shared_styles + templates['ui/Editors.h'].html_editor_styles + templates['ui/Controls.h'].html_page_controls + templates['ui/Editors.h'].html_settings_editors,
    html_shell_script: templates['ui/Navigation.h'].html_shell_script,
  };
  templates['network/NetworkWifi.h'] = {
    ...templates['network/ui/Styles.h'],
    html_network_wifi: templates['network/ui/View.h'].html_network_wifi + templates['network/ui/Script.h'].html_network_wifi_script,
  };
  return templates;
};
