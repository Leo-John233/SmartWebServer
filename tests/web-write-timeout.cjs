const output=require('./test-output.cjs');
const fs=require('fs'),path=require('path'),cp=require('child_process');
const root=path.join(__dirname,'../SmartWebServer');
const source=fs.readFileSync(path.join(root,'src/lib/wifi/webServer/GuardedServer.h'),'utf8');
const klass=source.replace(/^#include.*$/gm,'').replace(/^#pragma once.*$/gm,'');
const budget=fs.readFileSync(path.join(root,'src/lib/wifi/NetworkBudget.h'),'utf8').replace(/^#pragma once.*$/gm,'');
const work=path.join(output,'web-write-timeout');fs.mkdirSync(work,{recursive:true});
const code=`#include <cassert>
#include <cstddef>
using PGM_P=const char*;
#define NETWORK_LOAD_GUARD 0
#define ON 1
${budget}
struct Client {bool closed=false,partial=false;int waits=0,timeout=0;void setTimeout(int ms){timeout=ms;}size_t write(const char*,size_t n){if(closed)return 0;++waits;return partial?n/2:n;}size_t write_P(const char*p,size_t n){return write(p,n);}void stop(){closed=true;}};
class ESP8266WebServer {public:ESP8266WebServer(int){};protected:Client _currentClient;virtual size_t _currentClientWrite(const char*,size_t)=0;virtual size_t _currentClientWrite_P(PGM_P,size_t)=0;};
${klass}
class TestServer:public SwsWebServer {public:TestServer():SwsWebServer(80){}using SwsWebServer::_currentClientWrite;using SwsWebServer::_currentClientWrite_P;Client& client(){return _currentClient;}};
int main(){for(int flash=0;flash<2;++flash){TestServer server;auto& c=server.client();auto send=[&](){return flash?server._currentClientWrite_P("data",4):server._currentClientWrite("data",4);};assert(send()==4&&!c.closed&&c.timeout==1500);c.partial=true;assert(send()==2&&c.closed);int waits=c.waits;for(int i=0;i<100;++i)assert(send()==0);assert(c.waits==waits);}}
`;
const file=path.join(work,'test.cpp'),exe=path.join(work,'test.exe');fs.writeFileSync(file,code);
cp.execFileSync('g++',['-std=c++11',file,'-o',exe]);cp.execFileSync(exe);
console.log('PASS: healthy RAM/Flash writes preserved, partial writes close stale socket and later chunks do not wait again');
