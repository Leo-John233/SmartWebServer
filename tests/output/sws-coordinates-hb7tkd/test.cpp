#pragma once
#include <cstdio>
#include <cstring>

// 严格检查整数坐标，禁止空值、尾随字符和越界值进入设备协议
inline bool coordinateInteger(const char *value, int maximum, int &number) {
  if (!value || !*value) return false;
  number = 0;
  for (const char *p = value; *p; ++p) {
    if (*p < '0' || *p > '9') return false;
    number = number * 10 + (*p - '0');
    if (number > maximum) return false;
  }
  return true;
}
inline bool coordinateCommands(const char *const values[7], char *ra, char *dec) {
  int h, m, s, d, dm, ds;
  if (!coordinateInteger(values[0], 23, h) || !coordinateInteger(values[1], 59, m) ||
      !coordinateInteger(values[2], 59, s) || !coordinateInteger(values[4], 90, d) ||
      !coordinateInteger(values[5], 59, dm) || !coordinateInteger(values[6], 59, ds)) return false;
  if ((strcmp(values[3], "+") && strcmp(values[3], "-")) || (d == 90 && (dm || ds))) return false;
  snprintf(ra, 24, ":Sr%02d:%02d:%02d#", h, m, s);
  snprintf(dec, 24, ":Sd%c%02d*%02d:%02d#", values[3][0], d, dm, ds);
  return true;
}

#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <iostream>
struct String:std::string{using std::string::string;bool equals(const char*s)const{return *this==s;}};
struct Web{std::map<std::string,String> args;std::string reply;String arg(const char*k){return args[k];}void sendContent(const char*s){reply+=s;}}www;
struct Status{bool onStepFound=true;}status;
struct Device{std::vector<std::string> sent;int fail=0;bool rejectGoto=false;bool commandBool(const char*s){sent.push_back(s);return int(sent.size())!=fail;}bool command(const char*s,char*out){sent.push_back(s);strcpy(out,rejectGoto?"1":"0");return true;}}onStep;
void gotoCoordinatesGet() {
  const char *names[7] = {"ra_h", "ra_m", "ra_s", "dec_sign", "dec_d", "dec_m", "dec_s"};
  String parts[7]; const char *values[7];
  for (int i = 0; i < 7; ++i) { parts[i] = www.arg(names[i]); values[i] = parts[i].c_str(); }
  char ra[24], dec[24], reply[80] = "";
  String action = www.arg("coord_action");
  if ((!action.equals("go") && !action.equals("sync")) || !coordinateCommands(values, ra, dec)) {
    www.sendContent("invalid"); return;
  }
  if (!status.onStepFound || !onStep.commandBool(ra) || !onStep.commandBool(dec)) {
    www.sendContent("rejected"); return;
  }
  bool accepted;
  if (action.equals("sync")) accepted = onStep.commandBool(":CS#");
  else accepted = onStep.command(":MS#", reply) && reply[0] == '0';
  www.sendContent(accepted ? "ok" : "rejected");
}


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
