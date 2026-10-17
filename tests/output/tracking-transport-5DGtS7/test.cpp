
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
#pragma once

#include <stdint.h>

// 与网页、串口命令及 AP/STA 配置分离，仅约束网络传输占用的资源。
namespace swsNetwork {
  static const uint32_t minimumHeap = 10000;
  static const uint32_t readWaitMs = 1000;
  static const uint32_t writeWaitMs = 1500;
  static const uint32_t responseLimitMs = 15000;
  static const unsigned commandBytesPerTurn = 64;
  static const unsigned commandsPerTurn = 1;

  inline bool elapsed(uint32_t now, uint32_t start, uint32_t limit) {
    return uint32_t(now - start) >= limit;
  }
}

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
char OnStepCmd::serialRecvFlush() {
  char c = 0;
  while (SERIAL_ONSTEP.available() > 0) c = SERIAL_ONSTEP.read();
  return c;
}


bool OnStepCmd::processCommand(const char* cmd, char* response, long timeOutMs) {

  #ifdef ESP32
    // create the mutex automatically
    static bool firstCall = true;
    if (firstCall) {
      xMutex = xSemaphoreCreateMutex();
      firstCall = false;
    }

    xSemaphoreTake(xMutex, portMAX_DELAY);
  #endif

  SERIAL_ONSTEP.setTimeout(timeOutMs);
  
  // clear the read/write buffers
  serialRecvFlush();

  // send the command
  SERIAL_ONSTEP.print(cmd);

  response[0] = 0;
  bool noResponse = false;
  bool shortResponse = false;
  if (cmd[0] == (char)6 && cmd[1] == 0) shortResponse = true;
  if (cmd[0] == ':' || cmd[0] == ';') {
    if (cmd[1] == 'G') {
      if (strchr("RDE", cmd[2])) { if (timeOutMs < 300) timeOutMs = 300; }
      if (cmd[2] == 'X') {
        if ((cmd[3] == 'E' && cmd[4] == 'E') ||
            (cmd[3] == '8' && cmd[4] == '9')) shortResponse = true;
      }
    } else
    if (cmd[1] == 'M') {
      if (strchr("ewnsg", cmd[2])) noResponse = true;
      if (strchr("ADNPS", cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'Q') {
      if (strchr("#ewns", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'A') {
      if (strchr("W123456789+",cmd[2])) { shortResponse = true; if (timeOutMs < 1000) timeOutMs = 1000; }
    } else
    if ((cmd[1] == 'F') || (cmd[1] == 'f')) {
      if (strchr("123456", cmd[2]) && cmd[3] != '#') {
        // direct focuser select command?
        if (strchr("+-QZHhF", cmd[3])) noResponse = true;
        if (strchr("1234", cmd[3])) noResponse = true;
        if (strchr("Aapc",cmd[3])) shortResponse = true;
      } else {
        // normal command
        if (strchr("+-QZHhF", cmd[2])) noResponse = true;
        if (strchr("1234", cmd[2])) noResponse = true;
        if (strchr("Aapc",cmd[2])) shortResponse = true;
      }
    } else
    if (cmd[1] == 'r') {
      if (strchr("+-PRFC<>Q1234", cmd[2])) noResponse = true;
      if (strchr("~S",cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'R') {
      if (strchr("AEGCMS0123456789", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'S') {
      if (strchr("CLSGtgMNOPrdhoTBXza", cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'L') {
      if (strchr("BNCDL!",cmd[2])) noResponse = true;
      if (strchr("o$W", cmd[2])) { shortResponse = true; if (timeOutMs < 1000) timeOutMs = 1000; }
    } else
    if (cmd[1] == 'B') {
      if (strchr("+-", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'C') {
      if (strchr("S", cmd[2])) noResponse = true;
    } else
    if (cmd[1] == 'h') {
      if (strchr("FC", cmd[2])) { noResponse = true; if (timeOutMs < 1000) timeOutMs = 1000; }
      if (strchr("QPR", cmd[2])) { shortResponse = true; if (timeOutMs < 300) timeOutMs = 300; }
    } else
    if (cmd[1] == 'T') {
      if (strchr("QR+-SLK", cmd[2])) noResponse = true;
      if (strchr("edrn", cmd[2])) shortResponse = true;
    } else
    if (cmd[1] == 'U') {
      noResponse = true; 
    } else
    if (cmd[1] == 'W') {
      if (strchr("R", cmd[2])) {
        if (strchr("+-", cmd[3])) shortResponse = true; else noResponse = true; // WR+ WR- else WR
      }
      if (strchr("S", cmd[2])) shortResponse = true;  // WS
      if (strchr("0123", cmd[2])) noResponse = true;  // W0 W1 W2 W3
    } else
    if (cmd[1] == '$' && cmd[2] == 'Q' && cmd[3] == 'Z') {
      if (strchr("+-Z/!", cmd[4])) noResponse = true;
    }

    // override for checksum protocol
    if (cmd[0] == ';') { noResponse = false; shortResponse = false; }
  }

  unsigned long timeout = millis() + (unsigned long)timeOutMs;
  if (noResponse) {
    response[0] = 0;
    #ifdef ESP32
      xSemaphoreGive(xMutex);
    #endif
    return true;
  } else
  if (shortResponse) {
    while ((long)(timeout - millis()) > 0) {
      if (SERIAL_ONSTEP.available()) { response[SERIAL_ONSTEP.readBytes(response, 1)] = 0; break; }
      delay(0);
    }
    if ((long)(timeout - millis()) <= 0) { DF("WRN: cmd "); D(cmd); DLF(" timed out"); }
    #ifdef ESP32
      xSemaphoreGive(xMutex);
    #endif
    return (response[0] != 0);
  } else {
    // get full response, '#' terminated
    int responsePos = 0;
    char b = 0;
    while ((long)(timeout - millis()) > 0 && b != '#') {
      if (SERIAL_ONSTEP.available()) {
        b = SERIAL_ONSTEP.read();
        response[responsePos] = b;
        responsePos++;
        if (responsePos > 79) responsePos = 79;
        response[responsePos] = 0;
      }
      delay(0);
    }
    if ((long)(timeout - millis()) <= 0) { DF("WRN: cmd \""); D(cmd); DLF("\" timed out"); }
    #ifdef ESP32
      xSemaphoreGive(xMutex);
    #endif
    const size_t n = strlen(response);
    return (n > 0) && (response[n - 1] == '#');
  }
}

bool OnStepCmd::command(const char* command, char* response) {
  bool success = processCommand(command, response, webTimeout);
  int l = strlen(response) - 1;
  if (l >= 0 && response[l] == '#') response[l] = 0;
  return success;
}

bool OnStepCmd::commandBlind(const char* command) {
  char response[80] = "";
  return processCommand(command, response, webTimeout);
}

bool OnStepCmd::commandEcho(const char* command) {
  char response[80] = "";
  char c[40] = "";
  snprintf(c, sizeof(c), ":EC%s#", command);
  return processCommand(c, response, webTimeout);
}

bool OnStepCmd::commandBool(const char* command) {
  char response[80] = "";
  bool success = processCommand(command, response, webTimeout);
  int l = strlen(response) - 1; if (l >= 0 && response[l] == '#') response[l] = 0;
  if (!success) return false;
  if (response[1] != 0) return false;
  if (response[0] == '0') return false; else return true;
}


OnStepCmd onStep;
struct String:std::string{using std::string::string;bool equals(const char*s){return *this==s;}};
const char*EmptyStr="";
struct Web{std::map<std::string,String>args;std::string result;
 String arg(const char*k){return args[k];}void setContentLength(int){}void sendHeader(const char*,const char*){}
 void send(int,const char*,String){result.clear();}void sendContent(const char*s){result+=s;}
}www;
int genericCalls=0;
void processMountGet(){genericCalls++;}void gotoCoordinatesGet(){}
void mountAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  // 坐标操作返回本次结果，避免与其他控制参数混合执行
  if (www.arg("dr").equals("T1") || www.arg("dr").equals("T0")) {
    // 跟踪开关必须返回主控的实际应答，不能用 HTTP 200 代替设备成功。
    char reply[80] = "";
    const bool received = onStep.command(www.arg("dr").equals("T1") ? ":Te#" : ":Td#", reply);
    www.sendContent(!received ? "timeout" : !strcmp(reply, "1") ? "ok" : !strcmp(reply, "0") ? "rejected" : "invalid");
  }
  else if (!www.arg("coord_action").equals(EmptyStr)) gotoCoordinatesGet();
  else processMountGet();

  www.sendContent("");
}


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
  void CmdServer::handleClient() {
    const unsigned long now = millis();

    // drop stale client socket (disconnected or timed out/hung)
    if (cmdSvrClient &&
       (!cmdSvrClient.connected() || (long)(clientEndTimeMs - now) < 0)) {
      cmdSvrClient.stop();
      cmdBuffer[0] = 0;
      cmdBufferPos = 0;
    }

    // new client
    if (!cmdSvrClient && cmdSvr->hasClient()) {
      cmdSvrClient = cmdSvr->available();
      if (cmdSvrClient) {
        #if NETWORK_LOAD_GUARD == ON
          if (ESP.getFreeHeap() < swsNetwork::minimumHeap) { cmdSvrClient.stop(); return; }
          cmdSvrClient.setTimeout(swsNetwork::writeWaitMs);
        #endif
        clientEndTimeMs = now + clientTimeoutMs;
        cmdBuffer[0] = 0;
        cmdBufferPos = 0;
      }
    }

    // check clients for data, if found get the command, pass to OnStep and pickup the response, then return the response to client
    unsigned bytes = 0, commands = 0;
    while (cmdSvrClient && cmdSvrClient.connected() && cmdSvrClient.available() > 0) {
      #if NETWORK_LOAD_GUARD == ON
        // 保留半帧和未读数据至下一轮，连续指令不能独占调度器。
        if (bytes >= swsNetwork::commandBytesPerTurn || commands >= swsNetwork::commandsPerTurn) break;
      #endif
      // still active? push back disconnect
      if (persist) clientEndTimeMs = millis() + clientTimeoutMs;

      // get the data
      int c = cmdSvrClient.read();
      if (c < 0 || c > 255) break;
      bytes++;
      swsNetworkActivity();
      
      // insert into the command buffer
      cmdBuffer[cmdBufferPos] = (char)c;
      cmdBufferPos++;
      if (cmdBufferPos > 39) cmdBufferPos = 39;
      cmdBuffer[cmdBufferPos] = 0;

      // send cmd and pickup the response
      if (c == '#' || (strlen(cmdBuffer) == 1 && c == (char)6)) {
        char result[80] = "";

        onStep.processCommand(cmdBuffer, result, cmdTimeout); 

        // pickup response
        if (strlen(result) > 0) {
          if (cmdSvrClient && cmdSvrClient.connected()) {
            const size_t length = strlen(result);
            if (cmdSvrClient.write((const uint8_t*)result, length) != length) cmdSvrClient.stop();
            swsNetworkActivity();
          }
        }

        // reset command buffer
        cmdBuffer[0] = 0;
        cmdBufferPos = 0;
        commands++;
      } else tasks.yield();
    }
  }


struct OriginalCmdServer:CmdServer{void handleClient();};
  void OriginalCmdServer::handleClient() {
    const unsigned long now = millis();

    // drop stale client socket (disconnected or timed out/hung)
    if (cmdSvrClient &&
       (!cmdSvrClient.connected() || (long)(clientEndTimeMs - now) < 0)) {
      cmdSvrClient.stop();
      cmdBuffer[0] = 0;
      cmdBufferPos = 0;
    }

    // new client
    if (!cmdSvrClient && cmdSvr->hasClient()) {
      cmdSvrClient = cmdSvr->available();
      if (cmdSvrClient) {
        clientEndTimeMs = now + clientTimeoutMs;
        cmdBuffer[0] = 0;
        cmdBufferPos = 0;
      }
    }

    // check clients for data, if found get the command, pass to OnStep and pickup the response, then return the response to client
    while (cmdSvrClient && cmdSvrClient.connected() && cmdSvrClient.available() > 0) {
      // still active? push back disconnect
      if (persist) clientEndTimeMs = millis() + clientTimeoutMs;

      // get the data
      int c = cmdSvrClient.read();
      if (c < 0 || c > 255) break;
      
      // insert into the command buffer
      cmdBuffer[cmdBufferPos] = (char)c;
      cmdBufferPos++;
      if (cmdBufferPos > 39) cmdBufferPos = 39;
      cmdBuffer[cmdBufferPos] = 0;

      // send cmd and pickup the response
      if (c == '#' || (strlen(cmdBuffer) == 1 && c == (char)6)) {
        char result[40] = "";

        onStep.processCommand(cmdBuffer, result, cmdTimeout); 

        // pickup response
        if (strlen(result) > 0) {
          if (cmdSvrClient && cmdSvrClient.connected()) {
            cmdSvrClient.print(result); // there was a delay(2); after this
          }
        }

        // reset command buffer
        cmdBuffer[0] = 0;
        cmdBufferPos = 0;
      } else tasks.yield();
    }
  }


int main(){
 // 同一组输入重放给原版和当前真实转发函数，逐字比较命令和回复。
 serial.responses[":Te#"]="1";serial.responses[":Td#"]="0";
 serial.responses[":GR#"]="12:34:56#";serial.responses[std::string(1,char(6))]="P";
 for(const std::string&input:std::vector<std::string>{":Te#",":Td#",":GR#",":Q#",":Te#:GR#:Td#",std::string(1,char(6))}){
   OriginalCmdServer old;CmdServer current;serial.sent.clear();old.cmdSvrClient.input=input;old.handleClient();
   const auto expected=serial.sent;serial.sent.clear();current.cmdSvrClient.input=input;while(current.cmdSvrClient.available())current.handleClient();
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
 std::cout<<"PASS: original/current TCP replay matches for fragmented/batched commands, tracking 1/0/timeout, ACK and STOP; actual endpoint, absent driver queries, complete-frame check and long replies\n";
}