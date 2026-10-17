const testOutput = require('./test-output.cjs');
// 执行完整辅助页面与状态输出，设备能力采用明确的模拟通道组合
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process');
const defaults=[
 {name:'Aux1',purpose:1,values:[1,0,0,0],voltage:12.1,current:.1},
 {name:'Aux2',purpose:2,values:[128,0,0,0],voltage:12.1,current:.4},
 {name:'Aux3',purpose:3,values:[1,1.5,4,3.2],voltage:12.1,current:.8},
 {name:'Aux4',purpose:4,values:[0,30,5,10]},
];
module.exports=({features=defaults,sourceRef}={})=>{
 const root=path.join(__dirname,'../src'),read=p=>fs.readFileSync(path.join(root,p),'utf8');
 const clean=s=>s.replace(/^\s*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
 const fixture=Array.from({length:8},(_,i)=>({name:'Aux'+(i+1),purpose:0,values:[0,0,0,0],...features[i]}));
 const scalar=n=>Number.isFinite(n)?String(n):'NAN';
 const misc=read('libApp/misc/Misc.cpp');
 let code=`#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#define PROGMEM
#define ON 1
#define OFF 0
#define METRIC 1
#define UNITS METRIC
#define DISPLAY_SPECIAL_CHARS ON
#define AJAX_PAGE_LAZY_GET_MS 200
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 200
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 3000
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define SD_TRUE 1
#define SWITCH 1
#define ANALOG_OUTPUT 2
#define DEW_HEATER 3
#define INTERVALOMETER 4
#define COVER_SWITCH 7
#define CONTENT_LENGTH_UNKNOWN -1
#define celsiusToNativeRelative(t) (t)
#define nativeToCelsiusRelative(t) (t)
#define TEMPERATURE_UNITS_ABV "C"
using std::isnan;
char*dtostrf(double v,int width,int precision,char*out){sprintf(out,"%*.*f",width,precision,v);return out;}
void sprintF(char*out,const char*format,double value){sprintf(out,format,value);}
struct String:std::string{using std::string::string;String(){}String(std::string s):std::string(s){}void concat(const char*s){append(s);}void concat(const String&s){append(s);}bool equals(const char*s){return *this==s;}long toInt(){return atol(c_str());}float toFloat(){return atof(c_str());}};
const char*EmptyStr="";
struct Web{std::string output;std::map<std::string,String>args;void sendContentAndClear(String&s){output+=s;s.clear();}void sendContent(const char*s){output+=s;}void setContentLength(int){}void sendHeader(const char*,const char*){}void send(int,const char*,String){}String arg(const char*k){return args[k];}}www;
struct Feature{const char*name;int purpose;float values[4],voltage,current;};
Feature features[8]={${fixture.map(f=>'{'+JSON.stringify(f.name)+','+f.purpose+',{'+f.values.map(scalar).join(',')+'},'+scalar(f.voltage)+','+scalar(f.current)+'}').join(',')}};
struct Status{int auxiliaryFound=${fixture.some(f=>f.purpose>0)?1:0};bool onStepFound=true;}status;
struct State{int selected=0;unsigned long lastAuxPageLoadTime=0;void updateAuxiliary(bool,bool){}void selectFeature(int n){selected=n;}const char*featureName(){return features[selected].name;}int featurePurpose(){return features[selected].purpose;}int featureValue1(){return features[selected].values[0];}float featureValue2(){return features[selected].values[1];}float featureValue3(){return features[selected].values[2];}float featureValue4(){return features[selected].values[3];}float featureVoltage(){return features[selected].voltage;}float featureCurrent(){return features[selected].current;}}state;
struct Serial{void setTimeout(int){}}SERIAL_ONSTEP;
struct Device{void serialRecvFlush(){}void commandBool(const char*){}}onStep;
int webTimeout=200;
unsigned long millis(){return 0;}
void handleNotFound(){www.output="NOT_FOUND";}
`;
 code+=require('./locale-source.cjs')()+'\n'+require('./page-support.cjs')()+'\n'+clean(read('pages/htmlHeaders.h'))+'\n'+clean(read('pages/htmlScripts.h'))+'\n'+clean(read('pages/htmlMessages.h'))+'\n'+clean(read('pages/Page.h'))+'\n';
 code+='void pageHeader(int){}bool servePageShell(int){return false;}\n';
 code+=misc.slice(misc.indexOf('uint8_t timeToByte('),misc.indexOf('void localeTemperature('))+'\n';
 const auxiliarySource=sourceRef?cp.execFileSync('git',['show',sourceRef+':src/pages/auxiliary/Auxiliary.cpp'],{encoding:'utf8'}):read('pages/auxiliary/Auxiliary.cpp');
 const auxiliaryHeader=sourceRef?cp.execFileSync('git',['show',sourceRef+':src/pages/auxiliary/Auxiliary.h'],{encoding:'utf8'}):read('pages/auxiliary/Auxiliary.h');
 code+=clean(auxiliaryHeader)+'\n'+clean(auxiliarySource)+'\n';
 code+='int main(){handleAux();std::cout<<www.output<<"\\nPREVIEW_AJAX\\n";www.output.clear();auxAjax();std::cout<<www.output<<"PREVIEW_TIMES\\n";for(int i=0;i<256;i++)std::cout<<byteToTime(i)<<"\\n";}';
 const dir=fs.mkdtempSync(path.join(testOutput,'sws-auxiliary-')),input=path.join(dir,'render.cpp'),exe=path.join(dir,'auxiliary-render.exe');fs.writeFileSync(input,code);
 cp.execFileSync(process.env.CXX||'g++',['-std=c++11',input,'-o',exe],{stdio:'pipe'});
 const [html,other]=cp.execFileSync(exe,[],{encoding:'utf8'}).replace(/\r/g,'').split('\nPREVIEW_AJAX\n'),[ajax,times]=other.split('PREVIEW_TIMES\n');
 let body='';if(html!=='NOT_FOUND'){const start=html.indexOf("<div class='b'>")+"<div class='b'>".length,end=html.lastIndexOf('</div></body></html>');if(start<"<div class='b'>".length||end<start)throw Error('辅助页面结构异常');body=html.slice(start,end);}
 return {body,ajax,features:fixture,timeValues:times.trim().split('\n').map(Number)};
};
if(require.main===module){
 const assert=require('node:assert/strict'),preview=module.exports();
 assert.equal((preview.body.match(/class='b1 sws-aux-card'/g)||[]).length,4);
 for(const key of ['x1v1','x2v1','x3v1','x3v2','x3v3','x4v1','x4v2','x4v3','x4v4'])assert(preview.body.includes(key),key+' 按通道生成');
 assert.equal((preview.body.match(/type='range'/g)||[]).length,6);
 assert(preview.ajax.includes('x2v1|50\n'));assert(preview.ajax.includes('x4v2|30\n'));assert.equal(preview.timeValues[86],30);
 const last=module.exports({features:Array.from({length:8},(_,i)=>({purpose:i===7?1:0}))});assert(last.body.includes('sw8_on'));assert(!last.body.includes('sw1_on'));
 assert.equal(module.exports({features:[]}).body,'');
 console.log('PASS: actual auxiliary page, four feature types, channel 8, disabled channels, polling and time conversion');
}
