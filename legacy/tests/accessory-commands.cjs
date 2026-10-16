// 编译真实请求处理函数，检查网页参数到设备命令的映射与目标边界
const fs=require('node:fs'),path=require('node:path'),os=require('node:os'),cp=require('node:child_process');
const root=path.join(__dirname,'../SmartWebServer/src/pages');
const files=['focuser/SlewingTile.cpp','focuser/HomeTile.cpp','focuser/SelectTile.cpp','focuser/BacklashTcfTile.cpp','rotator/SlewingTile.cpp','rotator/HomeTile.cpp','rotator/BacklashTile.cpp','rotator/DeRotatorTile.cpp'];
const handlers=files.map(file=>fs.readFileSync(path.join(root,file),'utf8').split('// pass related data back to OnStep')[1]).join('\n');
const code=`#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <vector>
#include <cassert>
#include <cstdint>
struct String:std::string{using std::string::string;String(std::string s):std::string(s){};String(){};bool equals(const char*s){return *this==s;}long toInt(){return atol(c_str());}float toFloat(){return atof(c_str());}};
const char*EmptyStr="";
struct Web{std::map<std::string,std::string>args;String arg(const char*k){return args[k];}}www;
struct Device{std::vector<std::string>sent;void commandBlind(const char*s){sent.push_back(s);}void commandBool(const char*s){sent.push_back(s);}}onStep;
struct State{int focuserSelected=1;}state;
#define PM_HIGH 1
struct Convert{double angle=0;void doubleToDms(char*s,double n,bool,bool,int){angle=n;strcpy(s,"ANGLE");}}convert;
${handlers}
void reset(const char*k,const char*v){www.args.clear();onStep.sent.clear();www.args[k]=v;}
void expect(void(*handler)(),const char*k,const char*v,const char*serial){reset(k,v);handler();assert(onStep.sent==std::vector<std::string>{serial});}
int main(){
 const char*focusValues[]={"stop","inf","in","out","outf","vs","s","n","f","vf"};
 const char*focusCommands[]={":FQ#",":F4#:F-#",":F2#:F-#",":F2#:F+#",":F4#:F+#",":F5#",":F6#",":F7#",":F8#",":F9#"};
 const char*rotValues[]={"stop","ccwf","ccw","cw","cwf","vs","s","n","f","vf"};
 const char*rotCommands[]={":rQ#",":r3#:rc#:r<#",":r1#:rc#:r<#",":r1#:rc#:r>#",":r3#:rc#:r>#",":r5#",":r6#",":r7#",":r8#",":r9#"};
 for(int i=0;i<10;i++){expect(focuserSlewingTileGet,"foc",focusValues[i],focusCommands[i]);expect(rotatorSlewingTileGet,"rot",rotValues[i],rotCommands[i]);}
 expect(focuserHomeTileGet,"dr","Fh",":Fh#");expect(focuserHomeTileGet,"dr","FH",":FH#");
 expect(rotatorHomeTileGet,"rot","home",":rC#");expect(rotatorHomeTileGet,"rot","reset",":rF#");
 for(int i=1;i<=6;i++){std::string v="F"+std::to_string(i),command=":FA"+std::to_string(i)+"#";expect(focuserSelectTileGet,"dr",v.c_str(),command.c_str());assert(state.focuserSelected==i);}
 expect(deRotatorTileGet,"rot","on",":r+#");expect(deRotatorTileGet,"rot","off",":r-#");expect(deRotatorTileGet,"rot","rev",":rR#");expect(deRotatorTileGet,"rot","par",":rP#");
 expect(focuserBacklashTcfTileGet,"foc_bl","12",":Fb12#");expect(focuserBacklashTcfTileGet,"foc_tcf_en","1",":Fc1#");expect(focuserBacklashTcfTileGet,"foc_tcf_en","0",":Fc0#");expect(focuserBacklashTcfTileGet,"foc_tcf_db","8",":Fd8#");expect(focuserBacklashTcfTileGet,"foc_tcf_coef","-2.5",":FC-2.5#");expect(rotatorBacklashTileGet,"b3","16",":rb16#");
 for(const char*v:{"-500000","0","500000","+12500"}){std::string command=":FS"+std::to_string(atol(v))+"#";expect(focuserSlewingTileGet,"fs",v,command.c_str());}
 for(const char*v:{"-500001","500001","1.5","12garbage"," 12","12 ","1e3","99999999999999999999999999999999","","nan","+","+-1"}){reset("fs",v);focuserSlewingTileGet();assert(onStep.sent.empty());}
 for(const char*v:{"-360","0","360","30.5","-0.5","1e2","+.5"}){expect(rotatorSlewingTileGet,"rs",v,":rSANGLE#");assert(std::abs(convert.angle-strtod(v,nullptr))<1e-8);}
 for(const char*v:{"-360.1","360.1","NaN","Infinity","0x10","1,5","1garbage"," 12","12 ","1e999","","+","--1"}){reset("rs",v);rotatorSlewingTileGet();assert(onStep.sent.empty());}
 puts("PASS: actual accessory handlers, motion/stop, speed presets, reference, selector, settings and target validation");
}`;
const dir=fs.mkdtempSync(path.join(os.tmpdir(),'sws-accessory-commands-')),input=path.join(dir,'commands.cpp'),exe=path.join(dir,'commands.exe');fs.writeFileSync(input,code);
cp.execFileSync(process.env.CXX||'g++',['-std=c++11',input,'-o',exe],{stdio:'pipe'});
process.stdout.write(cp.execFileSync(exe,[],{encoding:'utf8'}));
