const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const assert = require('node:assert/strict');
const root = path.join(__dirname,'..');
const clean = (file) => fs.readFileSync(path.join(root,'src',file),'utf8').replace(/^\s*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');

// 编译实际策略、GPIO 适配器和设置接口；只替换串口、引脚、时钟和存储。
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
  int calls=0,stops=0;bool received=true,stopSuccess=true;const char*reply="ST,,,,,,,";std::function<void()>stopHook;
  bool processCommand(const char*command,char*out,int timeout){
    assert(timeout==200);
    if(!strcmp(command,":Q#")){stops++;if(stopHook)stopHook();pinEvents.push_back(900);out[0]=0;return stopSuccess;}
    assert(!strcmp(command,":GXU1#"));calls++;strcpy(out,reply);strcat(out,"#");return received;
  }
}onStep;
struct KvPartition {
  enum class Status{Ok,Error};
  uint8_t stored=3;bool fail=false;
  bool getOrInit(const char*key,uint8_t&out){assert(!strcmp(key,"BOARD_IO_SETTINGS"));out=stored;return !fail;}
  Status put(const char*key,uint8_t value){assert(!strcmp(key,"BOARD_IO_SETTINGS"));if(fail)return Status::Error;stored=value;return Status::Ok;}
};
struct Nv{KvPartition store;KvPartition&kv(){return store;}}storage;
Nv&nv(){return storage;}
struct Tasks{int added=0;bool fail=false;bool add(unsigned long,int,bool,int,void(*)(),const char*){added++;return !fail;}}tasks;
struct Web{
  int methodValue=HTTP_GET,code=0;std::string output;std::map<std::string,String>args;
  std::function<void()> handler;
  int method(){return methodValue;}String arg(const char*k){return args[k];}
  void sendHeader(const char*,const char*){}
  void send(int c,const char*,const String&s){code=c;output=s;}
  void on(const char*,std::function<void()>fn){handler=fn;}
}www;
`;
  code+=clean('libApp/boardIo/Config.h')+'\n'+clean('libApp/boardIo/Policy.h')+'\n'+clean('libApp/boardIo/BoardIo.h')+'\n';
  code+=clean('libApp/boardIo/Policy.cpp')+'\n'+clean('libApp/boardIo/BoardIo.cpp')+'\n';
  code+=require('./locale-source.cjs')(language)+'\n';
  for(const [name,value]of Object.entries(templates['boardIo/View.h']))code+='const char '+name+'[]='+JSON.stringify(value)+';\n';
  code+=clean('pages/boardIo/BoardIo.cpp')+'\n'+clean('pages/Request.cpp')+'\n';
  code+=preview?`int main(){boardIo.initPins();boardIo.begin();status.onStepFound=true;status.mountFound=SD_TRUE;now=10;boardIo.driverFeedback(true,"ST,,,,,,,");now=600;boardIo.driverFeedback(true,"ST,,,,,,,");String data;boardIoTile(data);std::cout<<data;}`:!brakeEnabled?`
int main(){
  boardIo.initPins();assert(pins[5]==HIGH&&pinEvents[0]==51);
  boardIo.begin();assert(!boardIo.brakeAvailable()&&boardIo.ownsPin(5));
  assert(tasks.added==${activityAvailable ? '1' : '0'}); // 关闭自动保护后没有抱闸采样任务。
  status.onStepFound=true;status.mountFound=SD_TRUE;
  for(now=100;now<=2100;now+=250){boardIo.driverFeedback(true,"ST,,,,,,,");boardIo.pollDriver();boardIo.tick();assert(pins[5]==HIGH);}
  assert(onStep.calls==0); // 旧 NV 中保存的开启状态和正常反馈均不能使 IO5 拉低。
  for(const char*fault:{"ST,OA,,,,,,","ST,,OB,,,,,","ST,,,GA,,,,","ST,,,,GB,,,","ST,,,,,OT,,","ST,,,,,,PW,","ST,,,,,,,GF"})boardIo.driverFeedback(true,fault);
  assert(onStep.stops==0); // 编译关闭后停止和抱闸自动保护一起停用。
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
  for(int event:pinEvents)assert(event!=50); // 从初始化到整个测试结束从未输出 LOW。
  std::cout<<"PASS: config OFF hides brake controls and status, refuses API changes, skips driver queries and holds IO5 HIGH with legacy NV and healthy feedback\\n";
}`:`
void advance(uint32_t time){now=time;boardIo.tick();}
void healthy(){now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");now+=500;boardIo.driverFeedback(true,"ST,,,,,,,");}
int main(){
  boardIo.initPins();assert(pins[5]==LOW&&pinEvents[0]==50);
  advance(5000);assert(pins[5]==LOW); // 初始化及未启动时保留低电平。
  boardIo.begin();assert(tasks.added==2&&pins[5]==HIGH);
  assert(boardIo.ownsPin(2)&&boardIo.ownsPin(5)&&!boardIo.ownsPin(-1)&&!boardIo.ownsPin(4));
  assert(boardIo.brakeState()==BrakeState::Waiting);
  status.onStepFound=true;status.mountFound=SD_TRUE;
  now=5010;boardIo.pollDriver();assert(pins[5]==HIGH);
  const int calls=onStep.calls;boardIo.pollDriver();assert(onStep.calls==calls);
  advance(5510);assert(pins[5]==HIGH);boardIo.pollDriver();assert(boardIo.brakeState()==BrakeState::Released);
  char manualResponse[80]="";
  onStep.processCommand(":Q#",manualResponse,200);healthy();assert(pins[5]==HIGH&&!boardIo.policy().driverFault());
  for(const char*bad:{"0","?","OK","",",,,,,,",",,,,,,,,,","XX,,,,,,,","ST,OA,,,,,,bad","ST,OA,OB,GA,GB,OT,PW,GF","ST,OA,OB,GA,GB,OT,PW,"}){
    const int stops=onStep.stops;now+=10;boardIo.driverFeedback(true,bad);
    assert(pins[5]==HIGH&&onStep.stops==stops&&boardIo.brakeState()==BrakeState::Waiting);
    healthy();assert(pins[5]==HIGH);
  }
  for(const char*fault:{",OA,,,,,,",",,OB,,,,,",",,,GA,,,,",",,,,GB,,,",",,,,,OT,,",",,,,,,PW,",",,,,,,,GF","ST,OA,,,,,,","ST,,OB,,,,,","ST,,,GA,,,,","ST,,,,GB,,,","ST,,,,,OT,,","ST,,,,,,PW,","ST,,,,,,,GF"}){
    const int stops=onStep.stops;const size_t eventStart=pinEvents.size();
    onStep.stopHook=[](){boardIo.tick();swsNetworkActivity();assert(pins[5]==HIGH);};
    now+=10;boardIo.driverFeedback(true,fault);
    assert(pins[5]==HIGH&&onStep.stops==stops+1&&boardIo.brakeState()==BrakeState::Stopping);
    for(size_t i=eventStart;i<pinEvents.size();i++)assert(pinEvents[i]!=50); // 故障帧本身有 ST 也不直接抱闸。
    onStep.stopHook=nullptr;
    std::string stoppedReply=fault[0]==','?std::string("ST")+fault:fault;
    now+=100;boardIo.driverFeedback(true,stoppedReply.c_str());assert(pins[5]==HIGH);
    boardIo.driverFeedback(true,stoppedReply.c_str());assert(pins[5]==HIGH); // 同时刻的重复帧不能冒充持续静止。
    now+=250;boardIo.driverFeedback(true,stoppedReply.c_str());assert(pins[5]==LOW&&boardIo.brakeState()==BrakeState::Fault);
    assert(onStep.stops==stops+1);
    onStep.processCommand(":Q#",manualResponse,200);assert(pins[5]==LOW); // 手动 STOP 不改变抱闸。
    for(int i=0;i<4;i++){now+=250;boardIo.driverFeedback(true,stoppedReply.c_str());assert(pins[5]==LOW);}
    assert(onStep.stops==stops+2); // 只有上面的手动 STOP，没有自动重复发送。
    // 已抱闸后反馈失效也按本次要求松闸，旧的停稳结果不能复用。
    boardIo.driverFeedback(false,"");assert(pins[5]==HIGH);
    now+=10;boardIo.driverFeedback(true,fault);assert(pins[5]==HIGH&&onStep.stops==stops+3);
    now+=250;boardIo.driverFeedback(true,stoppedReply.c_str());assert(pins[5]==HIGH);
    now+=250;boardIo.driverFeedback(true,stoppedReply.c_str());assert(pins[5]==LOW);
    healthy();assert(pins[5]==HIGH&&boardIo.brakeState()==BrakeState::Released);
  }
  // 静止确认中途恢复运动必须重新计时，249ms 不能提前抱闸。
  boardIo.driverFeedback(true,",,,,,,,GF");
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH);
  now+=250;boardIo.driverFeedback(true,",,,,,,,GF");assert(pins[5]==HIGH);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH);
  now+=249;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH);
  now+=1;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==LOW);
  boardIo.tick();now+=1000;boardIo.tick();assert(pins[5]==LOW);
  now+=1;boardIo.tick();assert(pins[5]==HIGH);
  healthy();
  // STOP 后始终没有 ST，就持续松闸；不能用固定延时替代静止反馈。
  boardIo.driverFeedback(true,",,,,,,,GF");const int movingStops=onStep.stops;
  for(int i=0;i<10;i++){now+=250;boardIo.driverFeedback(true,",,,,,,,GF");assert(pins[5]==HIGH);}
  assert(onStep.stops==movingStops);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==LOW);
  const int staleStops=onStep.stops;advance(now+1001);assert(pins[5]==HIGH&&boardIo.brakeState()==BrakeState::Stale&&onStep.stops==staleStops);
  boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH&&onStep.stops==staleStops+1);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==LOW);
  // 每种无效反馈都验证从已抱闸切换到高电平，驱动全置位通信故障也不能继续抱闸。
  for(const char*missing:{"0","?","",",,,,,,","ST,OA,,,,,,bad","ST,OA,OB,GA,GB,OT,PW,GF"}){
    const int stops=onStep.stops;boardIo.driverFeedback(true,missing);assert(pins[5]==HIGH&&onStep.stops==stops);
    now+=10;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH);
    now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH);
    now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==LOW);
  }
  status.onStepFound=false;boardIo.tick();assert(pins[5]==HIGH);
  status.onStepFound=true;status.mountFound=0;boardIo.tick();assert(pins[5]==HIGH);
  status.mountFound=SD_TRUE;healthy();assert(pins[5]==HIGH);
  // 停止命令发送失败，ST 也不能让未完成的停止流程直接抱闸。
  onStep.stopSuccess=false;const int failedStops=onStep.stops;
  boardIo.driverFeedback(true,"ST,,,,,,,GF");assert(pins[5]==HIGH&&boardIo.brakeState()==BrakeState::StopFailed);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");now+=250;boardIo.driverFeedback(true,"ST,,,,,,,GF");
  assert(pins[5]==HIGH&&onStep.stops==failedStops+1);onStep.stopSuccess=true;healthy();assert(pins[5]==HIGH);
  // 静止前恢复正常反馈，也要先完成停止确认；随后正常反馈再持续 500ms 才恢复正常状态。
  boardIo.driverFeedback(true,",,,,,,,GF");
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  assert(boardIo.save(true,false));assert(pins[5]==LOW&&boardIo.brakeState()==BrakeState::Disabled&&nv().store.stored==1);
  const int disabledStops=onStep.stops,disabledCalls=onStep.calls;
  now+=1500;boardIo.pollDriver();boardIo.driverFeedback(true,"ST,OA,OB,GA,GB,OT,PW,GF");assert(pins[5]==LOW&&onStep.stops==disabledStops&&onStep.calls==disabledCalls);
  BoardIo disabledReboot;disabledReboot.initPins();disabledReboot.begin();assert(!disabledReboot.policy().protectionEnabled()&&pins[5]==LOW);
  assert(boardIo.save(false,true));assert(pins[5]==HIGH&&nv().store.stored==2); // 开启后无反馈同样保持高电平。
  nv().store.fail=true;assert(!boardIo.save(true,false));assert(pins[5]==HIGH&&boardIo.policy().protectionEnabled());nv().store.fail=false;
  BoardIo rebooted;rebooted.initPins();rebooted.begin();assert(!rebooted.policy().activityEnabled()&&rebooted.policy().protectionEnabled()&&pins[5]==HIGH);
  assert(boardIo.save(true,true));status.onStepFound=true;now=1200;
  registerPageHandler("/test",[](){assert(pins[2]==LOW);});www.handler();assert(pins[2]==LOW);
  advance(1320);assert(pins[2]==HIGH);advance(1440);assert(pins[2]==HIGH);
  for(uint32_t time=1500;time<2000;time+=20){now=time;swsNetworkActivity();assert(pins[2]==(boardIo.policy().ledLit(true,false,now)?LOW:HIGH));}
  assert(boardIo.save(false,true));advance(2400);assert(pins[2]==HIGH);
  status.onStepFound=false;advance(2400);assert(pins[2]==LOW&&pins[5]==HIGH);advance(2800);assert(pins[2]==HIGH);
  www.methodValue=HTTP_POST;www.args={{"setting","protection"},{"enabled","0"}};handleBoardIo();assert(www.code==200&&pins[5]==LOW);
  assert(www.output.find("\\"protection\\":false")!=std::string::npos);
  www.args["enabled"]="bad";handleBoardIo();assert(www.code==400&&pins[5]==LOW);
  www.args["enabled"]="1";nv().store.fail=true;handleBoardIo();assert(www.code==500&&pins[5]==LOW);nv().store.fail=false;
  handleBoardIo();assert(www.code==200&&pins[5]==HIGH);
  www.methodValue=HTTP_GET;handleBoardIo();assert(www.code==200);www.methodValue=2;handleBoardIo();assert(www.code==405);
  BoardIoPolicy absent;absent.configure(true,true,0);absent.driverFeedback(false,"",10);
  assert(!absent.feedbackDue(260)&&!absent.feedbackDue(1009)&&absent.feedbackDue(1010));
  absent.driverFeedback(true,"0",1010);assert(!absent.stopRequired()&&absent.releaseBrake(true,1010));
  absent.driverFeedback(true,"ST,,,,,,,",2010);assert(absent.feedbackDue(2260));
  BoardIoPolicy rollover;rollover.configure(true,true,0xfffffe00u);rollover.driverFeedback(true,"ST,,,,,,,GF",0xfffffe00u);
  assert(rollover.stopRequired());rollover.stopResult(true);
  rollover.driverFeedback(true,"ST,,,,,,,GF",0xffffff00u);assert(rollover.releaseBrake(true,0xffffff00u));
  rollover.driverFeedback(true,"ST,,,,,,,GF",100);assert(!rollover.releaseBrake(true,100));
  assert(rollover.releaseBrake(true,1200));rollover.checkFeedback(true,1200);
  rollover.driverFeedback(true,"ST,,,,,,,GF",1210);assert(rollover.stopRequired()&&rollover.releaseBrake(true,1210));
  String data;boardIoTile(data);assert(data.find("sws-io-activity")!=std::string::npos&&data.find("sws-io-protection")!=std::string::npos);
  data.clear();boardIoTileAjax(data);assert(data.find("sws-io-brake-state|")!=std::string::npos);
  tasks.fail=true;BoardIo failed;failed.initPins();failed.begin();assert(pins[5]==LOW&&failed.brakeState()==BrakeState::Starting);assert(!failed.save(true,false));
  std::cout<<"PASS: STOP then fresh stable ST before brake, all seven faults, no-ST wait, feedback loss/invalid/stale stays HIGH, recovery, failed STOP, manual STOP isolation, config/runtime OFF, NV, API, LED and rollover\\n";
}`;
  const file=path.join(work,'test.cpp'),exe=path.join(work,'test.exe');fs.writeFileSync(file,code);
  cp.execFileSync(process.env.CXX||'g++',['-std=c++11','-Wall','-Wextra',file,'-o',exe],{stdio:['pipe','pipe','pipe']});
  const html=cp.execFileSync(exe,[],{encoding:'utf8'});
  if(preview)return html;
  // 展开实际 pinmap 和模块配置，验证物理引脚及冲突诊断，而非检查源码字符串。
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
