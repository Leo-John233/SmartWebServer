// 用可控的时间、无线状态和请求参数在电脑上验证真实配网处理逻辑
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <iostream>
#define OPERATIONAL_MODE 1
#define WIFI 1
#define WIFI_AP 2
#define WIFI_STA 1
#define HTTP_POST 1
#define WL_CONNECTED 3
#define WL_CONNECT_FAILED 4
#define WL_NO_SSID_AVAIL 1
#define WIFI_AUTH_OPEN 0
#define ESP8266
#define ENC_TYPE_NONE 0
#define STATION_WRONG_PASSWORD 2
#define STA_AUTO_RECONNECT true
#define memcpy_P memcpy
class String : public std::string {
public:
  using std::string::string;
  using std::string::operator=;
  String(const std::string &s):std::string(s){}
  String(int n):std::string(std::to_string(n)){}
  unsigned int length()const{return size();}
  long toInt()const{return std::strtol(c_str(),nullptr,10);}
};
class IPAddress {
  uint8_t bytes[4]{};
public:
  IPAddress(uint32_t n=0){for(int i=0;i<4;i++)bytes[i]=(n>>(8*i))&255;}
  IPAddress(const uint8_t *p){memcpy(bytes,p,4);}
  uint8_t &operator[](int i){return bytes[i];}
  String toString()const{return std::to_string(bytes[0])+"."+std::to_string(bytes[1])+"."+std::to_string(bytes[2])+"."+std::to_string(bytes[3]);}
};
unsigned long clockMs=0;
unsigned long millis(){return clockMs;}
struct AccessPointSettings{char ssid[32]="ONSTEPX",pwd[32]="password";unsigned char channel=7;uint8_t ip[4]={192,168,0,1},gw[4]={192,168,0,1},sn[4]={255,255,255,0};};
struct StationSettings{char host[32]{},ssid[33]="Home";bool dhcpEnabled=true;uint8_t target[4]{},ip[4]={192,168,1,55},gw[4]={192,168,1,1},sn[4]={255,255,255,0};};
struct StationPassword{char password[64]="password";};
struct WifiSettings{char masterPassword[32]="password";bool accessPointEnabled=true;AccessPointSettings ap;bool stationEnabled=false,stationApFallback=true;};
struct Manager {WifiSettings settings;StationSettings station[3];StationPassword stationPassword[3];int stationNumber=1,writes=0;std::string storedPassword;StationSettings *sta=&station[0];StationPassword *staPwd=&stationPassword[0];bool networkBusy=false,webManaged=false,stationEnabledAtBoot=false;void setStation(int n){sta=&station[n-1];staPwd=&stationPassword[n-1];}void writeSettings(){writes++;storedPassword=staPwd->password;}} wifiManager;
struct WifiFake {
  std::string startedSsid,startedPassword;
  int mode=2,connection=0,scan=-1,begins=0,disconnects=0,apFailures=0;unsigned long operationDelayMs=0;bool configOk=true,autoReconnect=true,wrongPassword=false,apCloseOk=true;uint8_t local[4]={192,168,1,55},mask[4]={255,255,255,0};
  int getMode(){return mode;}int status(){return connection;}int RSSI(int i=0){return -45-i;}
  String SSID(int i=-1){return i<0?"Home":"<img>\"\\\n";}
  IPAddress localIP(){return IPAddress(local);}IPAddress subnetMask(){return IPAddress(mask);}IPAddress softAPIP(){return IPAddress(wifiManager.settings.ap.ip);}
  bool softAP(const char*,const char*,int){if(apFailures){apFailures--;return false;}mode|=2;return true;}bool softAPConfig(IPAddress,IPAddress,IPAddress){return true;}
  bool softAPdisconnect(bool){if(!apCloseOk)return false;mode&=~2;return true;}bool enableSTA(bool v){clockMs+=operationDelayMs;if(v)mode|=1;else mode&=~1;return true;}
  void disconnect(){clockMs+=operationDelayMs;connection=0;disconnects++;}void persistent(bool){}
  void setAutoReconnect(bool value){autoReconnect=value;}
  bool config(IPAddress,IPAddress,IPAddress,IPAddress=IPAddress()){return configOk;}
  void begin(const char*ssid,const char*password){begins++;startedSsid=ssid;startedPassword=password;}void scanDelete(){}int scanNetworks(bool,bool){return -1;}int scanComplete(){return scan;}int encryptionType(int){return 0;}
} WiFi;
int wifi_station_get_connect_status(){return WiFi.wrongPassword ? STATION_WRONG_PASSWORD : 0;}
struct WebFake {
  std::map<std::string,std::string> args;int code=0,verb=HTTP_POST;String body;
  String arg(const char *name){return args[name];}int method(){return verb;}
  void sendHeader(const char*,const char*){}void send(int c,const char*,const String &b){code=c;body=b;}
  void sendContent(const char*){}void sendContentAndClear(String &s){s="";}
} www;
bool loginRequired=false;
const char html_network_wifi[]="test";
