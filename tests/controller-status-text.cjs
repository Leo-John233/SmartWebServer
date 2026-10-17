const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const root = path.join(__dirname, '../src');
const original = fs.readFileSync(path.join(root, 'libApp/status/StateController.cpp'), 'utf8');
const work = fs.mkdtempSync(path.join(output, 'controller-text-'));
const begin = original.indexOf('static void appendControllerText(');
const end = original.indexOf('void State::axisStatusUpdate(');
const source = original.slice(begin, end);
// 用全部实际译文、所有故障位组合和带边界哨兵的目标缓冲区运行真实格式化函数。
for (const language of ['cn', 'en', 'ca', 'de', 'es', 'fr', 'it', 'jp', 'ro']) {
  const program = `
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdint>
#define DISPLAY_WEATHER 0
#define DISPLAY_INTERNAL_TEMPERATURE 0
#define ON 1
#define WIFI 1
#define OPERATIONAL_MODE 0
#define ERR_NONE 0
#define Y
unsigned long millis(){return 1;}
struct Status {
  bool onStepFound=true; int lastError=1;
  int getVersionMajor(){return 10;}
  void getLastErrorMessage(char *out,size_t size){memset(out,'x',size-1);out[size-1]=0;}
}status;
struct Device {bool command(const char*,char*){return false;}}onStep;
struct Channel {bool openLoad=false,shortToGround=false;};
struct Driver {bool valid=true,communicationFailure=false,standstill=false,overTemperature=false,overTemperaturePreWarning=false,fault=false;Channel outputA,outputB;};
struct State {
  char before[16]; Driver driver[9];
  char siteTemperatureStr[16],sitePressureStr[16],siteHumidityStr[16],siteDewPointStr[16];
  char controllerTemperatureStr[16],lastErrorStr[80],workLoadStr[20],signalStrengthStr[20],driverStatusStr[9][40];
  char after[16]; unsigned long lastControllerPageLoadTime=0;
  void updateController(bool); void axisStatusUpdate(){}
};
${require('./locale-source.cjs')(language)}
${source}
int main(){
  State state{};memset(state.before,0x5a,16);memset(state.after,0x6b,16);
  for(unsigned flags=0;flags<512;flags++){
    for(auto &driver:state.driver){
      driver.communicationFailure=flags&1;driver.standstill=flags&2;
      driver.outputA.openLoad=flags&4;driver.outputB.openLoad=flags&8;
      driver.outputA.shortToGround=flags&16;driver.outputB.shortToGround=flags&32;
      driver.overTemperature=flags&64;driver.overTemperaturePreWarning=flags&128;driver.fault=flags&256;
    }
    state.updateController(true);
    assert(state.lastErrorStr[79]==0);
    for(auto &text:state.driverStatusStr)assert(text[39]==0);
    for(int i=0;i<16;i++){assert(state.before[i]==0x5a);assert(state.after[i]==0x6b);}
  }
  struct {char text[80];char guard[16];}bounded{};memset(bounded.guard,0x7c,16);
  for(int n=0;n<20;n++)appendControllerText(bounded.text,sizeof(bounded.text),L_SHUTDOWN_OVER " 150C, " L_PRE_WARNING " >120C, ");
  assert(bounded.text[79]==0);for(char c:bounded.guard)assert(c==0x7c);
}`;
  const file = path.join(work, language + '.cpp'), exe = path.join(work, language + '.exe');
  fs.writeFileSync(file, program);
  for (const [command, args] of [[process.env.CXX || 'g++', ['-std=c++11', '-fstack-protector-all', file, '-o', exe]], [exe, []]]) {
    const result = cp.spawnSync(command, args, {encoding: 'utf8'});
    if (result.error) throw result.error;
    if (result.status !== 0) throw Error(language + '\n' + result.stdout + result.stderr);
  }
}
console.log('PASS: 9 locales, 512 driver fault combinations, long error messages, bounded text and stack protection');
