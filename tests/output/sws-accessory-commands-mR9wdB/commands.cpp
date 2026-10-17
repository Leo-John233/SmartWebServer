#include <string>
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

extern void focuserSlewingTileGet()
{
  String v;

  v = www.arg("foc");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("stop")) onStep.commandBlind(":FQ#");     // stop
    if (v.equals("inf"))  onStep.commandBlind(":F4#:F-#"); // rate fast, move in
    if (v.equals("in"))   onStep.commandBlind(":F2#:F-#"); // rate slow, move in
    if (v.equals("out"))  onStep.commandBlind(":F2#:F+#"); // rate slow, move out
    if (v.equals("outf")) onStep.commandBlind(":F4#:F+#"); // rate fast, move out

    if (v.equals("vs"))   onStep.commandBlind(":F5#");     // goto rate very slow
    if (v.equals("s"))    onStep.commandBlind(":F6#");     // goto rate slow
    if (v.equals("n"))    onStep.commandBlind(":F7#");     // goto rate normal
    if (v.equals("f"))    onStep.commandBlind(":F8#");     // goto rate fast
    if (v.equals("vf"))   onStep.commandBlind(":F9#");     // goto rate very fast
  }

  // set position
  v = www.arg("fs");
  if (!v.equals(EmptyStr))
  {
    // 必须是完整整数且同时满足上下限，避免越界或格式错误的目标被截断后发送
    const char *text = v.c_str();
    char *end;
    long p = strtol(text, &end, 10);
    bool startsNumber = (*text >= '0' && *text <= '9') || *text == '+' || *text == '-';
    if (startsNumber && end != text && *end == 0 && p >= -500000 && p <= 500000)
    {
      char temp[80];
      snprintf(temp, sizeof(temp), ":FS%ld#", p);
      onStep.commandBool(temp);
    }
  }

}


extern void focuserHomeTileGet()
{
  String v;
  v = www.arg("dr");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("FH")) onStep.commandBlind(":FH#"); // reset focuser at home (half travel)
    if (v.equals("Fh")) onStep.commandBlind(":Fh#"); // move focuser to home (half travel)
  }

}


extern void focuserSelectTileGet()
{
  String v;

  v = www.arg("dr");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("F1")) { onStep.commandBool(":FA1#"); state.focuserSelected = 1; }
    if (v.equals("F2")) { onStep.commandBool(":FA2#"); state.focuserSelected = 2; }
    if (v.equals("F3")) { onStep.commandBool(":FA3#"); state.focuserSelected = 3; }
    if (v.equals("F4")) { onStep.commandBool(":FA4#"); state.focuserSelected = 4; }
    if (v.equals("F5")) { onStep.commandBool(":FA5#"); state.focuserSelected = 5; }
    if (v.equals("F6")) { onStep.commandBool(":FA6#"); state.focuserSelected = 6; }
  }
}


extern void focuserBacklashTcfTileGet()
{
  String v;
  char cmd[32] = "";

  // send backlash
  v = www.arg("foc_bl");
  if (!v.equals(EmptyStr) && v.toInt() >= 0 && v.toInt() <= 32767) {
    snprintf(cmd, sizeof(cmd), ":Fb%d#", (int16_t)v.toInt());
    onStep.commandBool(cmd);
  }

  // send TCF en
  v = www.arg("foc_tcf_en");
  if (!v.equals(EmptyStr) && (v.equals("0") || v.equals("1"))) {
    snprintf(cmd, sizeof(cmd), ":Fc%s#", v.c_str());
    onStep.commandBool(cmd);
  }

  // send TCF deadband
  v = www.arg("foc_tcf_db");
  if (!v.equals(EmptyStr) && v.toInt() >= 1 && v.toInt() <= 32767) {
    snprintf(cmd, sizeof(cmd), ":Fd%d#", (int16_t)v.toInt());
    onStep.commandBool(cmd);
  }

  // send TCF coef
  v = www.arg("foc_tcf_coef");
  if (!v.equals(EmptyStr) && v.toFloat() >= -999.0 && v.toFloat() <= 999.0) {
    snprintf(cmd, sizeof(cmd), ":FC%s#", v.c_str());
    onStep.commandBool(cmd);
  }
}


extern void rotatorSlewingTileGet()
{
  String v;

  // stop movement
  v = www.arg("rot");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("stop")) onStep.commandBlind(":rQ#");         // stop
    if (v.equals("ccwf")) onStep.commandBlind(":r3#:rc#:r<#"); // rate 3, move ccw
    if (v.equals("ccw"))  onStep.commandBlind(":r1#:rc#:r<#"); // rate 1, move ccw
    if (v.equals("cw"))   onStep.commandBlind(":r1#:rc#:r>#"); // rate 1, move cw
    if (v.equals("cwf"))  onStep.commandBlind(":r3#:rc#:r>#"); // rate 3, move cw

    if (v.equals("vs"))   onStep.commandBlind(":r5#");         // goto rate very slow
    if (v.equals("s"))    onStep.commandBlind(":r6#");         // goto rate slow
    if (v.equals("n"))    onStep.commandBlind(":r7#");         // goto rate normal
    if (v.equals("f"))    onStep.commandBlind(":r8#");         // goto rate fast
    if (v.equals("vf"))   onStep.commandBlind(":r9#");         // goto rate very fast
  }

  // set position
  v = www.arg("rs");
  if (!v.equals(EmptyStr))
  {
    // 与网页数值输入一致，仅接受完整十进制数和有效角度范围
    const char *text = v.c_str();
    char *end;
    bool decimal = true;
    for (const char *c = text; *c; c++) {
      if (!((*c >= '0' && *c <= '9') || *c == '+' || *c == '-' || *c == '.' || *c == 'e' || *c == 'E')) decimal = false;
    }
    double f = strtod(text, &end);
    if (decimal && end != text && *end == 0 && f >= -360.0 && f <= 360.0)
    {
      char temp[80], temp1[40];
      convert.doubleToDms(temp1, f, true, true, PM_HIGH);
      snprintf(temp, sizeof(temp), ":rS%s#", temp1);
      onStep.commandBool(temp);
    }
  }

}


extern void rotatorHomeTileGet()
{
  String v;
  v = www.arg("rot");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("reset")) onStep.commandBlind(":rF#"); // reset rotator at home
    if (v.equals("home"))  onStep.commandBlind(":rC#"); // move rotator to home
  }
}


extern void rotatorBacklashTileGet()
{
  String v;
  char temp[32] = "";

  v = www.arg("b3");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 0 && v.toInt() <= 32767) { 
      snprintf(temp, sizeof(temp), ":rb%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

}


extern void deRotatorTileGet()
{
  String v;
  v = www.arg("rot");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("off")) onStep.commandBlind(":r-#"); // disable derotator
    if (v.equals("on"))  onStep.commandBlind(":r+#"); // enable derotator
    if (v.equals("rev")) onStep.commandBlind(":rR#"); // reverse rotator
    if (v.equals("par")) onStep.commandBlind(":rP#"); // move rotator to parallactic angle
  }

}

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
}