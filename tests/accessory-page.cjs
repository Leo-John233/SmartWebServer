const testOutput = require('./test-output.cjs');
// 运行真实设备卡片，避免模拟页面跳过状态读数和设备条件
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process');
module.exports=({kind='focuser',version=10,count=2,derotator=true}={})=>{
  const root=path.join(__dirname,'../src'),read=p=>fs.readFileSync(path.join(root,p),'utf8');
  const clean=s=>s.replace(/^\s*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
  const focus=kind==='focuser',tiles=focus?['Select','Slewing','Home','BacklashTcf']:['Slewing','Home','Backlash','DeRotator'];
  let code=`#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#define PROGMEM
#define ON 1
#define OFF 0
#define DISPLAY_SPECIAL_CHARS ON
#define AJAX_PAGE_LAZY_GET_MS 200
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
#define MT_ALTAZM 3
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
struct String:std::string{using std::string::string;void concat(const char*s){append(s);}};
struct Web{std::string output;void sendContentAndClear(String&s){output+=s;s.clear();}}www;
struct Status{int focuserCount=${count},mountType=MT_ALTAZM;bool derotatorFound=${derotator};bool focuserPresent[6]={${Array.from({length:6},(_,i)=>i<count?'true':'false').join(',')}};int getVersionMajor(){return ${version};}}status;
struct State{bool focuserSlewing=false,rotatorSlewing=false,focuserTcfEnable=true;int focuserSelected=1;
 const char*focuserPositionStr="12500 µm",*rotatorPositionStr="45°30′",*focuserSlewSpeedStr="0.30mm/s",*rotateSlewSpeedStr="3.0&deg;/s",*focuserTemperatureStr="18.6 °C",*focuserBacklashStr="12",*focuserDeadbandStr="8",*focuserTcfCoefStr="-2.5";void updateFocuser(){}}state;
struct Device{bool command(const char*,char*out){strcpy(out,"16");return true;}}onStep;
`;
  code+=require('./locale-source.cjs')()+'\n'+require('./page-support.cjs')()+'\n'+clean(read('pages/htmlHeaders.h'))+'\n'+clean(read('pages/htmlScripts.h'))+'\n';
  for(const tile of tiles)code+=clean(read('pages/'+kind+'/'+tile+'Tile.h'))+'\n';
  for(const tile of tiles)code+=clean(read('pages/'+kind+'/'+tile+'Tile.cpp')).split('// use Ajax')[0]+'\n';
  code+='int main(){String data;';
  for(const tile of tiles){const func=tile==='DeRotator'?'deRotatorTile':kind+tile+'Tile';code+='std::cout<<"PREVIEW_'+tile+'\\n";www.output.clear();'+(tile==='DeRotator'?'if(status.derotatorFound)':'')+func+'(data);std::cout<<www.output<<data;data.clear();';}
  code+='}';
  const dir=fs.mkdtempSync(path.join(testOutput,'sws-accessory-')),input=path.join(dir,'render.cpp'),exe=path.join(dir,'render.exe');fs.writeFileSync(input,code);
  const build=cp.spawnSync(process.env.CXX||'g++',['-std=c++11',input,'-o',exe],{encoding:'utf8'});if(build.status!==0)throw Error(build.stderr);
  const result=cp.spawnSync(exe,[],{encoding:'utf8'});if(result.status!==0)throw Error(result.stderr);
  return Object.fromEntries(result.stdout.split('PREVIEW_').slice(1).map(s=>{const i=s.indexOf('\n');return[s.slice(0,i).trim(),s.slice(i+1)];}));
};

if(require.main===module){
  const assert=require('node:assert/strict');
  const single=module.exports({kind:'focuser',version:9,count:1});
  assert(!single.Select.includes("id='foc1_sel'"),'单调焦器不显示多设备选择按钮');
  assert(single.Select.includes('foc_sel'),'保留当前设备状态');
  assert(!single.Slewing.includes('sws-accessory-stop'),'旧版调焦器不增加不支持的独立停止按钮');
  assert(!single.Slewing.includes('foc_rate_vf'),'旧版调焦器不增加不支持的速率设置');
  assert(single.Slewing.includes("fp(event,'in')")&&single.Slewing.includes("fr(event,'in')"),'旧版仍保留按住移动与松开停止');
  const basic=module.exports({kind:'rotator',derotator:false});
  assert.equal(basic.DeRotator,'','不支持消旋时不生成消旋卡片');
  assert(basic.Home.includes("s('rot','home')")&&basic.Home.includes("s('rot','reset')"),'保留设备参考位置命令');
  console.log('PASS: actual accessory renderers, single focuser, legacy version, optional derotator and reference commands');
}
