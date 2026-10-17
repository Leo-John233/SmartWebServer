const testOutput = require('./test-output.cjs');
// 运行真实编码器页面和参数处理，避免预览把两轴表单拼成同一轴
const fs = require('node:fs'),
  path = require('node:path'),
  cp = require('node:child_process');
module.exports = ({ absolute = false } = {}) => {
  const root = path.join(__dirname, '../SmartWebServer/src'),
    read = (p) => fs.readFileSync(path.join(root, p), 'utf8'),
    clean = (s) => s.replace(/^\s*#include.*$/gm, '').replace(/^#pragma once.*$/gm, '');
  let code = `#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <cassert>
#include <iostream>
#define ON 1
#define OFF 0
#define ENCODERS ON
${absolute ? '#define ENC_ABSOLUTE' : ''}
#define AXIS1_ENCODER_TICKS_DEG 8000
#define AXIS2_ENCODER_TICKS_DEG 9000
#define AXIS1_ENCODER_REVERSE OFF
#define AXIS2_ENCODER_REVERSE ON
#define AXIS1_ENCODER_DIFF_LIMIT_TO 60
#define AXIS2_ENCODER_DIFF_LIMIT_TO 90
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define DISPLAY_SPECIAL_CHARS ON
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define CONTENT_LENGTH_UNKNOWN -1
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
struct String:std::string{using std::string::string;String(){}String(std::string s):std::string(s){}void concat(const char*s){append(s);}void concat(const String&s){append(s);}bool equals(const char*s){return *this==s;}float toFloat(){return atof(c_str());}};
const char*EmptyStr="";
struct Web{std::map<std::string,String>args;std::string output;String arg(const char*k){return args[k];}void sendContentAndClear(String&s){output+=s;s.clear();}void sendContent(const char*s){output+=s;}void setContentLength(int){}void sendHeader(const char*,const char*){}void send(int,const char*,String){}}www;
struct Axis{float ticksPerDeg;int reverse;long diffTo;};
struct Settings{Axis axis1={8000,OFF,60},axis2={9000,ON,90};bool autoSync=false;};
struct Encoders{Settings settings;int from=0,to=0,origin=0;void syncFromOnStep(){from++;}void syncToOnStep(){to++;}void originFromOnStep(){origin++;}}encoders;
struct Storage{int writes=0;Storage&kv(){return *this;}template<class T>void put(const char*,const T&){writes++;}}storage;
Storage&nv(){return storage;}
struct Convert{bool atoi2(char*s,int16_t*out){*out=atoi(s);return true;}void stripNumericStr(char*){}}convert;
char*dtostrf(double v,int width,int precision,char*out){sprintf(out,"%*.*f",width,precision,v);return out;}
struct State{const char*encAngleAxis1Str="120.0",*encAngleAxis2Str="45.0",*angleAxis1Str="120.0",*angleAxis2Str="45.0",*diffAngleAxis1Str="0",*diffAngleAxis2Str="0";unsigned long lastMountPageLoadTime=0;void updateMount(bool){}}state;
struct Status{bool onStepFound=true;}status;
struct Serial{void setTimeout(int){}}SERIAL_ONSTEP;
struct Device{void serialRecvFlush(){}}onStep;
int webTimeout=200;unsigned long millis(){return 0;}
String keyValueString(String key,String value){return key+"|"+value+"\\n";}
String keyValueToggleBoolSelected(String on,String off,bool b){return on+"|"+(b?"selected":"unselected")+"\\n"+off+"|"+(b?"unselected":"selected")+"\\n";}
`;
  code +=
    require('./locale-source.cjs')() +
    '\n' +
    require('./page-support.cjs')() +
    '\n' +
    clean(read('pages/htmlHeaders.h')) +
    '\n' +
    clean(read('pages/htmlMessages.h')) +
    '\n' +
    clean(read('pages/htmlScripts.h')) +
    '\n' +
    clean(read('pages/Page.h')) +
    '\nvoid pageHeader(int){}bool servePageShell(int){return false;}\n';
  for (const f of ['AxisTile', 'SyncTile', 'Encoders'])
    code += clean(read('pages/encoders/' + f + '.h')) + '\n';
  for (const f of ['AxisTile', 'SyncTile', 'Encoders'])
    code += clean(read('pages/encoders/' + f + '.cpp')) + '\n';
  code += `int main(){handleEncoders();std::cout<<www.output;
 www.args={{"a2cpd","12345"},{"a2rev","0"},{"a2ths","120"}};encAxisTileGet();assert(encoders.settings.axis2.ticksPerDeg==12345);assert(encoders.settings.axis2.reverse==OFF);assert(encoders.settings.axis2.diffTo==120);assert(encoders.settings.axis1.ticksPerDeg==8000);
 www.args={{"revert","2"}};encAxisTileGet();assert(encoders.settings.axis2.ticksPerDeg==9000);assert(encoders.settings.axis2.reverse==ON);assert(encoders.settings.axis2.diffTo==90);
 www.args={{"enc","o2e"}};syncTileGet();assert(encoders.from==1);www.args={{"enc","e2o"}};syncTileGet();assert(encoders.to==1);www.args={{"enc","on"}};syncTileGet();assert(encoders.settings.autoSync);www.args={{"enc","off"}};syncTileGet();assert(!encoders.settings.autoSync);
 }`;
  const dir = fs.mkdtempSync(path.join(testOutput, 'sws-encoders-')),
    src = path.join(dir, 'encoder.cpp'),
    exe = path.join(dir, 'encoder-render.exe');
  fs.writeFileSync(src, code);
  cp.execFileSync(process.env.CXX || 'g++', ['-std=c++11', src, '-o', exe], { stdio: 'pipe' });
  const html = cp.execFileSync(exe, [], { encoding: 'utf8' }).replace(/\r/g, ''),
    start = html.indexOf("<div class='b'>") + "<div class='b'>".length,
    end = html.lastIndexOf('</div></body></html>');
  return html.slice(start, end);
};
if (require.main === module) {
  const assert = require('node:assert/strict'),
    body = module.exports();
  for (const name of ['a1cpd', 'a1rev', 'a1ths', 'a2cpd', 'a2rev', 'a2ths'])
    assert(body.includes("name='" + name + "'"));
  assert(!body.includes("s('enc','zro')"));
  assert(module.exports({ absolute: true }).includes("s('enc','zro')"));
  console.log(
    'PASS: actual encoder renderer, distinct axis parameters, sync, defaults and optional absolute origin',
  );
}
