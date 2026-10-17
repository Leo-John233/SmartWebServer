const output = require('./test-output.cjs');
const fs = require('node:fs'), path = require('node:path'), cp = require('node:child_process');
const read = file => fs.readFileSync(path.join(__dirname, '../src/pages/mount', file), 'utf8');
const site = read('SiteTile.cpp'), limits = read('LimitsTile.cpp');
const code = `
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <iostream>
struct String:std::string {
 using std::string::string;String(const std::string&s):std::string(s){}
 bool equals(const char*s)const{return *this==s;}int toInt()const{return atoi(c_str());}char charAt(size_t i)const{return i<size()?at(i):0;}
};
const char*EmptyStr="";
struct Web{std::map<std::string,String>args;String arg(const char*k){return args[k];}}www;
struct Status{int version=10;int getVersionMajor(){return version;}}status;
struct Device{std::vector<std::string>sent;void commandBool(const char*s){sent.push_back(s);}}onStep;
struct Convert{bool atoi2(char*s,int16_t*out){char*end;long n=strtol(s,&end,10);*out=n;return *s&&!(*end);}}convert;
int get_temp_month,get_temp_day,get_temp_year,get_temp_hour,get_temp_minute,get_temp_second;
${site.slice(site.indexOf('extern void siteTileGet()'))}
${limits.slice(limits.indexOf('extern void limitsTileGet()'))}
void expectSite(std::map<std::string,String> args,const char*command){www.args=args;onStep.sent.clear();siteTileGet();assert(onStep.sent==std::vector<std::string>{command});}
void expectLimit(const char*key,const char*value,const char*command){www.args={{key,value}};onStep.sent.clear();limitsTileGet();assert(onStep.sent==std::vector<std::string>{command});}
int main(){
 expectSite({{"g1","-0"},{"g2","30"},{"g3","5"}},":Sg-000*30:05#");
 expectSite({{"t1","-0"},{"t2","15"},{"t3","0"}},":St-00*15:00#");
 expectSite({{"g1","121"},{"g2","0"},{"g3","0"}},":Sg+121*00:00#");
 expectSite({{"u1","-8"},{"u2","0"}},":SG-08:00#");
 expectSite({{"u1","-13"},{"u2","0"}},":SG-13:00#");
 expectSite({{"u1","-14"},{"u2","0"}},":SG-14:00#");
 expectSite({{"u1","12"},{"u2","0"}},":SG+12:00#");
 expectSite({{"u1","-0"},{"u2","30"}},":SG-00:30#");
 expectSite({{"u1","-5"},{"u2","45"}},":SG-05:45#");
 for(const char*h:{"-15","13"}){www.args={{"u1",h},{"u2","0"}};onStep.sent.clear();siteTileGet();assert(onStep.sent.empty());}
 www.args={{"u1","0"},{"u2","15"}};onStep.sent.clear();siteTileGet();assert(onStep.sent.empty());
 expectLimit("hl","-30",":Sh-30#");expectLimit("ol","90",":So90#");
 expectLimit("el","-270",":SXE9,-1080#");expectLimit("wl","270",":SXEA,1080#");
 for(const char*k:{"hl","ol","el","wl"}){www.args={{k,"999"}};onStep.sent.clear();limitsTileGet();assert(onStep.sent.empty());}
 std::cout<<"PASS: actual site/limit handlers, coordinate signs, signed zero, UTC +13/+14, fractional zones and original limit units";
}`;
const folder = fs.mkdtempSync(path.join(output, 'sws-site-commands-'));
const source = path.join(folder, 'commands.cpp'), exe = path.join(folder, 'commands.exe');
fs.writeFileSync(source, code);
for (const [command, args] of [[process.env.CXX || 'g++', ['-std=c++11',source,'-o',exe]],[exe,[]]]) {
  const result=cp.spawnSync(command,args,{encoding:'utf8'});
  if(result.error)throw result.error;
  if(result.status!==0)throw Error(result.stderr || 'exit '+result.status);
  process.stdout.write(result.stdout);
}
