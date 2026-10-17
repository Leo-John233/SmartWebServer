const output=require('./test-output.cjs');
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process');
const root=path.join(__dirname,'../src');
const read=p=>fs.readFileSync(path.join(root,p),'utf8');
const clean=s=>s.replace(/^\s*#include[^\n]*$/gm,'').replace(/^#pragma once.*$/gm,'');
// 使用本机固件实际依赖的 2.4.2 内核调度代码，检查适配层与原有状态机的组合。
const sdk=process.env.SWS_ESP8266_CORE||path.join(process.env.LOCALAPPDATA,'Arduino15/packages/esp8266/hardware/esp8266/2.4.2');
const core=fs.readFileSync(path.join(sdk,'libraries/ESP8266WebServer/src/ESP8266WebServer.cpp'),'utf8');
const handle=core.slice(core.indexOf('void ESP8266WebServer::handleClient()'),core.indexOf('void ESP8266WebServer::close()'));
const work=fs.mkdtempSync(path.join(output,'network-load-'));
const code=`
#include <cassert>
#include <cstdint>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#define ON 1
#define OFF 0
#define WIFI 1
#define OPERATIONAL_MODE WIFI
#define COMMAND_SERVER ON
#define PSTR(x) x
#define PROGMEM
#define HTTP_MAX_DATA_WAIT 5000
#define HTTP_MAX_SEND_WAIT 5000
#define HTTP_MAX_CLOSE_WAIT 2000
#define CONTENT_LENGTH_NOT_SET -1
using PGM_P=const char*;
uint32_t clockMs=0;
uint32_t millis(){return clockMs;}
void yield(){}
struct Esp{uint32_t heap=24000;uint32_t getFreeHeap(){return heap;}}ESP;
struct Socket {std::string input,output;bool closed=false,partial=false;unsigned waits=0,closes=0;uint32_t timeout=0;};
struct WiFiClient {
 std::shared_ptr<Socket> socket;
 WiFiClient()=default;WiFiClient(std::shared_ptr<Socket>s):socket(s){}
 operator bool()const{return socket&&!socket->closed;}
 bool connected(){return bool(*this);}int available(){return connected()?socket->input.size():0;}
 int read(){if(!available())return -1;auto c=(unsigned char)socket->input[0];socket->input.erase(0,1);return c;}
 void setTimeout(uint32_t v){if(socket)socket->timeout=v;}void setNoDelay(bool){}
 void stop(){if(socket){socket->closed=true;socket->closes++;}}
 size_t write(const char*s,size_t n){if(!connected())return 0;socket->waits++;size_t count=socket->partial?n/2:n;socket->output.append(s,count);return count;}
 size_t write(const uint8_t*s,size_t n){return write((const char*)s,n);}
 size_t write_P(const char*s,size_t n){return write(s,n);}
 void print(const char*s){write(s,strlen(s));}
};
struct WiFiServer {
 std::deque<std::shared_ptr<Socket>> pending;
 WiFiServer(int=0){}void begin(){}void setNoDelay(bool){}
 bool hasClient(){return !pending.empty();}WiFiClient available(){if(pending.empty())return {};auto s=pending.front();pending.pop_front();return WiFiClient(s);}
};
enum HTTPClientStatus {HC_NONE,HC_WAIT_READ,HC_WAIT_CLOSE};
class ESP8266WebServer {
 public:
  ESP8266WebServer(int){}void handleClient();
 protected:
  WiFiServer _server;WiFiClient _currentClient;HTTPClientStatus _currentStatus=HC_NONE;uint32_t _statusChange=0;int _contentLength=0;
  std::unique_ptr<int> _currentUpload;
  virtual size_t _currentClientWrite(const char*,size_t)=0;virtual size_t _currentClientWrite_P(const char*,size_t)=0;
  virtual bool _parseRequest(WiFiClient&c)=0;virtual void _handleRequest()=0;
};
${handle}
${clean(read('lib/wifi/NetworkBudget.h'))}
${clean(read('lib/wifi/webServer/GuardedServer.h'))}
class TestServer:public SwsWebServer {
 public:
  TestServer():SwsWebServer(80){}
  unsigned handled=0;uint32_t readTimeout=0;std::function<void()> action;
  using SwsWebServer::_currentClientWrite;using SwsWebServer::_currentClientWrite_P;
  std::shared_ptr<Socket> enqueue(std::string input="GET /mount.htm HTTP/1.1\\r\\n\\r\\n") {auto s=std::make_shared<Socket>();s->input=input;_server.pending.push_back(s);return s;}
  HTTPClientStatus phase(){return _currentStatus;}
  void current(std::shared_ptr<Socket>s){_currentClient=WiFiClient(s);}
 protected:
  bool _parseRequest(WiFiClient&c)override{readTimeout=c.socket->timeout;c.socket->input.clear();return true;}
  void _handleRequest()override{handled++;if(action)action();else {_currentClientWrite("HTTP/1.1 200 OK\\r\\nConnection: close\\r\\n\\r\\n",38);_currentClientWrite_P("page",4);}}
};
struct Tasks{void yield(){clockMs++;}}tasks;
void swsNetworkActivity(){}
long cmdTimeout=200;
struct OnStep {std::vector<std::string> sent;bool processCommand(const char*c,char*out,long){sent.push_back(c);strcpy(out,strcmp(c,":Q#")?"1":"");return true;}}onStep;
class CmdServer {
 public:
  CmdServer(uint32_t,long,bool=false);void begin();void handleClient();
  WiFiServer*cmdSvr;WiFiClient cmdSvrClient;char cmdBuffer[40];int cmdBufferPos;
  unsigned long clientEndTimeMs,clientTimeoutMs;bool persist;long port;
  ~CmdServer(){delete cmdSvr;}
  std::shared_ptr<Socket> enqueue(std::string s){auto socket=std::make_shared<Socket>();socket->input=s;cmdSvr->pending.push_back(socket);return socket;}
};
${clean(read('lib/wifi/cmdServer/CmdServer.cpp'))}
int main(){
 #if NETWORK_LOAD_GUARD == ON
  // LAN 和 AP 共用同一服务，多个已排队连接依次完成，不等客户端主动断开。
  TestServer web;std::vector<std::shared_ptr<Socket>> clients;
  for(int i=0;i<4;i++)clients.push_back(web.enqueue());
  for(int i=0;i<4;i++){web.handleClient();assert(web.handled==unsigned(i+1)&&web.phase()==HC_NONE);assert(clients[i]->closed&&clients[i]->output.find("page")!=std::string::npos);}
  assert(clockMs==0&&web.readTimeout==1000);
  for(int i=0;i<2000;i++){auto c=web.enqueue();web.handleClient();assert(c->closed&&web.phase()==HC_NONE);}
  // 请求尚未发送的连接不能长期挡住后面的客户，含 millis 回绕。
  for(uint32_t start:{uint32_t(0),uint32_t(0xffffff00)}){clockMs=start;TestServer idle;auto first=idle.enqueue("");auto second=idle.enqueue();idle.handleClient();assert(idle.handled==0);clockMs=uint32_t(start+1000);idle.handleClient();assert(first->closed&&second->closed&&idle.handled==1);}
  // 低内存先返回 503，绝不执行本次控制请求；内存恢复后立即接受新请求。
  clockMs=0;ESP.heap=9999;TestServer busy;auto c=busy.enqueue("GET /mount-ajax-get.txt?dr=T1 HTTP/1.1\\r\\n\\r\\n");busy.handleClient();assert(busy.handled==0&&c->closed&&c->output.find("503 Service Unavailable")!=std::string::npos&&c->output.find("Retry-After: 2")!=std::string::npos);
  ESP.heap=24000;auto retry=busy.enqueue();busy.handleClient();assert(busy.handled==1&&retry->closed);
  // 包含解析或上传的等待时间不计入响应发送限额；仅限制开始输出后的总时间。
  TestServer slow;auto s=slow.enqueue();slow.action=[&](){clockMs+=60000;assert(slow._currentClientWrite("a",1)==1);clockMs+=14999;assert(slow._currentClientWrite_P("b",1)==1);clockMs++;assert(slow._currentClientWrite("c",1)==0);};slow.handleClient();assert(s->closed&&s->output=="ab");
  // 调度期间重入不得覆盖当前连接。
  TestServer nested;auto n=nested.enqueue();nested.action=[&](){nested.handleClient();assert(nested._currentClientWrite("ok",2)==2);};nested.handleClient();assert(nested.handled==1&&n->output=="ok");
  // 不完整写入立即关闭；后续分段不会再次等待。
  for(bool flash:{false,true}){TestServer shortWrite;auto q=std::make_shared<Socket>();q->partial=true;shortWrite.current(q);auto send=[&](){return flash?shortWrite._currentClientWrite_P("data",4):shortWrite._currentClientWrite("data",4);};assert(send()==2&&q->closed);for(int i=0;i<100;i++)assert(send()==0);assert(q->waits==1&&q->timeout==1500);}
  // 洪水客户端每轮只处理一条命令，另一入口下一轮可正常处理停止命令。
  clockMs=0;CmdServer heavy(9999,10000,true),light(9998,10000,true);heavy.begin();light.begin();std::string batch;for(int i=0;i<100;i++)batch+=":Te#";auto flood=heavy.enqueue(batch),stop=light.enqueue(":Q#");heavy.handleClient();assert(onStep.sent==std::vector<std::string>{":Te#"}&&flood->input.size()==batch.size()-4);light.handleClient();assert(onStep.sent.back()==":Q#");for(int i=0;i<99;i++)heavy.handleClient();assert(flood->input.empty()&&flood->output==std::string(100,'1'));
  // 半帧跨轮保留，垃圾数据也有字节上限；错误应答链路及时释放。
  CmdServer fragmented(9999,10000,true);fragmented.begin();auto f=fragmented.enqueue(":T");fragmented.handleClient();f->input="e#";fragmented.handleClient();assert(f->output=="1");f->input=std::string(500,'x');fragmented.handleClient();assert(f->input.size()==436);f->input="#";f->partial=true;fragmented.handleClient();assert(f->closed);
  CmdServer refused(9999,10000,true);refused.begin();auto r=refused.enqueue(":Te#");const auto sentBefore=onStep.sent.size();ESP.heap=9999;refused.handleClient();assert(r->closed&&onStep.sent.size()==sentBefore);ESP.heap=24000;
  // 标准端口的一秒连接期限不应截断被分轮处理的已接收批量命令。
  clockMs=0;CmdServer standard(9999,1000,false);standard.begin();auto queued=standard.enqueue(":Te#:Te#:Te#:Te#:Te#");for(int i=0;i<5;i++){standard.handleClient();clockMs+=400;}assert(queued->output=="11111"&&queued->input.empty());standard.handleClient();assert(queued->closed);
 #else
  TestServer old;auto c=old.enqueue();old.enqueue();old.handleClient();assert(old.handled==1&&old.phase()==HC_WAIT_CLOSE);old.handleClient();assert(old.handled==1);clockMs+=2001;old.handleClient();old.handleClient();assert(old.handled==2);
  CmdServer original(9999,10000,true);original.begin();auto b=original.enqueue(":Te#:Te#");original.handleClient();assert(b->output=="11");
 #endif
 std::cout<<"PASS: NETWORK_LOAD_GUARD="<<NETWORK_LOAD_GUARD<<"\\n";
}`;
const source=path.join(work,'test.cpp');fs.writeFileSync(source,code);
for(const enabled of [1,0]){const exe=path.join(work,'test-'+enabled+'.exe');cp.execFileSync('g++',['-std=c++11','-fstack-protector-all','-DNETWORK_LOAD_GUARD='+enabled,source,'-o',exe],{stdio:'pipe'});process.stdout.write(cp.execFileSync(exe,{encoding:'utf8'}));}
