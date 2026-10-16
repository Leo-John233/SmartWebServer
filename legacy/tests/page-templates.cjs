// 由预处理器展开真实模板和中文语言宏，避免预览自行改写描述
const fs=require('node:fs'),path=require('node:path'),os=require('node:os'),cp=require('node:child_process');
module.exports=function(root){
  const base=path.join(root,'src/pages'),files=[];
  for(const dir of ['index','mount','focuser','rotator','encoders','auxiliary'])for(const name of fs.readdirSync(path.join(base,dir)))if(name.endsWith('.h'))files.push(dir+'/'+name);
  files.push('htmlHeaders.h','LibraryHelp.h','htmlScripts.h');
  const clean=s=>s.replace(/^[ \t]*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
  let code='#define ON 1\n#define OFF 0\n#define PROGMEM\n#define DISPLAY_WEATHER ON\n#define DISPLAY_INTERNAL_TEMPERATURE ON\n#define WIFI 2\n#define OPERATIONAL_MODE WIFI\n#define DRIVE_CONFIGURATION ON\n#define DISPLAY_COORDINATE_ORIGIN ON\n#define DISPLAY_RESET_CONTROLS ON\n#define HOME_OFFSET_RANGE_AXIS1 3600\n#define HOME_OFFSET_RANGE_AXIS2 3600\n#define STR_INNER(x) #x\n#define STR(x) STR_INNER(x)\n';
  code+=fs.readFileSync(path.join(root,'src/locales/Strings_cn.h'),'utf8')+'\n';
  files.forEach((file,i)=>code+='\nPREVIEW_FILE_'+i+'\n'+clean(fs.readFileSync(path.join(base,file),'utf8'))+'\n');
  const dir=fs.mkdtempSync(path.join(os.tmpdir(),'sws-templates-')),input=path.join(dir,'templates.cpp');fs.writeFileSync(input,code);
  const result=cp.spawnSync(process.env.CXX||'g++',['-E','-P',input],{encoding:'utf8',maxBuffer:8*1024*1024});if(result.status!==0)throw Error(result.stderr);
  const templates={};for(const section of result.stdout.split(/PREVIEW_FILE_/).slice(1)){
    const i=Number(section.match(/^\d+/)[0]),values={};
    for(const m of section.matchAll(/const char (\w+)\[\]\s*=\s*((?:"(?:\\.|[^"\\])*"\s*)+);/g))values[m[1]]=[...m[2].matchAll(/"(?:\\.|[^"\\])*"/g)].map(x=>JSON.parse(x[0])).join('');
    templates[files[i]]=values;
  }
  return templates;
};
