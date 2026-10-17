
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
extern void siteTileGet()
{
  String v, v1, v2;
  int16_t i, sign;
  char temp[80];

  // Date/Time
  v = www.arg("dm");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 11))
      get_temp_month = i + 1;
  }
  v = www.arg("dd");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 1 && i <= 31))
      get_temp_day = i;
  }
  v = www.arg("dy");
  if (!v.equals(EmptyStr))
  {
    if ((convert.atoi2((char *)v.c_str(), &i)) && (i >= 2016 && i <= 9999))
    {
      get_temp_year = i - 2000;
      snprintf(temp, sizeof(temp), ":SC%02d/%02d/%02d#", get_temp_month, get_temp_day, get_temp_year);
      onStep.commandBool(temp);
    }
  }
  v = www.arg("th");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 23))
      get_temp_hour = i;
  }
  v = www.arg("tm");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 59))
      get_temp_minute = i;
  }
  v = www.arg("ts");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 59))
    {
      get_temp_second = i;
      char temp[20];
      snprintf(temp, sizeof(temp), ":SL%02d:%02d:%02d#", get_temp_hour, get_temp_minute, get_temp_second);
      onStep.commandBool(temp);
    }
  }

  // Location
  v = www.arg("g1");  // long deg
  v1 = www.arg("g2"); // long min
  if (status.getVersionMajor() > 3) v2 = www.arg("g3"); else v2 = "0"; // long sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.charAt(0) == '-') sign = '-'; else sign = '+';
    int16_t d = abs(v.toInt());
    int16_t m = v1.toInt();
    int16_t s = v2.toInt();
    if (d >= 0 && d <= 180 && m >= 0 && m <= 60 && s >= 0 && s <= 60)
    {
      if (status.getVersionMajor() > 3)
        snprintf(temp, sizeof(temp), ":Sg%c%03d*%02d:%02d#", sign, d, m, s);
      else
        snprintf(temp, sizeof(temp), ":Sg%c%03d*%02d#", sign, d, m);
      onStep.commandBool(temp);
    }
  }

  v = www.arg("t1");  // lat deg
  v1 = www.arg("t2"); // lat min
  if (status.getVersionMajor() > 3) v2 = www.arg("t3"); else v2 = "0"; // lat sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.charAt(0) == '-') sign = '-'; else sign = '+';
    int16_t d = abs(v.toInt());
    int16_t m = v1.toInt();
    int16_t s = v2.toInt();
    if (d >= 0 && d <= 90 && m >= 0 && m <= 60 && s >= 0 && s <= 60)
    {
      snprintf(temp, sizeof(temp), ":St%c%02d*%02d:%02d#", sign, d, m, s);
      onStep.commandBool(temp);
    }
  }

  v = www.arg("u1");  // UT offset hrs
  v1 = www.arg("u2"); // UT offset min
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr)) {
    if (v.charAt(0) == '-') sign = '-'; else sign = '+';
    int16_t h = abs(v.toInt());
    int16_t m = v1.toInt();
    if (v.toInt() >= -14 && v.toInt() <= 12 && (m == 0 || m == 30 || m == 45))
    {
      snprintf(temp, sizeof(temp), ":SG%c%02d:%02d#", sign, h, m);
      onStep.commandBool(temp);
    }
  }

}

extern void limitsTileGet()
{
  String v;
  char temp[80];

  // Overhead limit
  v = www.arg("ol");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 60 && v.toInt() <= 90)
    {
      snprintf(temp, sizeof(temp), ":So%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Horizon limit
  v = www.arg("hl");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -30 && v.toInt() <= 30)
    {
      snprintf(temp, sizeof(temp), ":Sh%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Meridian limit E
  v = www.arg("el");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -270 && v.toInt() <= 270)
    {
      snprintf(temp, sizeof(temp), ":SXE9,%d#", (int16_t)round((v.toInt() * 60.0) / 15.0));
      onStep.commandBool(temp);
    }
  }

  // Meridian limit W
  v = www.arg("wl");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -270 && v.toInt() <= 270)
    {
      snprintf(temp, sizeof(temp), ":SXEA,%d#", (int16_t)round((v.toInt() * 60.0) / 15.0));
      onStep.commandBool(temp);
    }
  }

  // Backlash
  v = www.arg("b1");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 0 && v.toInt() <= 3600)
    {
      snprintf(temp, sizeof(temp), ":$BR%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  v = www.arg("b2");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 0 && v.toInt() <= 3600)
    {
      snprintf(temp, sizeof(temp), ":$BD%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }
}

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
}