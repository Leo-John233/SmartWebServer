// 编译实际赤道仪卡片生成函数，模拟设备响应而不重新拼接表单
const fs=require('node:fs'),path=require('node:path'),os=require('node:os'),cp=require('node:child_process');
module.exports=()=>{
 const root=path.join(__dirname,'../SmartWebServer'),read=p=>fs.readFileSync(path.join(root,p),'utf8'),clean=s=>s.replace(/^\s*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
 const files=['Site','Library','HomePark','Align','Limits','Pec','Goto'];
 let code=`#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define sprintf_P sprintf
#define ON 1
#define DISPLAY_SPECIAL_CHARS ON
#define OFF 0
#define MT_GEM 1
#define MT_FORK 2
#define MT_ALTAZM 3
#define HOME_OFFSET_RANGE_AXIS1 3600
#define HOME_OFFSET_RANGE_AXIS2 3600
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
using byte=unsigned char;
struct String:std::string {using std::string::string;void concat(const char*s){append(s);}};
struct Web {std::string output;void sendContentAndClear(String&s){output+=s;s.clear();}} www;
struct Status {bool hasHomeSense=true,pecEnabled=true,inGoto=false,meridianFlips=true;int mountType=MT_GEM,alignMaxStars=9;int getVersionMajor(){return 10;}int getVersionMinor(){return 28;}bool hasMeridianFlipHomeModes(){return true;}} status;
struct State {const char *dateStr="10/16/26",*timeStr="10:08:38",*lastStr="19:52:26",*latitudeStr="+31°00′00″",*longitudeStr="-121°00′00″",*alignProgress="",*alignLrStr="0″",*alignUdStr="0″",*pierSideStr="中天翻转，由西向东",*indexAzmStr="120°",*indexRaStr="23h57m00s",*targetRaStr="05h35m17s",*indexAltStr="45°",*indexDecStr="+90°00′00″",*targetDecStr="-05°23′28″",*slewSpeedStr="3°/s";double latitude=31;} state;
struct Device {const char* commandString(const char*){return "126";}bool command(const char*s,char*out){const char*v="0";if(!strcmp(s,":GtH#"))v="+31*00:00";if(!strcmp(s,":GgH#"))v="-121*00:00";if(!strcmp(s,":GG#"))v="-08:00";if(!strcmp(s,":h?#"))v="1,-120,-240";if(!strcmp(s,":Go#"))v="80";if(!strcmp(s,":Gh#"))v="-10";if(!strcmp(s,":GXE7#"))v="12800";strcpy(out,v);return true;}} onStep;
struct Convert {void stripNumericStr(char*s){int n=atoi(s);sprintf(s,"%d",n);}} convert;
const char*getHomeParkStateStr(){return "待机";}
`;
 code+=read('src/locales/Strings_cn.h')+'\n'+clean(read('src/pages/htmlHeaders.h'))+'\n';
 for(const f of files)code+=clean(read('src/pages/mount/'+f+'Tile.h'))+'\n';
 for(const f of files){let s=clean(read('src/pages/mount/'+f+'Tile.cpp'));s=s.split('// use Ajax')[0];code+=s+'\n';}
 code+='int main(){String data;';for(const f of files)code+='std::cout<<"PREVIEW_'+f+'\\n";www.output.clear();'+f[0].toLowerCase()+f.slice(1)+'Tile(data);std::cout<<www.output<<data;data.clear();';code+='}';
 const dir=fs.mkdtempSync(path.join(os.tmpdir(),'sws-mount-')),src=path.join(dir,'test.cpp'),exe=path.join(dir,'test.exe');fs.writeFileSync(src,code);
 const build=cp.spawnSync(process.env.CXX||'g++',['-std=c++11',src,'-o',exe],{encoding:'utf8'});if(build.status!==0)throw Error(build.stderr);
 const run=cp.spawnSync(exe,[],{encoding:'utf8'});if(run.status!==0)throw Error(run.stderr);
 return Object.fromEntries(run.stdout.split('PREVIEW_').slice(1).map(s=>{let i=s.indexOf('\n');return[s.slice(0,i).trim(),s.slice(i+1)]}));
};
