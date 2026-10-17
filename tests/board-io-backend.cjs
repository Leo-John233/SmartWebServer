const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const assert = require('node:assert/strict');
const root = path.join(__dirname,'..');
const clean = (file) => fs.readFileSync(path.join(root,'src',file),'utf8').replace(/^\s*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');

// 编译实际策略、GPIO 适配器和设置接口；只替换串口、引脚、时钟和存储
module.exports = function({preview=false,language=process.env.SWS_TEST_LANGUAGE||'cn',
  brakeEnabled=/^#define\s+RA_BRAKE_PROTECTION\s+ON\b/m.test(fs.readFileSync(path.join(root,'Config.h'),'utf8')),
  activityAvailable=true}={}) {
  const work=fs.mkdtempSync(path.join(output,'board-io-'));
  const templates=require('./page-templates.cjs')(root,language);
  let code=`
#include <string>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <stdint.h>
#define SWS_ACTIVITY_LED_PIN ${activityAvailable ? '2' : 'OFF'}
#define SWS_RA_BRAKE_PIN 5
#define RA_BRAKE_PROTECTION ${brakeEnabled ? 'ON' : 'OFF'}
#define AXIS1_ENCODER OFF
#define AXIS2_ENCODER OFF
#define DISPLAY_RESET_CONTROLS OFF
#define FWU -204
#define LOW 0
#define HIGH 1
#define OUTPUT 1
#define OFF -1
#define ON 1
#define SD_TRUE 2
#define LED_STATUS_ON_STATE HIGH
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define HTTP_GET 1
#define HTTP_POST 3
#define OPERATIONAL_MODE 2
#define WIFI 2
#define PROGMEM
uint32_t now=0;
uint32_t millis(){return now;}
std::map<int,int>pins;std::vector<int>pinEvents;
void digitalWrite(int pin,int value){pins[pin]=value;pinEvents.push_back(pin*10+value);}
void pinMode(int pin,int){assert(pins.count(pin));}
struct String:std::string{
  using std::string::string;
  void concat(const char*s){append(s);}void concat(const String&s){append(s);}
  void replace(const char*from,const char*to){size_t p=0;while((p=find(from,p))!=npos){std::string::replace(p,strlen(from),to);p+=strlen(to);}}
};
struct Status{bool onStepFound=false;int mountFound=0;}status;
struct Device{
  int calls=0,stops=0,trackingStops=0;bool trackingSuccess=true;bool received=true,stopSuccess=true;const char*reply="ST,,,,,,,";std::function<void()>stopHook;
  bool processCommand(const char*command,char*out,int timeout){
    assert(timeout==200);
    if(!strcmp(command,":Q#")){stops++;if(stopHook)stopHook();pinEvents.push_back(900);out[0]=0;return stopSuccess;}
    if(!strcmp(command,":Td#")){trackingStops++;strcpy(out,trackingSuccess?"1":"0");return received;}
    assert(!strcmp(command,":GXU1#"));calls++;strcpy(out,reply);strcat(out,"#");return received;
  }
}onStep;
struct KvPartition {
  enum class Status{Ok,Error,NotFound};
  uint8_t stored=3,storedFault=0;bool fail=false,missing=false;
  Status get(const char*key,uint8_t&out){if(missing)return Status::NotFound;if(!getOrInit(key,out))return Status::Error;return Status::Ok;}
  bool getOrInit(const char*key,uint8_t&out){assert(!strcmp(key,"BOARD_IO_SETTINGS")||!strcmp(key,"RA_BRAKE_FAULT"));out=!strcmp(key,"BOARD_IO_SETTINGS")?stored:storedFault;return !fail;}
  Status put(const char*key,uint8_t value){assert(!strcmp(key,"BOARD_IO_SETTINGS")||!strcmp(key,"RA_BRAKE_FAULT"));if(fail)return Status::Error;if(!strcmp(key,"BOARD_IO_SETTINGS"))stored=value;else storedFault=value;return Status::Ok;}
};
struct Nv{KvPartition store;KvPartition&kv(){return store;}}storage;
Nv&nv(){return storage;}
struct Tasks{int added=0;bool fail=false;int failAt=0;bool add(unsigned long,int,bool,int,void(*)(),const char*){added++;return !fail&&added!=failAt;}}tasks;
struct Web{
  int methodValue=HTTP_GET,code=0;std::string output;std::map<std::string,String>args;
  std::function<void()> handler;
  int method(){return methodValue;}String arg(const char*k){return args[k];}
  void sendHeader(const char*,const char*){}
  void send(int c,const char*,const String&s){code=c;output=s;}
  void on(const char*,std::function<void()>fn){handler=fn;}
}www;
`;
  code+=clean('libApp/boardIo/Config.h')+'\n'+clean('libApp/boardIo/ProtectionPort.h')+'\n'+clean('libApp/boardIo/CommandGate.h')+'\n'+clean('libApp/boardIo/Policy.h')+'\n'+clean('libApp/boardIo/OnStepProtection.h')+'\n'+clean('libApp/boardIo/BoardIo.h')+'\n';
  code+=clean('libApp/boardIo/Policy.cpp')+'\n'+clean('libApp/boardIo/OnStepProtection.cpp')+'\n'+clean('libApp/boardIo/BoardIo.cpp')+'\n';
  code+=require('./locale-source.cjs')(language)+'\n';
  for(const [name,value]of Object.entries(templates['boardIo/View.h']))code+='const char '+name+'[]='+JSON.stringify(value)+';\n';
  code+=clean('pages/boardIo/BoardIo.cpp')+'\n'+clean('pages/Request.cpp')+'\n';
  code+=preview?`int main(){boardIo.initPins();boardIo.begin();status.onStepFound=true;status.mountFound=SD_TRUE;now=10;boardIo.pollDriver();now=600;boardIo.pollDriver();String data;boardIoTile(data);std::cout<<data;}`:!brakeEnabled?`
int main(){
  boardIo.initPins();assert(pins[5]==HIGH&&pinEvents[0]==51);
  boardIo.begin();assert(!boardIo.brakeAvailable()&&boardIo.ownsPin(5));
  assert(tasks.added==${activityAvailable ? '1' : '0'}); // 关闭自动保护后没有抱闸采样任务
  status.onStepFound=true;status.mountFound=SD_TRUE;
  for(now=100;now<=2100;now+=250){boardIo.pollDriver();boardIo.pollDriver();boardIo.tick();assert(pins[5]==HIGH);}
  assert(onStep.calls==0); // 旧 NV 中保存的开启状态和正常反馈均不能使 IO5 拉低
  for(const char*fault:{"ST,OA,,,,,,","ST,,OB,,,,,","ST,,,GA,,,,","ST,,,,GB,,,","ST,,,,,OT,,","ST,,,,,,PW,","ST,,,,,,,GF"})boardIo.pollDriver();
  assert(onStep.stops==0); // 编译关闭后停止和抱闸自动保护一起停用
  assert(boardIo.save(true,true)==${activityAvailable ? 'true' : 'false'});assert(pins[5]==HIGH);
  status.onStepFound=false;boardIo.tick();assert(pins[5]==HIGH);
  String data;boardIoTile(data);assert(data.find("sws-io-protection")==std::string::npos);
  assert((data.find("sws-io-activity")!=std::string::npos)==${activityAvailable ? 'true' : 'false'});
  data.clear();boardIoTileAjax(data);assert(data.find("sws-io-brake-state|")==std::string::npos);
  www.methodValue=HTTP_POST;www.args={{"setting","protection"},{"enabled","1"}};handleBoardIo();assert(www.code==409&&pins[5]==HIGH);
  www.args["enabled"]="0";handleBoardIo();assert(www.code==409&&pins[5]==HIGH);
  for(uint8_t stored: {0,1,2,3,255}){
    nv().store.stored=stored;
    BoardIo reboot;reboot.initPins();reboot.begin();reboot.tick();assert(pins[5]==HIGH);
  }
  nv().store.fail=true;BoardIo noStorage;noStorage.initPins();noStorage.begin();noStorage.tick();assert(pins[5]==HIGH);
  tasks.fail=true;BoardIo failed;failed.initPins();failed.begin();failed.tick();assert(pins[5]==HIGH);
  for(int event:pinEvents)assert(event!=50); // 从初始化到整个测试结束从未输出 LOW
  std::cout<<"PASS: config OFF hides brake controls and status, refuses API changes, skips driver queries and holds IO5 HIGH with legacy NV and healthy feedback\\n";
}`:`

void advance(uint32_t time){now=time;boardIo.tick();}
void sample(uint32_t time,const char *reply="ST,,,,,,,"){now=time;onStep.reply=reply;boardIo.pollDriver();}
void start(){nv().store.stored=3;nv().store.storedFault=0;boardIo=BoardIo();boardIo.initPins();boardIo.begin();sample(now+250);sample(now+500);assert(pins[5]==HIGH&&boardIo.motionAllowed());}
int main(){
  boardIo.initPins();assert(pins[5]==LOW&&pinEvents[0]==50);boardIo.begin();
  status.onStepFound=true;status.mountFound=SD_TRUE;
  advance(3000);assert(pins[5]==LOW&&!boardIo.motionAllowed());
  sample(3010);sample(3510);assert(pins[5]==HIGH&&boardIo.motionAllowed());
  sample(3760);assert(pins[5]==HIGH&&onStep.stops==0);
  for(const char*fault:{"ST,OA,,,,,,","ST,,OB,,,,,","ST,,,GA,,,,","ST,,,,GB,,,","ST,,,,,OT,,","ST,,,,,,PW,","ST,,,,,,,GF"}){
    start();const int stops=onStep.stops,tracking=onStep.trackingStops;
    onStep.stopHook=[](){boardIo.tick();assert(!boardIo.save(true,false));assert(pins[5]==HIGH);};
    sample(now+250,fault);onStep.stopHook=nullptr;
    assert(pins[5]==HIGH&&onStep.stops==stops+1&&onStep.trackingStops==tracking+1&&!boardIo.motionAllowed());
    assert(nv().store.storedFault==uint8_t(BrakeFault::Driver));
    sample(now+250,fault);assert(pins[5]==HIGH);sample(now+250,fault);assert(pins[5]==LOW);
    sample(now+250,"0");assert(pins[5]==LOW);sample(now+250);sample(now+500);
    assert(pins[5]==LOW&&!boardIo.motionAllowed()&&onStep.stops==stops+1);
    assert(boardIo.resetFault());assert(pins[5]==HIGH&&boardIo.motionAllowed()&&nv().store.storedFault==0);
    assert(onStep.trackingStops==tracking+1); // 复位只解除锁定，不恢复旧跟踪
  }
  // 超时和失联保持当前电平，不重复发 STOP，也不使用延时抱闸
  start();int stops=onStep.stops;advance(now+1001);assert(pins[5]==HIGH&&!boardIo.motionAllowed()&&onStep.stops==stops+1);
  for(int i=0;i<10;i++)sample(now+250,",,,,,,,");assert(pins[5]==HIGH&&onStep.stops==stops+1);
  status.onStepFound=false;advance(now+2000);assert(pins[5]==HIGH);status.onStepFound=true;
  assert(boardIo.retryStop()&&onStep.stops==stops+2);sample(now+250);sample(now+250);assert(pins[5]==LOW);
  status.onStepFound=false;advance(now+2000);assert(pins[5]==LOW);status.onStepFound=true;
  // 持久锁定跨重启、刷新、开关切换保留，重启本身不重复停止
  stops=onStep.stops;BoardIo reboot;reboot.initPins();reboot.begin();assert(!reboot.motionAllowed()&&pins[5]==LOW&&onStep.stops==stops);
  assert(reboot.policy().faultReason()!=BrakeFault::None);
  assert(reboot.save(true,false)&&reboot.save(true,true));assert(reboot.policy().faultReason()!=BrakeFault::None);
  // 关闭保护先停止，关闭期间继续采样，重新打开不会自行解除锁定
  start();stops=onStep.stops;assert(boardIo.save(true,false));assert(pins[5]==HIGH&&onStep.stops==stops+1);
  sample(now+250);sample(now+250);assert(pins[5]==LOW);assert(boardIo.save(true,true));
  sample(now+250);sample(now+500);assert(pins[5]==LOW);assert(boardIo.resetFault());assert(pins[5]==HIGH);
  // Td 被拒绝时不把 Q 当作完整停止；人工重试才能启动下一次停止
  onStep.trackingSuccess=false;sample(now+250,"ST,,,,,,,GF");sample(now+250);sample(now+500);
  assert(pins[5]==HIGH&&boardIo.brakeState()==BrakeState::StopFailed);
  onStep.trackingSuccess=true;assert(boardIo.retryStop());sample(now+250);sample(now+250);assert(pins[5]==LOW);
  // 保存失败不清除故障；恢复存储后才允许复位
  sample(now+500);nv().store.fail=true;assert(!boardIo.resetFault()&&pins[5]==LOW);
  nv().store.fail=false;assert(boardIo.resetFault()&&pins[5]==HIGH);
  nv().store.fail=true;sample(now+250,"ST,,,,,,,GF");assert(!boardIo.faultSaved()&&pins[5]==HIGH);
  nv().store.fail=false;sample(now+1000);assert(boardIo.faultSaved());sample(now+250);assert(pins[5]==LOW);
  // 新增网页操作保持独立接口，不接受不满足条件的复位
  www.methodValue=HTTP_POST;www.args={{"action","reset"}};handleBoardIo();assert(www.code==409);
  www.args={{"action","retry-stop"}};handleBoardIo();assert(www.code==200);
  www.methodValue=HTTP_GET;www.args.clear();handleBoardIo();assert(www.output.find("trackingOffAccepted")!=std::string::npos);
  www.methodValue=HTTP_POST;www.args={{"action","unknown"}};handleBoardIo();assert(www.code==400);
  // 所有旧协议异常都不能伪装为有效反馈
  for(const char*reply:{"0","", "garbage", "ST,OA,OB,GA,GB,OT,PW,GF", "ST,,,,,,,extra", "ST,,,,,,"}){
    ProtectionFeedback f;assert(!OnStepProtection::decodeDriver(reply,f)&&!f.driverValid);
  }
  ProtectionFeedback f;assert(OnStepProtection::decodeDriver("ST,,,,,,,",f)&&f.driverValid&&f.standstill&&!f.driverFault);
  String data;boardIoTile(data);assert(data.find("data-action='reset'")!=std::string::npos);
  // 输出任务或采样任务任一启动失败，都保持上电 LOW 并拦截运动
  for(int which:{1,2}){tasks.failAt=tasks.added+which;BoardIo failed;failed.initPins();failed.begin();assert(pins[5]==LOW&&!failed.motionAllowed()&&failed.brakeState()==BrakeState::StartFailed);tasks.failAt=0;}
  // 首次写入默认值失败也必须锁定，不能把缺少记录误当成保存成功
  nv().store.missing=true;nv().store.fail=true;BoardIo noStorage;noStorage.initPins();noStorage.begin();
  assert(pins[5]==LOW&&!noStorage.motionAllowed()&&noStorage.policy().faultReason()==BrakeFault::Storage);
  nv().store.missing=false;nv().store.fail=false;nv().store.storedFault=0;
  // 替换采样适配器不需要修改策略、GPIO 或网页模块
  struct Port:ProtectionPort{
    int reads=0,stops=0;
    ProtectionFeedback sample()override{reads++;ProtectionFeedback f;f.sampledAt=now;f.driverValid=f.standstill=true;return f;}
    ProtectionStopResult stop()override{stops++;ProtectionStopResult r;r.haltSent=r.trackingOffAccepted=true;return r;}
  }port;
  BoardIo alternate;assert(alternate.setProtectionPort(port));alternate.initPins();alternate.begin();
  now+=250;alternate.pollDriver();now+=500;alternate.pollDriver();assert(pins[5]==HIGH&&port.reads==2);
  assert(!alternate.setProtectionPort(port));assert(alternate.save(true,false)&&pins[5]==HIGH&&port.stops==1);
  now+=250;alternate.pollDriver();now+=250;alternate.pollDriver();assert(pins[5]==LOW&&port.stops==1);
  std::cout<<"PASS: actual driver adapter, Q/Td, GPIO order and level hold, storage/reboot, switch OFF sampling, reset/retry API and task failures\\n";
}
`;
  const file=path.join(work,'test.cpp'),exe=path.join(work,'test.exe');fs.writeFileSync(file,code);
  cp.execFileSync(process.env.CXX||'g++',['-std=c++11','-Wall','-Wextra',file,'-o',exe],{stdio:['pipe','pipe','pipe']});
  const html=cp.execFileSync(exe,[],{encoding:'utf8'});
  if(preview)return html;
  // 展开实际 pinmap 和模块配置，验证物理引脚及冲突诊断，而非检查源码字符串
  const pinSource=`#define OFF -1\n#define ON -2\n#define WIFI 2\n#define OPERATIONAL_MODE WIFI\n#define ESP8266\n#define FWU -204\n#define DISPLAY_RESET_CONTROLS OFF\n#define AXIS1_ENCODER OFF\n#define AXIS2_ENCODER OFF\n`+
    '#include "'+path.join(root,'src/pinmaps/Pins.Esp8266.h').replaceAll('\\','/')+'"\n'+
    '#include "'+path.join(root,'src/libApp/boardIo/Config.h').replaceAll('\\','/')+'"\n'+
    'static_assert(RA_BRAKE_PIN==5 && SWS_RA_BRAKE_PIN==5,"IO5 brake mapping");\n'+
    'static_assert(AXIS2_ENCODER_A_PIN==OFF && VGPIO_PIN_2==OFF,"IO5 not reused by encoder or virtual GPIO");\n'+
    'static_assert(SWS_ACTIVITY_LED_PIN==2,"IO2 activity LED mapping");\n'+
    'static_assert(RA_BRAKE_PROTECTION==OFF,"missing feature config defaults to OFF");\n';
  const pinFile=path.join(work,'pins.cpp');fs.writeFileSync(pinFile,pinSource);
  cp.execFileSync(process.env.CXX||'g++',['-std=c++11','-fsyntax-only',pinFile]);
  fs.writeFileSync(pinFile,pinSource.replace('#define AXIS2_ENCODER OFF','#define AXIS2_ENCODER ON'));
  const conflict=cp.spawnSync(process.env.CXX||'g++',['-std=c++11','-fsyntax-only',pinFile],{encoding:'utf8'});
  assert.notEqual(conflict.status,0);assert(conflict.stderr.includes('Axis 2 encoder A is disabled'));
  console.log(html.trim());
};
if(require.main===module){module.exports();module.exports({brakeEnabled:false});module.exports({brakeEnabled:false,activityAvailable:false});}
