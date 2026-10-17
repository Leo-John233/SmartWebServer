const testOutput = require('./test-output.cjs');
// 运行方式 node legacy/tests/network-wifi-backend.cjs
// 将实际处理代码与模拟无线和网页接口一起编译，检查失败恢复与参数校验
const fs=require('node:fs'),path=require('node:path'),cp=require('node:child_process');
const temp=fs.mkdtempSync(path.join(testOutput,'sws-wifi-test-'));
const source=fs.readFileSync(path.join(__dirname,'../SmartWebServer/src/pages/network/NetworkWifi.cpp'),'utf8').replace(/^#include "NetworkWifi.h"\r?\n/m,'').replace(/^#include "..\/..\/lib\/wifi\/WifiManager.h"\r?\n/m,'').replace(/#include "(StationConnection|StationReconnect).h"/g,(_,name)=>fs.readFileSync(path.join(__dirname,'../SmartWebServer/src/pages/network',name+'.h'),'utf8').replace(/^#pragma once.*$/gm,''));
const program=fs.readFileSync(path.join(__dirname,'network-wifi-stubs.h'),'utf8')+'\n'+require('./locale-source.cjs')()+'\n'+source+'\n'+fs.readFileSync(path.join(__dirname,'network-wifi-cases.h'),'utf8');
const cpp=path.join(temp,'test.cpp'),exe=path.join(temp,process.platform==='win32'?'test.exe':'test');fs.writeFileSync(cpp,program);
for(const [command,args] of [[process.env.CXX||'g++',['-std=c++11','-Wall','-Wextra',cpp,'-o',exe]],[exe,[]]]){const r=cp.spawnSync(command,args,{encoding:'utf8'});process.stdout.write(r.stdout||'');process.stderr.write(r.stderr||'');if(r.error)throw r.error;if(r.status!==0)process.exit(r.status||1);}
