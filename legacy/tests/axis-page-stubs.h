// 模拟设备响应，实际页面生成与提交代码保持原样
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define strcpy_P strcpy
#define strncpy_P strncpy
#define ON 1
#define OFF 0
#define DRIVE_CONFIGURATION ON
#define DISPLAY_RESET_CONTROLS OFF
#define HOME_OFFSET_RANGE_AXIS1 3600
#define HOME_OFFSET_RANGE_AXIS2 3600
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define SD_TRUE 1
struct String : std::string {
  using std::string::string;
  String(const std::string &s): std::string(s) {}
  void concat(const char *s) { append(s); }
  bool equals(const char *s) const { return *this == s; }
  int toInt() const { return atoi(c_str()); }
};
const char *EmptyStr="";
struct Web {
  std::string output;
  std::map<std::string,String> args;
  void sendContentAndClear(String &s) { output += s; s.clear(); }
  String arg(const char *key) { return args[key]; }
} www;
struct Status {
  int mountFound=1,rotatorFound=1;
  bool focuserPresent[6]={true,true,true,true,true,true};
  int getVersionMajor() {return 10;}
  int getVersionMinor() {return 28;}
} status;
struct State {char driverStatusStr[9][16]={{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"}};} state;
struct Device {
  std::vector<std::string> sent;
  bool command(const char *s,char *out) {
    int axis=0,n=0;
    if(strstr(s,",M#")){strcpy(out,"TMC2209");return true;}
    sscanf(s,":GXA%d,%d#",&axis,&n);
    if(n==0){strcpy(out,"26");return true;}
    int type=n==7?1:n==8||n==9?9:n==10||n==11?10:n>=16?5:3;
    snprintf(out,80,"1,0,256,%d,$%d",type,n);return true;
  }
  bool commandBool(const char *s){sent.push_back(s);return true;}
} onStep;
struct Convert {void stripNumericStr(char*,bool){}} convert;
void dtostrf(double value,int,int precision,char *out){sprintf(out,"%.*f",precision,value);}
void sstrcpyex(char *out,const char *in,size_t n){strncpy(out,in,n);out[n]=0;}
const char *keyValueString(const char*,const char*){return "";}
