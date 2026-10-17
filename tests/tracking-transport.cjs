const output=require('./test-output.cjs');
const fs=require('fs'),path=require('path'),cp=require('child_process');
const root=path.join(__dirname,'../src');
const read=p=>fs.readFileSync(path.join(root,p),'utf8');
const command=read('libApp/cmd/Cmd.cpp');
const mount=read('pages/mount/Mount.cpp');
const wifi=read('lib/wifi/cmdServer/CmdServer.cpp');
const originalWifi=cp.execFileSync('git',['show','193a81809f5b19b3afa2a8c58f18291c2f0b0475:src/lib/wifi/cmdServer/CmdServer.cpp'],{encoding:'utf8'});
const code=`
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <cassert>
#include <iostream>
#define DF(x)
#define D(x)
#define DLF(x)
#define CONTENT_LENGTH_UNKNOWN -1
#define ON 1
#define NETWORK_LOAD_GUARD 1
${read('lib/wifi/NetworkBudget.h').replace(/^#pragma once.*$/gm,'')}
struct Esp{unsigned getFreeHeap(){return 24000;}}ESP;
unsigned long clockMs=0;
unsigned long millis(){return clockMs;}
void delay(int){clockMs++;}
long webTimeout=200,cmdTimeout=200;
struct Serial {
 std::string incoming;std::map<std::string,std::string>responses;std::vector<std::string>sent;
 void setTimeout(long){} int available(){return incoming.size();}
 int read(){int c=incoming[0];incoming.erase(0,1);return c;}
 int readBytes(char*out,int count){int i=0;while(i<count&&available())out[i++]=read();return i;}
 void print(const char*s){sent.push_back(s);incoming+=responses[s];}
} serial;
#define SERIAL_ONSTEP serial
struct OnStepCmd {
 char serialRecvFlush();bool processCommand(const char*,char*,long);
 bool command(const char*,char*);bool commandBlind(const char*);bool commandBool(const char*);bool commandEcho(const char*);
};
${command.slice(command.indexOf('char OnStepCmd::serialRecvFlush()'),command.indexOf('void OnStepCmd::clearSerialChannel()'))}
${command.slice(command.indexOf('bool OnStepCmd::processCommand('),command.indexOf('char* OnStepCmd::commandString('))}
OnStepCmd onStep;
struct String:std::string{using std::string::string;bool equals(const char*s){return *this==s;}};
const char*EmptyStr="";
struct Web{std::map<std::string,String>args;std::string result;
 String arg(const char*k){return args[k];}void setContentLength(int){}void sendHeader(const char*,const char*){}
 void send(int,const char*,String){result.clear();}void sendContent(const char*s){result+=s;}
}www;
int genericCalls=0;
void processMountGet(){genericCalls++;}void gotoCoordinatesGet(){}
${mount.slice(mount.indexOf('void mountAjaxGet()'),mount.indexOf('void mountAjax()'))}
struct Client {std::string input,output;operator bool()const{return true;}bool connected(){return true;}
 int available(){return input.size();}void stop(){}int read(){int c=input[0];input.erase(0,1);return c;}
 void print(const char*s){output+=s;}
 void setTimeout(unsigned){}size_t write(const uint8_t*s,size_t n){output.append((const char*)s,n);return n;}
};
struct Server{bool hasClient(){return false;}Client available(){return Client();}} server;
struct Tasks{void yield(){clockMs++;}}tasks;
void swsNetworkActivity(){}
struct CmdServer{Server*cmdSvr=&server;Client cmdSvrClient;unsigned long clientEndTimeMs=1000000,clientTimeoutMs=10000;
 bool persist=true;char cmdBuffer[41]={};int cmdBufferPos=0;void handleClient();};
${wifi.slice(wifi.indexOf('  void CmdServer::handleClient()'),wifi.lastIndexOf('#endif'))}
struct OriginalCmdServer:CmdServer{void handleClient();};
${originalWifi.slice(originalWifi.indexOf('  void CmdServer::handleClient()'),originalWifi.lastIndexOf('#endif')).replace('CmdServer::handleClient','OriginalCmdServer::handleClient')}
int main(){
 // 同一组输入重放给原版和当前真实转发函数，逐字比较命令和回复。
 serial.responses[":Te#"]="1";serial.responses[":Td#"]="0";
 serial.responses[":GR#"]="12:34:56#";serial.responses[std::string(1,char(6))]="P";
 serial.responses[":GD#"]="+45*00:00#";serial.responses[":GU#"]="nNpEW190#";
 serial.responses[":Sr12:34:56#"]="1";serial.responses[":Sd+45*00:00#"]="1";serial.responses[":MS#"]="0";
 for(const std::string&input:std::vector<std::string>{":Te#",":Td#",":GR#",":Q#",":Te#:GR#:Td#",std::string(1,char(6)),":GR#:GD#:GU#",":Sr12:34:56#:Sd+45*00:00#:MS#",":Mgw0020#:Mgn0005#:GU#"}){
   OriginalCmdServer old;CmdServer current;serial.sent.clear();old.cmdSvrClient.input=input;old.handleClient();
   const auto expected=serial.sent;serial.sent.clear();current.cmdSvrClient.input=input;while(current.cmdSvrClient.available()){current.handleClient();clockMs+=5;}
   assert(serial.sent==expected&&current.cmdSvrClient.output==old.cmdSvrClient.output);
 }
 OriginalCmdServer fragmentedOld;CmdServer fragmented;
 for(const char*part:{":T","e", "#"}){
   fragmentedOld.cmdSvrClient.input=part;fragmentedOld.handleClient();
   fragmented.cmdSvrClient.input=part;fragmented.handleClient();
   assert(fragmented.cmdSvrClient.output==fragmentedOld.cmdSvrClient.output);
 }
 assert(fragmented.cmdSvrClient.output=="1");
 for(const auto &test:std::vector<std::pair<std::string,std::string>>{{"1","ok"},{"0","rejected"},{"","timeout"},{"?","invalid"}}){
  for(const auto &action:std::vector<std::pair<std::string,std::string>>{{"T1",":Te#"},{"T0",":Td#"}}){
   serial.responses[action.second]=test.first;serial.sent.clear();www.args["dr"]=action.first.c_str();mountAjaxGet();
   assert(www.result==test.second&&serial.sent==std::vector<std::string>{action.second});
   assert(genericCalls==0); // 不得经通用入口再次发送同一跟踪命令。
   CmdServer tcp;tcp.cmdSvrClient.input=action.second;tcp.handleClient();assert(tcp.cmdSvrClient.output==test.first);
  }
 }
 // 缺失驱动的单字符 0 和全置位状态不能改变下一条 Te 的应答。
 for(const char*driver:{"0","0#","ST,OA,OB,GA,GB,OT,PW,GF#",""}){
  char response[80];serial.responses[":GXU1#"]=driver;onStep.command(":GXU1#",response);
  serial.responses[":Te#"]="1";assert(onStep.commandBool(":Te#"));
  CmdServer tcp;tcp.cmdSvrClient.input=":Te#";tcp.handleClient();assert(tcp.cmdSvrClient.output=="1");
 }
 // 长应答经真实 TCP 转发函数完整返回，不覆盖调用栈。
 std::string longReply(70,'x');longReply+='#';serial.responses[":GVP#"]=longReply;
 CmdServer tcp;tcp.cmdSvrClient.input=":GVP#";tcp.handleClient();assert(tcp.cmdSvrClient.output==longReply);
 char response[80];serial.responses[":GXU1#"]="ST,,,,,,,";assert(!onStep.command(":GXU1#",response));
 std::cout<<"PASS: original/current TCP replay matches for fragmented/batched queries, coordinate setters/GOTO, pulse guiding, tracking 1/0/timeout, ACK and STOP with 5ms scheduling; actual endpoint, absent driver queries, complete-frame check and long replies\\n";
}`;
const folder=fs.mkdtempSync(path.join(output,'tracking-transport-')),source=path.join(folder,'test.cpp'),exe=path.join(folder,'test.exe');
fs.writeFileSync(source,code);
cp.execFileSync(process.env.CXX||'g++',['-std=c++11','-fstack-protector-all',source,'-o',exe],{stdio:'pipe'});
process.stdout.write(cp.execFileSync(exe,{encoding:'utf8'}));
