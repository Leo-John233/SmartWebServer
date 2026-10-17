const testOutput = require('./test-output.cjs');
// 验证真实坐标处理代码，错误输入或设备拒绝时不得触发移动
const fs=require('fs'),path=require('path'),cp=require('child_process');
const root=path.join(__dirname,'../SmartWebServer/src/pages/mount');
const handler=fs.readFileSync(path.join(root,'GotoTile.cpp'),'utf8').split('void gotoCoordinatesGet()')[1];
const stub=`#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <iostream>
struct String:std::string{using std::string::string;bool equals(const char*s)const{return *this==s;}};
struct Web{std::map<std::string,String> args;std::string reply;String arg(const char*k){return args[k];}void sendContent(const char*s){reply+=s;}}www;
struct Status{bool onStepFound=true;}status;
struct Device{std::vector<std::string> sent;int fail=0;bool rejectGoto=false;bool commandBool(const char*s){sent.push_back(s);return int(sent.size())!=fail;}bool command(const char*s,char*out){sent.push_back(s);strcpy(out,rejectGoto?"1":"0");return true;}}onStep;
void gotoCoordinatesGet()`;
const cases=`
void reset(){www.reply="";onStep.sent.clear();onStep.fail=0;onStep.rejectGoto=false;status.onStepFound=true;www.args={{"ra_h","23"},{"ra_m","59"},{"ra_s","59"},{"dec_sign","-"},{"dec_d","0"},{"dec_m","30"},{"dec_s","0"},{"coord_action","go"}};}
int main(){reset();gotoCoordinatesGet();assert(www.reply=="ok");assert(onStep.sent==std::vector<std::string>({":Sr23:59:59#",":Sd-00*30:00#",":MS#"}));
reset();www.args["coord_action"]="sync";gotoCoordinatesGet();assert(onStep.sent.back()==":CS#");assert(www.reply=="ok");
for(auto bad:{"","24","-1","1.5","1x","9999999999999999999"}){reset();www.args["ra_h"]=bad;gotoCoordinatesGet();assert(www.reply=="invalid");assert(onStep.sent.empty());}
reset();www.args["dec_d"]="90";gotoCoordinatesGet();assert(www.reply=="invalid");assert(onStep.sent.empty());
reset();www.args["dec_d"]="90";www.args["dec_m"]="0";gotoCoordinatesGet();assert(www.reply=="ok");
reset();www.args["coord_action"]="other";gotoCoordinatesGet();assert(onStep.sent.empty());
reset();www.args["dec_sign"]="x";gotoCoordinatesGet();assert(onStep.sent.empty());
reset();status.onStepFound=false;gotoCoordinatesGet();assert(onStep.sent.empty());assert(www.reply=="rejected");
for(int fail:{1,2}){reset();onStep.fail=fail;gotoCoordinatesGet();assert(int(onStep.sent.size())==fail);assert(www.reply=="rejected");}
reset();onStep.rejectGoto=true;gotoCoordinatesGet();assert(www.reply=="rejected");
std::cout<<"PASS: coordinate ranges, signed zero, command order, device rejection and no movement on invalid input";}
`;
const dir=fs.mkdtempSync(path.join(testOutput,'sws-coordinates-')),src=path.join(dir,'test.cpp'),exe=path.join(dir,'test.exe');
fs.writeFileSync(src,fs.readFileSync(path.join(root,'CoordinateInput.h'),'utf8')+'\n'+stub+handler+'\n'+cases);
// Windows 可能短暂占用刚生成的程序，仅对启动权限错误做有限等待
(async()=>{for(const [cmd,args]of [[process.env.CXX||'g++',['-std=c++11',src,'-o',exe]],[exe,[]]]){let r;for(let attempt=0;attempt<6;attempt++){r=cp.spawnSync(cmd,args,{encoding:'utf8'});if(r.error?.code!=='EPERM')break;await new Promise(resolve=>setTimeout(resolve,1000));}if(r.status!==0)throw Error(r.stderr||String(r.error));if(r.stdout)console.log(r.stdout);}})().catch(error=>{console.error(error);process.exit(1)});
