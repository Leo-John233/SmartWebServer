const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const assert = require('node:assert/strict');
const root = path.join(__dirname,'../SmartWebServer');
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
  boardIo.initPins();assert(pins[5]==LOW&&pinEvents[0]==50);
  boardIo.begin();assert(!boardIo.brakeAvailable()&&boardIo.ownsPin(5));
  assert(tasks.added==${activityAvailable ? '1' : '0'}); // 关闭自动保护后没有抱闸采样任务。
  status.onStepFound=true;status.mountFound=SD_TRUE;
  for(now=100;now<=2100;now+=250){boardIo.driverFeedback(true,"ST,,,,,,,");boardIo.pollDriver();boardIo.tick();assert(pins[5]==LOW);}
  assert(onStep.calls==0); // 旧 NV 中保存的开启状态和正常反馈均不能使 IO5 拉高。
  for(const char*fault:{"ST,OA,,,,,,","ST,,OB,,,,,","ST,,,GA,,,,","ST,,,,GB,,,","ST,,,,,OT,,","ST,,,,,,PW,","ST,,,,,,,GF"})boardIo.driverFeedback(true,fault);
  assert(onStep.stops==0); // 编译关闭后停止和抱闸自动保护一起停用。
  assert(boardIo.save(true,true)==${activityAvailable ? 'true' : 'false'});assert(pins[5]==LOW);
  status.onStepFound=false;boardIo.tick();assert(pins[5]==LOW);
  String data;boardIoTile(data);assert(data.find("sws-io-protection")==std::string::npos);
  assert((data.find("sws-io-activity")!=std::string::npos)==${activityAvailable ? 'true' : 'false'});
  data.clear();boardIoTileAjax(data);assert(data.find("sws-io-brake-state|")==std::string::npos);
  www.methodValue=HTTP_POST;www.args={{"setting","protection"},{"enabled","1"}};handleBoardIo();assert(www.code==409&&pins[5]==LOW);
  www.args["enabled"]="0";handleBoardIo();assert(www.code==409&&pins[5]==LOW);
  for(int event:pinEvents)assert(event!=51); // 从初始化到整个测试结束从未输出 HIGH。
  std::cout<<"PASS: config OFF hides brake controls and status, refuses API changes, skips driver queries and holds IO5 LOW with legacy NV and healthy feedback\\n";
}`:`
void advance(uint32_t time){now=time;boardIo.tick();}
int main(){
  boardIo.initPins();assert(pins[5]==LOW&&pinEvents[0]==50);
  advance(5000);assert(pins[5]==LOW); // 配置未加载时绝不释放。
  boardIo.begin();assert(tasks.added==2&&pins[5]==LOW);
  assert(boardIo.ownsPin(2)&&boardIo.ownsPin(5)&&!boardIo.ownsPin(-1)&&!boardIo.ownsPin(4));
  assert(boardIo.brakeState()==BrakeState::Waiting);
  status.onStepFound=true;status.mountFound=SD_TRUE;
  now=5010;boardIo.pollDriver();assert(pins[5]==LOW);
  const int calls=onStep.calls;boardIo.pollDriver();assert(onStep.calls==calls); // 不重复查询。
  advance(5260);assert(pins[5]==LOW);
  advance(5510);assert(pins[5]==LOW); // 单帧正常反馈不能仅靠延时触发释放。
  boardIo.pollDriver();assert(pins[5]==HIGH);
  // 手动 STOP 只发运动停止命令；单独 ST 不会改变已松闸状态。
  char manualResponse[80]="";
  onStep.processCommand(":Q#",manualResponse,200);assert(pins[5]==HIGH);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH&&!boardIo.policy().driverFault());
  // STOP 分发中模拟其他任务刷新 GPIO，仍必须等 STOP 调用完成再拉低 IO5。
  onStep.stopHook=[](){boardIo.tick();swsNetworkActivity();assert(pins[5]==HIGH);};
  for(const char*bad:{"0","?","OK","",",,,,,,",",,,,,,,,,","XX,,,,,,,", "ST,OA,OB,GA,GB,OT,PW,GF",",OA,,,,,,",",,OB,,,,,",",,,GA,,,,",",,,,GB,,,",",,,,,OT,,",",,,,,,PW,",",,,,,,,GF","ST,OA,,,,,,","ST,,OB,,,,,","ST,,,GA,,,,","ST,,,,GB,,,","ST,,,,,OT,,","ST,,,,,,PW,","ST,,,,,,,GF"}){
    const int stops=onStep.stops;const size_t eventStart=pinEvents.size();
    now+=10;boardIo.driverFeedback(true,bad);assert(pins[5]==LOW);
    const bool fault=boardIo.policy().driverFault();
    assert(onStep.stops==stops+(fault?1:0));
    if(fault){
      size_t stopEvent=eventStart;while(stopEvent<pinEvents.size()&&pinEvents[stopEvent]!=900){assert(pinEvents[stopEvent]!=50);stopEvent++;}
      assert(stopEvent<pinEvents.size()&&pinEvents.back()==50);
      // 持续异常、短暂正常或无效反馈均不能让同一故障连续发送 STOP。
      onStep.stopHook=nullptr;
      now+=10;boardIo.driverFeedback(true,bad);
      now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");now+=100;boardIo.driverFeedback(true,bad);
      boardIo.driverFeedback(false,"");boardIo.driverFeedback(true,bad);assert(onStep.stops==stops+1);
      onStep.stopHook=[](){boardIo.tick();swsNetworkActivity();assert(pins[5]==HIGH);};
    }
    now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");
    now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");
    now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  }
  // STOP 发送失败仍立即抱闸，不等待回应或无限重试。
  onStep.stopSuccess=false;const int failedStop=onStep.stops;
  boardIo.driverFeedback(true,",,,,,,,GF");assert(pins[5]==LOW&&onStep.stops==failedStop+1);
  boardIo.driverFeedback(true,",,,,,,,GF");assert(onStep.stops==failedStop+1);
  onStep.stopHook=nullptr;onStep.stopSuccess=true;
  // 故障已抱闸时再按手动 STOP，既不松闸，也不重新触发自动 STOP。
  const int beforeManual=onStep.stops;onStep.processCommand(":Q#",manualResponse,200);
  assert(pins[5]==LOW&&onStep.stops==beforeManual+1);
  boardIo.driverFeedback(true,",,,,,,,GF");assert(pins[5]==LOW&&onStep.stops==beforeManual+1);
  now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");now+=500;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  boardIo.driverFeedback(false,"");assert(pins[5]==LOW);
  now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");now+=500;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  advance(now+1001);assert(pins[5]==LOW&&boardIo.brakeState()==BrakeState::Stale);
  boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW); // 过期后重新等待稳定反馈。
  status.onStepFound=false;boardIo.tick();assert(pins[5]==LOW);
  status.onStepFound=true;now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");now+=500;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  assert(boardIo.save(true,false));assert(pins[5]==LOW&&boardIo.brakeState()==BrakeState::Disabled);
  assert(nv().store.stored==1); // 关闭保护立即抱闸，设置写入 NV。
  const int disabledCalls=onStep.calls,disabledStops=onStep.stops;
  now+=1500;boardIo.pollDriver();boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW&&onStep.calls==disabledCalls);
  boardIo.driverFeedback(true,"ST,OA,OB,GA,GB,OT,PW,GF");assert(pins[5]==LOW&&onStep.stops==disabledStops);
  BoardIo disabledReboot;disabledReboot.initPins();disabledReboot.begin();assert(!disabledReboot.policy().protectionEnabled()&&pins[5]==LOW);
  disabledReboot.driverFeedback(true,"ST,,,,,,,");now+=500;disabledReboot.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW);
  assert(boardIo.save(false,true));assert(pins[5]==LOW&&nv().store.stored==2);
  now+=500;boardIo.tick();assert(pins[5]==LOW); // 重新开启不能沿用关闭前的就绪结果。
  boardIo.driverFeedback(true,"ST,,,,,,,");now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW);
  now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  boardIo.driverFeedback(false,"");assert(pins[5]==LOW);
  nv().store.fail=true;assert(!boardIo.save(true,false));assert(pins[5]==LOW&&boardIo.policy().protectionEnabled());nv().store.fail=false;
  BoardIo rebooted;rebooted.initPins();rebooted.begin();assert(!rebooted.policy().activityEnabled()&&rebooted.policy().protectionEnabled()&&pins[5]==LOW);
  // 网络活动既覆盖网页入口，也覆盖资源发送；空闲时没有持续脉冲。
  assert(boardIo.save(true,true));status.onStepFound=true;now=1200;
  registerPageHandler("/test",[](){assert(pins[2]==LOW);});www.handler();assert(pins[2]==LOW);
  advance(1320);assert(pins[2]==HIGH);advance(1440);assert(pins[2]==HIGH);
  for(uint32_t time=1500;time<2000;time+=20){now=time;swsNetworkActivity();assert(pins[2]==(boardIo.policy().ledLit(true,false,now)?LOW:HIGH));}
  assert(boardIo.save(false,true));advance(2400);assert(pins[2]==HIGH);
  status.onStepFound=false;advance(2400);assert(pins[2]==LOW);advance(2800);assert(pins[2]==HIGH); // 关闭活动功能仍保留掉线闪烁。
  // 接口使用明确的 0/1，只在持久化成功后更新状态。
  www.methodValue=HTTP_POST;www.args={{"setting","protection"},{"enabled","0"}};handleBoardIo();assert(www.code==200&&pins[5]==LOW);
  assert(www.output.find("\\"protection\\":false")!=std::string::npos);
  www.args["enabled"]="bad";handleBoardIo();assert(www.code==400&&pins[5]==LOW);
  www.args["enabled"]="1";nv().store.fail=true;handleBoardIo();assert(www.code==500&&pins[5]==LOW);nv().store.fail=false;
  handleBoardIo();assert(www.code==200&&pins[5]==LOW);
  www.methodValue=HTTP_GET;handleBoardIo();assert(www.code==200);
  www.methodValue=2;handleBoardIo();assert(www.code==405);
  // uint32_t 时钟回绕不应绕过反馈稳定时间或过期判断。
  BoardIoPolicy rollover;rollover.configure(true,true,0xfffffe00u);rollover.driverFeedback(true,"ST,,,,,,,",0xfffffe00u);
  rollover.driverFeedback(true,"ST,,,,,,,",0xffffff00u);assert(!rollover.releaseBrake(true,0xffffff00u));
  rollover.driverFeedback(true,"ST,,,,,,,",100);assert(rollover.releaseBrake(true,100));assert(!rollover.releaseBrake(true,1200));
  String data;boardIoTile(data);assert(data.find("sws-io-activity")!=std::string::npos&&data.find("sws-io-protection")!=std::string::npos);
  data.clear();boardIoTileAjax(data);assert(data.find("sws-io-brake-state|")!=std::string::npos);
  tasks.fail=true;BoardIo failed;failed.initPins();failed.begin();assert(pins[5]==LOW&&failed.brakeState()==BrakeState::Starting);assert(!failed.save(true,false));
  std::cout<<"PASS: STOP before brake for seven faults including standstill, manual STOP isolation, no repeated STOP, task reentry order, failed STOP fallback, startup/disabled locks, recovery, NV, HTTP, LED and clock rollover\\n";
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
