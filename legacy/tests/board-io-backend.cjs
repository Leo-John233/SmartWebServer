const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const assert = require('node:assert/strict');
const root = path.join(__dirname,'../SmartWebServer');
const clean = (file) => fs.readFileSync(path.join(root,'src',file),'utf8').replace(/^\s*#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');

// 编译实际策略、GPIO 适配器和设置接口；只替换串口、引脚、时钟和存储。
module.exports = function({preview=false,language=process.env.SWS_TEST_LANGUAGE||'cn'}={}) {
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
#define SWS_ACTIVITY_LED_PIN 2
#define SWS_RA_BRAKE_PIN 5
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
struct Device{int calls=0;bool received=true;const char*reply="ST,,,,,,,";bool processCommand(const char*command,char*out,int timeout){assert(!strcmp(command,":GXU1#")&&timeout==200);calls++;strcpy(out,reply);strcat(out,"#");return received;}}onStep;
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
  code+=clean('libApp/boardIo/Policy.h')+'\n'+clean('libApp/boardIo/BoardIo.h')+'\n';
  code+=clean('libApp/boardIo/Policy.cpp')+'\n'+clean('libApp/boardIo/BoardIo.cpp')+'\n';
  code+=require('./locale-source.cjs')(language)+'\n';
  for(const [name,value]of Object.entries(templates['boardIo/View.h']))code+='const char '+name+'[]='+JSON.stringify(value)+';\n';
  code+=clean('pages/boardIo/BoardIo.cpp')+'\n'+clean('pages/Request.cpp')+'\n';
  code+=preview?`int main(){boardIo.initPins();boardIo.begin();status.onStepFound=true;status.mountFound=SD_TRUE;now=10;boardIo.driverFeedback(true,"ST,,,,,,,");now=600;boardIo.driverFeedback(true,"ST,,,,,,,");String data;boardIoTile(data);std::cout<<data;}`:`
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
  for(const char*bad:{"0","?","OK","",",,,,,,",",,,,,,,,,","XX,,,,,,,", "ST,OA,OB,GA,GB,OT,PW,GF",",OA,,,,,,",",,,GA,,,,",",,,,GB,,,",",,,,,OT,,",",,,,,,PW,",",,,,,,,GF"}){
    now+=10;boardIo.driverFeedback(true,bad);assert(pins[5]==LOW);
    now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");
    now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");
    now+=250;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  }
  boardIo.driverFeedback(false,"");assert(pins[5]==LOW);
  now+=10;boardIo.driverFeedback(true,"ST,,,,,,,");now+=500;boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==HIGH);
  advance(now+1001);assert(pins[5]==LOW&&boardIo.brakeState()==BrakeState::Stale);
  boardIo.driverFeedback(true,"ST,,,,,,,");assert(pins[5]==LOW); // 过期后重新等待稳定反馈。
  status.onStepFound=false;boardIo.tick();assert(pins[5]==LOW);
  assert(boardIo.save(true,false));assert(pins[5]==HIGH&&boardIo.brakeState()==BrakeState::Disabled);
  assert(nv().store.stored==1); // 用户明确关闭保护时保持释放，设置写入 NV。
  assert(boardIo.save(false,true));assert(pins[5]==LOW&&nv().store.stored==2);
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
  www.methodValue=HTTP_POST;www.args={{"setting","protection"},{"enabled","0"}};handleBoardIo();assert(www.code==200&&pins[5]==HIGH);
  assert(www.output.find("\\"protection\\":false")!=std::string::npos);
  www.args["enabled"]="bad";handleBoardIo();assert(www.code==400&&pins[5]==HIGH);
  www.args["enabled"]="1";nv().store.fail=true;handleBoardIo();assert(www.code==500&&pins[5]==HIGH);nv().store.fail=false;
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
  std::cout<<"PASS: actual IO2/IO5 output polarity, startup lock, valid/invalid feedback, stable recovery, stale lock, no-browser polling, manual override, NV reload/failure, HTTP settings, activity pulses, original disconnect blink and clock rollover\\n";
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
    'static_assert(SWS_ACTIVITY_LED_PIN==2,"IO2 activity LED mapping");\n';
  const pinFile=path.join(work,'pins.cpp');fs.writeFileSync(pinFile,pinSource);
  cp.execFileSync(process.env.CXX||'g++',['-std=c++11','-fsyntax-only',pinFile]);
  fs.writeFileSync(pinFile,pinSource.replace('#define AXIS2_ENCODER OFF','#define AXIS2_ENCODER ON'));
  const conflict=cp.spawnSync(process.env.CXX||'g++',['-std=c++11','-fsyntax-only',pinFile],{encoding:'utf8'});
  assert.notEqual(conflict.status,0);assert(conflict.stderr.includes('Axis 2 encoder A is disabled'));
  console.log(html.trim());
};
if(require.main===module)module.exports();
