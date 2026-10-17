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

#define DISPLAY_LANGUAGE L_cn
// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define PROD_ABV                     "SWS"

#define METRIC                       ON
#define IMPERIAL                     -200
#define LOCALE_DEFAULT               -201
#define AUTO_ON                      -202
#define AUTO_OFF                     -203
#define FWU                          -204

// ajax page update feature
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 500     // fast update rate
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 5000    // time before return to normal update rate
#define AJAX_PAGE_LAZY_GET_MS         1000    // wait time for lazy get

// various auxillary features
#define SWITCH                       1
#define ANALOG_OUTPUT                2
#define DEW_HEATER                   3
#define INTERVALOMETER               4
#define COVER_SWITCH                 7

// The settings below are for initialization only, afterward they are stored and recalled from EEPROM and must
// be changed in the web interface OR with a reset (for initialization again) as described in the Config.h comments
#define TIMEOUT_WEB                  200
#define TIMEOUT_COMMAND              200

// task manager
#define TASKS_MAX                    40      // up to 40 tasks
#define TASKS_SKIP_MISSED

// NV -------------------------------------------------------------------------------------------------------------------

// unique volume signature for this volume/partition layout
#define NV_VOLUME_SIGNATURE         0x0001u

// -----------------------------------------------------------------------------------
// Locales
#pragma once

// see Strings_xx.h for individual locale translations

// languages (one unique numeric code required for each)
#define L_ca 1 // Catalan
#define L_cn 2 // Chinese
#define L_de 3 // German
#define L_en 4 // English
#define L_es 5 // Spanish
#define L_fr 6 // French
#define L_it 7 // Italian
#define L_jp 8 // Japanese
#define L_us 9 // English (US, imperial units)
#define L_ro 10 // Romanian

// -----------------------------------------------------------------------------------
// Locale
#pragma once




// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_ca
#include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ca.h"
#define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_cn
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_cn.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_de
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_es
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_es.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_fr
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_fr.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_it
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_it.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_jp
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_jp.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS IMPERIAL
#endif
#if DISPLAY_LANGUAGE == L_ro
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ro.h"
  #define UNITS METRIC
#endif

// 未提供专用停止文字时沿用当前语言的停止描述
#ifndef BUTTON_STOP
  #define BUTTON_STOP L_STOP
#endif

// 未提供本地翻译时保留通用同步文字
#ifndef L_COORD_SYNC
  #define L_COORD_SYNC "Sync"
#endif

// misc. locale support functions
#ifndef DISPLAY_UNITS
  #define DISPLAY_UNITS UNITS
#else
  #if DISPLAY_UNITS == LOCALE_DEFAULT
    #undef DISPLAY_UNITS
    #define DISPLAY_UNITS UNITS
  #endif
#endif

#if DISPLAY_UNITS == IMPERIAL
  #define nativeToCelsius(t) ((t - 32.0F)*(5.0F/9.0F))
  #define celsiusToNative(t) (t*(9.0F/5.0F) + 32.0F)
  #define nativeToCelsiusRelative(t) (t*(5.0F/9.0F))
  #define celsiusToNativeRelative(t) (t*(9.0F/5.0F))
  #define TEMPERATURE_UNITS_ABV "F"
#else
  #define nativeToCelsius(t) (t)
  #define celsiusToNative(t) (t)
  #define nativeToCelsiusRelative(t) (t)
  #define celsiusToNativeRelative(t) (t)
  #define TEMPERATURE_UNITS_ABV "C"
#endif

// 其他语言未提供新标签时使用默认文字
#ifndef L_HOME_ACTIONS
#define L_HOME_ACTIONS "Home"
#endif
#ifndef L_PARK_ACTIONS
#define L_PARK_ACTIONS "Parking"
#endif
#ifndef L_COORD_GOTO
#define L_COORD_GOTO "Coordinate GOTO"
#endif
#ifndef L_COORD_SYSTEM
#define L_COORD_SYSTEM "Coordinate system"
#endif
#ifndef L_COORD_JNOW
#define L_COORD_JNOW "RA / Dec (JNOW)"
#endif
#ifndef L_COORD_RA
#define L_COORD_RA "RA"
#endif
#ifndef L_COORD_DEC
#define L_COORD_DEC "DEC"
#endif
#ifndef L_COORD_SIGN
#define L_COORD_SIGN "Sign"
#endif
#ifndef L_COORD_H
#define L_COORD_H "h"
#endif
#ifndef L_COORD_D
#define L_COORD_D "deg"
#endif
#ifndef L_COORD_M
#define L_COORD_M "min"
#endif
#ifndef L_COORD_S
#define L_COORD_S "sec"
#endif
#ifndef L_COORD_SEND
#define L_COORD_SEND "Sending coordinates"
#endif
#ifndef L_COORD_OK
#define L_COORD_OK "Command accepted"
#endif
#ifndef L_COORD_REJECT
#define L_COORD_REJECT "Command rejected, check device status"
#endif
#ifndef L_COORD_INVALID
#define L_COORD_INVALID "Enter valid coordinates; minutes and seconds must be zero at 90 degrees"
#endif
#ifndef L_COORD_NETWORK
#define L_COORD_NETWORK "Connection lost, check device before retrying"
#endif
#ifndef L_CAT_PREV
#define L_CAT_PREV "Previous"
#endif
#ifndef L_CAT_NEXT
#define L_CAT_NEXT "Next"
#endif
#ifndef L_CAT_TARGET
#define L_CAT_TARGET "Set target"
#endif
#ifndef L_CAT_HELP
#define L_CAT_HELP "Catalog help"
#endif

#ifndef L_COORD_AZ
#define L_COORD_AZ "Azimuth"
#endif
#ifndef L_COORD_ALT
#define L_COORD_ALT "Altitude"
#endif
#ifndef L_GOTO_ACTION
#define L_GOTO_ACTION "GOTO"
#endif

#ifndef L_GOTO_TARGET
#define L_GOTO_TARGET "Go to current target"
#endif
#ifndef L_SYNC_TARGET
#define L_SYNC_TARGET "Sync current target"
#endif

#ifndef L_TRK_AXES
#define L_TRK_AXES "Compensation axes"
#endif

#ifndef L_DEVICE_TRACKING
#define L_DEVICE_TRACKING "Device tracking"
#endif

#ifndef L_SIDEBAR_NOTE
#define L_SIDEBAR_NOTE "OnStep smart control"
#endif



#if OPERATIONAL_MODE == WIFI


#include <stdint.h>

// 只决定连接时序，不接触网页、密码存储或无线接口，主循环执行返回的动作。
namespace swsWifi {
class StationConnection {
  public:
    enum Action { Wait, Start, Retry, Success, Failed };

    void begin(uint32_t now) {
      started = phaseAt = now;
      attempts = 0;
      settling = true;
      bad = false;
    }

    Action poll(uint32_t now, bool connected, bool error, bool hasAddress) {
      if (!settling && connected && hasAddress) return Success;
      if (uint32_t(now - started) >= 30000) return Failed;
      if (settling) {
        if (uint32_t(now - phaseAt) < 350) return Wait;
        settling = false;
        phaseAt = now;
        bad = false;
        attempts++;
        return Start;
      }
      if (error) {
        if (!bad) { bad = true; badAt = now; }
      } else bad = false;
      // 忽略启动初期及短暂的失败状态，持续失败或首次等待超时才重试一次。
      const bool rejected = bad && uint32_t(now - phaseAt) >= 3000 && uint32_t(now - badAt) >= 1500;
      const bool expired = uint32_t(now - phaseAt) >= 15000;
      if (!rejected && !expired) return Wait;
      if (attempts >= 2) return Failed;
      settling = true;
      phaseAt = now;
      bad = false;
      return Retry;
    }

  private:
    uint32_t started = 0, phaseAt = 0, badAt = 0;
    uint8_t attempts = 0;
    bool settling = false, bad = false;
};
}



#include <stdint.h>

// 已保存网络的后台恢复策略，独立于单次连接尝试和网页操作。
namespace swsWifi {
class StationReconnect {
  public:
    void init(bool saved, uint32_t now) { knownNetwork = saved; blocked = false; failures = 0; next = now + 2000; }
    void online(uint32_t now) { knownNetwork = true; blocked = false; failures = 0; next = now + 2000; }
    void lost(uint32_t now) { next = now + 2000; }
    void defer(uint32_t now) { next = now + 5000; }
    void failed(uint32_t now, bool permanent) {
      blocked = blocked || permanent;
      const uint32_t delay = failures >= 4 ? 60000 : 5000UL << failures;
      if (failures < 4) failures++;
      next = now + delay;
    }
    bool due(uint32_t now) const { return knownNetwork && !blocked && int32_t(now - next) >= 0; }
    bool known() const { return knownNetwork; }

  private:
    bool knownNetwork = false, blocked = false;
    uint8_t failures = 0;
    uint32_t next = 0;
};
}


extern bool loginRequired;

namespace {
bool scanning = false, scanAddedSta = false, connecting = false, failed = false;
bool networkAdopted = false, backgroundConnecting = false, wasConnected = false;
bool stationStarted = false, authenticationRejected = false;
uint8_t pending = 0; // 1 表示连接路由器，2 表示设置热点，先返回网页响应再切换网络
unsigned long scheduledAt = 0, scanStarted = 0;
swsWifi::StationConnection stationConnection;
swsWifi::StationReconnect stationReconnect, previousReconnect;
// 上游将路由器参数和密码独立保存，提交时一起暂存并在应用时更新
WifiSettings proposed;
StationSettings proposedStation;
StationPassword proposedPassword;
StationSettings previousStation;
StationPassword previousPassword;
bool stationCandidate = false;
String notice;
const char *noticeTarget = "sta";

// 待验证参数只供当前连接使用，其他设置写入时仍看到原来的已保存凭据。
StationSettings &connectionStation() { return stationCandidate ? proposedStation : *wifiManager.sta; }
StationPassword &connectionPassword() { return stationCandidate ? proposedPassword : *wifiManager.staPwd; }

// 转义网络名称中的引号和控制字符，保证扫描结果始终是合法 JSON
String quote(const String &value) {
  String out = "\"";
  for (unsigned int i = 0; i < value.length(); i++) {
    unsigned char c = value[i];
    if (c == '"' || c == '\\') { out += '\\'; out += (char)c; }
    else if (c < 32) { char escaped[7]; snprintf(escaped, sizeof(escaped), "\\u%04x", c); out += escaped; }
    else out += (char)c;
  }
  return out + '"';
}
void reply(int code, const String &body) {
  www.sendHeader("Cache-Control", "no-store");
  www.send(code, "application/json; charset=utf-8", body);
}
void error(int code, const String &message) { reply(code, "{\"error\":" + quote(message) + "}"); }
bool apActive() { return (WiFi.getMode() & WIFI_AP) != 0; }
String ipText(const uint8_t *ip) { return IPAddress(ip).toString(); }
uint32_t ipNumber(const uint8_t *ip) {
  return ((uint32_t)ip[0] << 24) | ((uint32_t)ip[1] << 16) | ((uint32_t)ip[2] << 8) | ip[3];
}
// 比较两个网段是否重叠，避免热点和路由器使用同一地址范围
bool overlap(const uint8_t *a, const uint8_t *am, const uint8_t *b, const uint8_t *bm) {
  uint32_t mask = ipNumber(am) & ipNumber(bm);
  return (ipNumber(a) & mask) == (ipNumber(b) & mask);
}
bool parseIp(const String &s, uint8_t *out) {
  unsigned int index = 0;
  for (int octet = 0; octet < 4; octet++) {
    int number = 0, digits = 0;
    while (index < s.length() && s[index] >= '0' && s[index] <= '9') {
      number = number * 10 + s[index++] - '0';
      if (++digits > 3 || number > 255) return false;
    }
    if (!digits) return false;
    out[octet] = number;
    if (octet < 3 && (index >= s.length() || s[index++] != '.')) return false;
  }
  return index == s.length();
}
// 同时检查地址格式、连续子网掩码和网关所属网段
bool addresses(uint8_t *ip, uint8_t *gw, uint8_t *sn) {
  if (!parseIp(www.arg("ip"), ip) || !parseIp(www.arg("gw"), gw) || !parseIp(www.arg("sn"), sn)) return false;
  uint32_t mask = ipNumber(sn), host = ipNumber(ip) & ~mask;
  uint32_t inverse = ~mask;
  return mask && inverse >= 3 && !(inverse & (inverse + 1)) && host && host != inverse &&
    ip[0] > 0 && ip[0] < 224 && ip[0] != 127 && gw[0] > 0 && gw[0] < 224 &&
    (ipNumber(ip) & mask) == (ipNumber(gw) & mask);
}
bool startAp() {
  AccessPointSettings &a = wifiManager.settings.ap;
  if (!WiFi.softAP(a.ssid, a.pwd, a.channel)) return false;
  return WiFi.softAPConfig(IPAddress(a.ip), IPAddress(a.gw), IPAddress(a.sn));
}
void endScan() {
  WiFi.scanDelete();
  scanning = false;
  if (scanAddedSta) WiFi.enableSTA(false);
  scanAddedSta = false;
  wifiManager.networkBusy = false;
}
// 发起连接后立即返回，由主循环判断成功或超时
bool configureStation() {
  stationStarted = authenticationRejected = wasConnected = false;
  // 网页连接由本状态机管理，避免 SDK 重连与网页重试同时争用无线接口。
  WiFi.setAutoReconnect(false);
  if (!WiFi.enableSTA(true)) return false;
  WiFi.disconnect();
  // 断开和重新配置分两个主循环阶段，等待 SDK 完成旧连接的清理。
  stationConnection.begin(millis());
  connecting = true;
  wifiManager.networkBusy = true;
  failed = false;
  notice = L_UI_CONNECTING_NOTE;
  noticeTarget = "sta";
  return true;
}
void connectionFailed(const String &reason, bool retryable = true) {
  const bool wasBackground = backgroundConnecting;
  const bool restoreDifferentConfiguration = stationCandidate &&
    (strcmp(proposedStation.ssid, previousStation.ssid) ||
     strcmp(proposedPassword.password, previousPassword.password) ||
     proposedStation.dhcpEnabled != previousStation.dhcpEnabled ||
     memcmp(proposedStation.ip, previousStation.ip, 4) ||
     memcmp(proposedStation.gw, previousStation.gw, 4) ||
     memcmp(proposedStation.sn, previousStation.sn, 4));
  connecting = false;
  failed = true;
  backgroundConnecting = false;
  wifiManager.networkBusy = false;
  WiFi.setAutoReconnect(false);
  WiFi.disconnect();
  // 结束失败的 STA 尝试，保留 AP，停止错误凭据引起的后台扫描和重连。
  WiFi.enableSTA(false);
  // 回滚未验证的凭据，后续保存热点等设置时也不能把错误密码一并写入。
  if (stationCandidate) {
    stationCandidate = false;
    stationReconnect = previousReconnect;
  }
  stationReconnect.failed(millis(), (authenticationRejected || !retryable) && !restoreDifferentConfiguration);
  bool available = apActive() || startAp();
  const String feedback = authenticationRejected ? String(L_UI_WIFI_AUTH_FAILED) :
    wasBackground && retryable ? String(L_UI_RECONNECT_PENDING) : reason;
  notice = feedback + (available ? L_UI_AP_AVAILABLE : L_UI_AP_FAILED);
  noticeTarget = "sta";
}
// 页面只读取配置和运行状态，不返回任何保存的密码
String configJson() {
  StationSettings &s = connectionStation();
  AccessPointSettings &a = wifiManager.settings.ap;
  String json = "{\"sta\":{\"enabled\":";
  json += wifiManager.settings.stationEnabled ? "true" : "false";
  json += ",\"ssid\":" + quote(s.ssid) + ",\"dhcp\":" + (s.dhcpEnabled ? "true" : "false");
  json += ",\"open\":"; json += connectionPassword().password[0] == 0 ? "true" : "false";
  json += ",\"remembered\":"; json += !stationCandidate && strlen(wifiManager.staPwd->password) >= 8 ? "true" : "false";
  json += ",\"known\":"; json += stationReconnect.known() ? "true" : "false";
  json += ",\"ip\":" + quote(ipText(s.ip)) + ",\"gw\":" + quote(ipText(s.gw)) + ",\"sn\":" + quote(ipText(s.sn)) + "},\"ap\":{\"enabled\":";
  json += wifiManager.settings.accessPointEnabled ? "true" : "false";
  json += ",\"ssid\":" + quote(a.ssid) + ",\"channel\":" + String(a.channel);
  json += ",\"ip\":" + quote(ipText(a.ip)) + ",\"gw\":" + quote(ipText(a.gw)) + ",\"sn\":" + quote(ipText(a.sn)) + "}";
  json += ",\"connected\":"; json += WiFi.status() == WL_CONNECTED ? "true" : "false";
  json += ",\"connecting\":"; json += connecting ? "true" : "false";
  json += ",\"background\":"; json += backgroundConnecting ? "true" : "false";
  json += ",\"pending\":"; json += pending ? "true" : "false";
  json += ",\"failed\":"; json += failed ? "true" : "false";
  json += ",\"apActive\":"; json += apActive() ? "true" : "false";
  json += ",\"ssid\":" + quote(WiFi.SSID()) + ",\"ip\":" + quote(WiFi.localIP().toString());
  json += ",\"rssi\":" + String(WiFi.RSSI()) + ",\"apIp\":" + quote(WiFi.softAPIP().toString());
  json += ",\"apSsid\":" + quote(a.ssid) + ",\"messageTarget\":\"" + String(noticeTarget) + "\",\"message\":" + quote(notice) + "}";
  return json;
}
}

void handleWifiNetwork() {
  // 沿用网络页面的登录状态，未登录时不能扫描或修改配置
  if (loginRequired) { error(403, L_UI_LOGIN_FIRST); return; }
  String action = www.arg("action");
  if (action == "status") { reply(200, configJson()); return; }
  if (www.method() != HTTP_POST) { error(405, L_UI_POST_REQUIRED); return; }
  if (backgroundConnecting && (action == "scan" || action == "connect" || action == "ap")) {
    // 用户操作优先，取消后台尝试后再处理；旧凭据和热点均保留。
    WiFi.disconnect();
    connecting = backgroundConnecting = false;
    wifiManager.networkBusy = false;
    stationReconnect.defer(millis());
  }
  if (pending || connecting) { error(409, L_UI_BUSY); return; }

  if (action == "scan") {
    if (!scanning) {
      WiFi.persistent(false);
      scanAddedSta = !(WiFi.getMode() & WIFI_STA);
      WiFi.enableSTA(true); // 保留热点和已有路由器连接，只开启扫描需要的无线接口
      wifiManager.networkBusy = true;
      WiFi.scanDelete();
      int result = WiFi.scanNetworks(true, false);
      if (result == -2) { endScan(); error(503, L_UI_SCAN_START_FAILED); return; }
      scanning = true; scanStarted = millis();
    }
    int count = WiFi.scanComplete();
    if (count == -1) { reply(200, "{\"running\":true}"); return; }
    if (count < 0) { endScan(); error(503, L_UI_SCAN_FAILED); return; }
    String json = "{\"running\":false,\"networks\":[";
    // 最多返回 24 条结果，限制密集网络环境下的内存占用
    int limit = count < 24 ? count : 24;
    for (int i = 0; i < limit; i++) {
      if (i) json += ',';
      json += "{\"ssid\":" + quote(WiFi.SSID(i)) + ",\"rssi\":" + String(WiFi.RSSI(i));
      #ifdef ESP8266
        bool open = WiFi.encryptionType(i) == ENC_TYPE_NONE;
      #else
        bool open = WiFi.encryptionType(i) == WIFI_AUTH_OPEN;
      #endif
      json += ",\"open\":"; json += open ? "true}" : "false}";
    }
    json += "]}"; endScan(); reply(200, json); return;
  }
  if (scanning) { error(409, L_UI_SCAN_WAIT); return; }
  if (action != "connect" && action != "ap") { error(400, L_UI_UNKNOWN_ACTION); return; }
  proposed = wifiManager.settings;
  proposedStation = *wifiManager.sta;
  proposedPassword = *wifiManager.staPwd;
  bool enabled = www.arg("enabled") == "1";
  String ssid = www.arg("ssid"), password = www.arg("password");
  if (ssid.length() == 0 || ssid.length() > 31 || password.length() > (action == "connect" ? 63U : 31U)) {
    error(400, L_UI_CREDENTIAL_LENGTH); return;
  }
  if (action == "connect") {
    StationSettings &s = proposedStation;
    char *savedPassword = proposedPassword.password;
    bool open = www.arg("open") == "1";
    if (enabled && !open && ((password.length() && password.length() < 8) ||
        (!password.length() && (ssid != s.ssid || strlen(savedPassword) < 8)))) {
      error(400, L_UI_PASSWORD_REQUIRED); return;
    }
    proposed.stationEnabled = enabled;
    s.dhcpEnabled = www.arg("dhcp") == "1";
    if (!s.dhcpEnabled && !addresses(s.ip, s.gw, s.sn)) { error(400, L_UI_BAD_STATIC); return; }
    if (enabled && !s.dhcpEnabled && overlap(s.ip, s.sn, proposed.ap.ip, proposed.ap.sn)) {
      error(400, L_UI_STA_OVERLAP); return;
    }
    if (!enabled && !proposed.accessPointEnabled) { error(400, L_UI_ENABLE_AP_FIRST); return; }
    // 关闭状态下更换网络名称时清除旧密码，防止下次误用其他网络的密码
    bool changedNetwork = ssid != s.ssid;
    strcpy(s.ssid, ssid.c_str());
    if (open) savedPassword[0] = 0;
    else if (password.length()) strcpy(savedPassword, password.c_str());
    else if (changedNetwork) savedPassword[0] = 0;
    // 查看已连接网络后原样保存，不断开正常连接，也不重复写入相同设置。
    if (enabled && WiFi.status() == WL_CONNECTED && WiFi.SSID() == s.ssid &&
        !changedNetwork && !strcmp(savedPassword, wifiManager.staPwd->password) &&
        s.dhcpEnabled == wifiManager.sta->dhcpEnabled &&
        (s.dhcpEnabled || (!memcmp(s.ip, wifiManager.sta->ip, 4) &&
          !memcmp(s.gw, wifiManager.sta->gw, 4) && !memcmp(s.sn, wifiManager.sta->sn, 4)))) {
      if (!wifiManager.settings.stationEnabled) {
        wifiManager.settings.stationEnabled = true;
        wifiManager.writeSettings();
      }
      failed = false;
      notice = L_UI_CONNECTED;
      noticeTarget = "sta";
      reply(200, "{\"message\":" + quote(notice) + "}");
      return;
    }
    pending = 1;
  } else {
    AccessPointSettings &a = proposed.ap;
    if (password.length() && password.length() < 8) { error(400, L_UI_AP_PASSWORD_LENGTH); return; }
    if (!enabled && (WiFi.status() != WL_CONNECTED || !proposed.stationEnabled)) {
      error(400, L_UI_CONNECT_FIRST); return;
    }
    if (!addresses(a.ip, a.gw, a.sn)) { error(400, L_UI_BAD_AP_IP); return; }
    if (enabled && WiFi.status() == WL_CONNECTED) {
      IPAddress ip = WiFi.localIP(), mask = WiFi.subnetMask();
      if (overlap(a.ip, a.sn, &ip[0], &mask[0])) { error(400, L_UI_AP_OVERLAP); return; }
    }
    String channel = www.arg("channel");
    int ch = channel.toInt();
    if (ch < 1 || ch > 11 || String(ch) != channel) { error(400, L_UI_BAD_CHANNEL); return; }
    strcpy(a.ssid, ssid.c_str());
    if (password.length()) strcpy(a.pwd, password.c_str());
    a.channel = ch;
    proposed.accessPointEnabled = enabled;
    pending = 2;
  }
  scheduledAt = millis(); failed = false;
  noticeTarget = pending == 2 ? "ap" : "sta";
  notice = L_UI_APPLYING;
  reply(202, "{\"message\":" + quote(notice) + "}");
}

void pollWifiNetwork() {
  unsigned long now = millis();
  if (!networkAdopted) {
    networkAdopted = true;
    wifiManager.webManaged = true;
    WiFi.persistent(false);
    WiFi.setAutoReconnect(false);
    const bool saved = wifiManager.stationEnabledAtBoot || wifiManager.settings.stationEnabled;
    stationReconnect.init(saved, now);
    if (saved) wifiManager.settings.stationEnabled = true;
    wasConnected = WiFi.status() == WL_CONNECTED;
    if (wasConnected) stationReconnect.online(now);
  }
  if (!connecting && !pending) {
    const bool linked = WiFi.status() == WL_CONNECTED;
    if (linked && !wasConnected) stationReconnect.online(now);
    if (!linked && wasConnected) {
      stationReconnect.lost(now);
      if (wifiManager.settings.stationEnabled && !apActive()) startAp();
    }
    wasConnected = linked;
  }
  // 浏览器关闭后也会释放扫描缓存，并恢复原来的无线模式
  if (scanning && now - scanStarted >= 30000) endScan();
  if (pending && now - scheduledAt >= 1000) {
    uint8_t operation = pending; pending = 0;
    // 延迟应用期间若路由器掉线，取消关闭热点，保留可用入口
    if (operation == 2 && !proposed.accessPointEnabled && WiFi.status() != WL_CONNECTED) {
      failed = true;
      notice = L_UI_ROUTER_LOST;
      return;
    }
    const WifiSettings previousSettings = wifiManager.settings;
    const bool previousApActive = apActive();
    wifiManager.settings = proposed;
    wifiManager.setStation(wifiManager.stationNumber);
    if (operation == 1) {
      previousReconnect = stationReconnect;
      previousStation = *wifiManager.sta;
      previousPassword = *wifiManager.staPwd;
      stationCandidate = proposed.stationEnabled;
      if (!stationCandidate) {
        *wifiManager.sta = proposedStation;
        *wifiManager.staPwd = proposedPassword;
      }
    }
    WiFi.persistent(false);
    if (operation == 1) {
      backgroundConnecting = false;
      wifiManager.webManaged = true;
      // 配网期间保留热点，连接失败后仍能通过热点修改设置
      if (wifiManager.settings.stationEnabled) {
        if (!apActive()) startAp();
        if (!configureStation()) connectionFailed(L_UI_APPLY_IP_FAILED, false);
      } else {
        WiFi.disconnect(); WiFi.enableSTA(false);
        if (!apActive()) startAp();
        notice = L_UI_STA_DISCONNECTED;
        wifiManager.writeSettings();
      }
    } else {
      const bool applied = wifiManager.settings.accessPointEnabled ? startAp() : WiFi.softAPdisconnect(true);
      if (!applied) {
        // 无线接口拒绝设置时恢复旧配置，失败的热点参数不能写入 Flash。
        wifiManager.settings = previousSettings;
        if (previousApActive) startAp(); else WiFi.softAPdisconnect(true);
        failed = true;
        notice = L_UI_AP_APPLY_FAILED;
        return;
      }
      notice = wifiManager.settings.accessPointEnabled ? L_UI_AP_SAVED : L_UI_AP_CLOSED;
    }
    // 连接参数验证成功后再持久化，失败密码不能覆盖原有可用配置。
    if (operation == 2) wifiManager.writeSettings();
  }
  if (connecting) {
    const int connection = WiFi.status();
    if (stationStarted && connection == WL_CONNECT_FAILED) {
      #ifdef ESP8266
        authenticationRejected = authenticationRejected || wifi_station_get_connect_status() == STATION_WRONG_PASSWORD;
      #endif
    }
    IPAddress ip = WiFi.localIP(), mask = WiFi.subnetMask();
    // SDK 切换模式和断开连接会消耗时间，不能用本轮开始时的旧时间减去新起点。
    const uint32_t connectionNow = millis();
    const swsWifi::StationConnection::Action action = stationConnection.poll(connectionNow,
      connection == WL_CONNECTED, connection == WL_CONNECT_FAILED || connection == WL_NO_SSID_AVAIL,
      ipNumber(&ip[0]) != 0 && ipNumber(&mask[0]) != 0);
    if (action == swsWifi::StationConnection::Start) {
      StationSettings &s = connectionStation();
      IPAddress zero((uint32_t)0);
      const bool configured = s.dhcpEnabled ? WiFi.config(zero, zero, zero) :
        WiFi.config(IPAddress(s.ip), IPAddress(s.gw), IPAddress(s.sn), IPAddress(s.gw));
      if (!configured) connectionFailed(L_UI_APPLY_IP_FAILED, false);
      else { stationStarted = true; WiFi.begin(s.ssid, connectionPassword().password); }
    } else if (action == swsWifi::StationConnection::Retry) {
      if (authenticationRejected) connectionFailed(L_UI_WIFI_AUTH_FAILED);
      else {
        WiFi.disconnect();
        stationStarted = false;
        notice = L_UI_CONNECTION_RETRY;
      }
    } else if (action == swsWifi::StationConnection::Success) {
      AccessPointSettings &a = wifiManager.settings.ap;
      if (apActive() && overlap(a.ip, a.sn, &ip[0], &mask[0])) {
        connectionFailed(L_UI_DHCP_OVERLAP, false);
      } else {
        connecting = false; wifiManager.networkBusy = false; failed = false;
        backgroundConnecting = false;
        wasConnected = true;
        stationReconnect.online(connectionNow);
        notice = L_UI_CONNECTED_ADDRESS + WiFi.localIP().toString();
        if (stationCandidate) {
          *wifiManager.sta = proposedStation;
          *wifiManager.staPwd = proposedPassword;
          stationCandidate = false;
          wifiManager.writeSettings();
        }
      }
    } else if (action == swsWifi::StationConnection::Failed) {
      connectionFailed(L_UI_CONNECT_FAILED);
    }
  }
  #if STA_AUTO_RECONNECT == true
    // 已保存网络持续采用退避重试，后台尝试可被手动操作打断。
    if (stationReconnect.due(now) && wifiManager.settings.stationEnabled && !connecting && !scanning && !pending &&
        WiFi.status() != WL_CONNECTED) {
      backgroundConnecting = true;
      if (!apActive()) startAp();
      if (!configureStation()) connectionFailed(L_UI_APPLY_IP_FAILED, false);
    }
  #endif
}
#endif

void request(const char *action){www.args["action"]=action;www.code=0;handleWifiNetwork();}
void reset(){pending=0;connecting=false;scanning=false;failed=false;scanAddedSta=false;networkAdopted=backgroundConnecting=wasConnected=stationStarted=authenticationRejected=false;stationReconnect=swsWifi::StationReconnect();previousReconnect=swsWifi::StationReconnect();stationCandidate=false;notice="";noticeTarget="sta";clockMs=0;wifiManager.settings=WifiSettings();wifiManager.station[0]=StationSettings();wifiManager.stationPassword[0]=StationPassword();wifiManager.setStation(1);wifiManager.writes=0;wifiManager.storedPassword.clear();wifiManager.networkBusy=wifiManager.webManaged=wifiManager.stationEnabledAtBoot=false;WiFi=WifiFake();www=WebFake();loginRequired=false;}
void station(){www.args={{"ssid","New Home"},{"password","new-pass-123"},{"enabled","1"},{"dhcp","1"}};}
void apRequest(){www.args={{"ssid","ONSTEPX"},{"password",""},{"enabled","1"},{"channel","7"},{"ip","192.168.0.1"},{"gw","192.168.0.1"},{"sn","255.255.255.0"}};}
void advance(unsigned long ms){clockMs+=ms;pollWifiNetwork();}
void startConnection(){advance(1001);advance(350);}
int main(){
reset();loginRequired=true;request("scan");assert(www.code==403&&WiFi.mode==2);loginRequired=false;www.verb=0;request("connect");assert(www.code==405);
reset();request("status");assert(www.code==200&&www.body.find("password")==std::string::npos&&www.body.find("\"messageTarget\":\"sta\"")!=std::string::npos);
reset();request("scan");assert(scanning&&WiFi.mode==3&&wifiManager.networkBusy&&WiFi.disconnects==0);WiFi.scan=2;request("scan");assert(www.code==200&&!scanning&&WiFi.mode==2&&!wifiManager.networkBusy);assert(www.body.find("\\\"")!=std::string::npos);
reset();WiFi.mode=3;WiFi.connection=3;request("scan");WiFi.scan=0;request("scan");assert(WiFi.mode==3&&WiFi.connection==3&&WiFi.disconnects==0);
reset();request("scan");clockMs=30000;pollWifiNetwork();assert(!scanning&&WiFi.mode==2&&!wifiManager.networkBusy);
reset();station();www.args["password"]="short";request("connect");assert(www.code==400&&pending==0&&wifiManager.writes==0);
reset();station();www.args["ssid"]=std::string(32,'a');request("connect");assert(www.code==400);
reset();station();www.args["password"]="";request("connect");assert(www.code==400);www.args["ssid"]="Home";request("connect");assert(www.code==202);startConnection();assert(std::string(wifiManager.staPwd->password)=="password"&&WiFi.begins==1);
reset();station();www.args["password"]="";www.args["open"]="1";request("connect");assert(www.code==202&&proposedPassword.password[0]==0);
reset();station();www.args["dhcp"]="0";www.args["ip"]="192.168.0.55";www.args["gw"]="192.168.0.1";www.args["sn"]="255.255.255.0";request("connect");assert(www.code==400);www.args["ip"]="192.168.1.55";www.args["gw"]="192.168.1.1";www.args["sn"]="255.0.255.0";request("connect");assert(www.code==400);
reset();station();request("connect");assert(www.code==202&&WiFi.begins==0);clockMs=500;pollWifiNetwork();assert(WiFi.begins==0);clockMs=1001;pollWifiNetwork();assert(connecting&&WiFi.begins==0&&apActive());advance(350);assert(WiFi.begins==1&&wifiManager.writes==0&&!WiFi.autoReconnect);
// 新参数只用于验证中的连接，成功前不能改变已保存的参数和密码
assert(std::string(connectionStation().ssid)=="New Home"&&std::string(connectionPassword().password)=="new-pass-123");assert(std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password");
assert(WiFi.startedSsid=="New Home"&&WiFi.startedPassword=="new-pass-123");
WiFi.connection=3;pollWifiNetwork();assert(!connecting&&!failed&&!wifiManager.networkBusy&&wifiManager.writes==1);
reset();station();request("connect");startConnection();advance(15000);assert(connecting&&WiFi.begins==1);advance(350);assert(WiFi.begins==2);advance(14650);assert(!connecting&&failed&&apActive()&&WiFi.mode==WIFI_AP&&wifiManager.writes==0);clockMs=60000;pollWifiNetwork();assert(!connecting&&WiFi.begins==2);
// 临时失败状态不终止正确连接；持续失败至多重试一次，之后等待用户修改。
for(int status:{WL_CONNECT_FAILED,WL_NO_SSID_AVAIL}){
 reset();station();request("connect");startConnection();WiFi.connection=status;advance(100);assert(connecting&&!failed);advance(1000);assert(connecting);WiFi.connection=WL_CONNECTED;advance(100);assert(!connecting&&!failed&&WiFi.begins==1&&wifiManager.writes==1);
}
reset();station();request("connect");startConnection();WiFi.connection=WL_NO_SSID_AVAIL;advance(3500);advance(1500);advance(350);assert(WiFi.begins==2&&connecting);WiFi.connection=WL_CONNECTED;advance(100);assert(!failed&&!connecting&&wifiManager.writes==1);
for(int status:{WL_CONNECT_FAILED,WL_NO_SSID_AVAIL}){
 reset();station();request("connect");startConnection();WiFi.connection=status;advance(3500);advance(1500);assert(connecting&&!failed&&WiFi.begins==1);advance(350);assert(WiFi.begins==2);WiFi.connection=status;advance(3500);advance(1500);
 assert(failed&&!connecting&&!wifiManager.networkBusy&&!stationReconnect.known()&&WiFi.mode==WIFI_AP&&!WiFi.autoReconnect&&wifiManager.writes==0);
 assert(std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password");
 clockMs=600000;pollWifiNetwork();assert(WiFi.begins==2&&!connecting);
 request("status");assert(www.code==200&&www.body.find("\"failed\":true")!=std::string::npos);
 // 失败后仍可扫描，并提交正确密码连接成功。
 request("scan");WiFi.scan=2;request("scan");assert(www.code==200&&WiFi.mode==WIFI_AP);
 station();www.args["password"]="correct-pass";request("connect");assert(www.code==202);startConnection();assert(connecting&&WiFi.begins==3);
 WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(!failed&&!connecting&&wifiManager.writes==1);
 // 已成功连接后意外掉线仍保留一次自动恢复机会。
 WiFi.connection=0;advance(1);assert(!connecting);advance(1999);assert(!connecting);advance(1);assert(connecting&&WiFi.begins==3);advance(350);assert(WiFi.begins==4);
 WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(!connecting&&wifiManager.writes==1);
}
reset();station();request("connect");startConnection();WiFi.local[2]=0;WiFi.connection=3;pollWifiNetwork();assert(failed&&WiFi.connection==0&&apActive());
// STA 密码使用实际的 63 字节上限，AP 的旧存储边界仍是 31 字节。
reset();station();www.args["password"]=std::string(63,'x');request("connect");assert(www.code==202);startConnection();assert(std::string(connectionPassword().password)==std::string(63,'x'));WiFi.connection=WL_CONNECTED;pollWifiNetwork();assert(wifiManager.writes==1);request("status");assert(www.body.find(std::string(63,'x'))==std::string::npos&&www.body.find("\"remembered\":true")!=std::string::npos);
reset();station();www.args["password"]=std::string(64,'x');request("connect");assert(www.code==400&&pending==0);apRequest();www.args["password"]=std::string(32,'x');request("ap");assert(www.code==400);
// 已连接网络的原样保存不掉线、不擦除密码、不写 Flash。
// 另一个页面在连接验证中保存管理设置，也不能提前持久化未经验证的新密码。
reset();station();request("connect");startConnection();wifiManager.writeSettings();assert(wifiManager.storedPassword=="password");request("status");assert(www.body.find("New Home")!=std::string::npos&&www.body.find("\"remembered\":false")!=std::string::npos);advance(30000);assert(failed&&wifiManager.storedPassword=="password"&&std::string(wifiManager.sta->ssid)=="Home");
reset();station();request("connect");startConnection();wifiManager.writeSettings();WiFi.connection=WL_CONNECTED;advance(1);assert(wifiManager.storedPassword=="new-pass-123"&&std::string(wifiManager.sta->ssid)=="New Home");
reset();WiFi.mode=3;WiFi.connection=WL_CONNECTED;wifiManager.settings.stationEnabled=true;station();www.args["ssid"]="Home";www.args["password"]="";request("connect");assert(www.code==200&&pending==0&&WiFi.disconnects==0&&wifiManager.writes==0);assert(std::string(wifiManager.staPwd->password)=="password");
// 还未拿到 IP 时不能报告成功，也不会把错误网络地址写入已保存配置。
reset();station();request("connect");startConnection();memset(WiFi.local,0,4);WiFi.connection=WL_CONNECTED;advance(1000);assert(connecting&&wifiManager.writes==0);advance(29000);assert(failed&&!connecting&&wifiManager.writes==0);
// 配置失败同样恢复热点和旧凭据。
reset();station();request("connect");WiFi.configOk=false;startConnection();assert(failed&&!connecting&&apActive()&&wifiManager.writes==0);
// 无线时序在 millis 回绕后仍保持相同的等待与总超时限制。
reset();station();request("connect");WiFi.operationDelayMs=3;advance(1001);assert(connecting&&!failed&&WiFi.begins==0);advance(350);assert(connecting&&WiFi.begins==1);WiFi.connection=WL_CONNECTED;advance(1);assert(!failed&&!connecting&&wifiManager.writes==1);
{swsWifi::StationConnection flow;uint32_t start=0xffffff00;flow.begin(start);assert(flow.poll(uint32_t(start+349),false,false,false)==swsWifi::StationConnection::Wait);assert(flow.poll(uint32_t(start+350),false,false,false)==swsWifi::StationConnection::Start);assert(flow.poll(uint32_t(start+30000),false,false,false)==swsWifi::StationConnection::Failed);}
// 启动时回退到 AP 不应丢失原来已开启的 STA 意图，恢复流程不依赖浏览器。
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();assert(wifiManager.webManaged&&!WiFi.autoReconnect&&wifiManager.settings.stationEnabled);advance(1999);assert(!connecting);advance(1);assert(connecting&&backgroundConnecting&&apActive());advance(350);assert(WiFi.begins==1);WiFi.connection=WL_CONNECTED;advance(1);assert(!connecting&&!backgroundConnecting&&wifiManager.writes==0);
// 断线后的每次后台尝试有截止时间，等待间隔递增至 60 秒；恢复后不重复写 Flash。
WiFi.connection=0;advance(1);advance(2000);assert(backgroundConnecting);
for(unsigned long wait:{5000UL,10000UL,20000UL,40000UL,60000UL,60000UL}){
 advance(30000);assert(failed&&!connecting&&apActive());int begins=WiFi.begins;
 advance(wait-1);assert(!connecting&&WiFi.begins==begins);advance(1);assert(connecting&&backgroundConnecting);advance(350);assert(WiFi.begins==begins+1);
}
WiFi.connection=WL_CONNECTED;advance(1);assert(!failed&&!connecting&&wifiManager.writes==0);
// 已知网络后台连接时用户刷新、换网和关闭开关都优先，无需等 30 秒。
WiFi.connection=0;advance(1);advance(2000);advance(350);assert(backgroundConnecting);
request("scan");assert(www.code==200&&scanning&&!connecting&&!backgroundConnecting);WiFi.scan=0;request("scan");assert(!scanning);advance(5000);assert(backgroundConnecting);advance(350);
station();request("connect");assert(www.code==202&&!connecting&&pending==1);startConnection();assert(connecting&&!backgroundConnecting);WiFi.connection=WL_CONNECTED;advance(1);assert(wifiManager.writes==1);
WiFi.connection=0;advance(1);advance(2000);assert(backgroundConnecting);station();www.args["enabled"]="0";request("connect");assert(www.code==202&&!connecting);advance(1001);assert(!wifiManager.settings.stationEnabled);int disabledBegins=WiFi.begins;advance(600000);assert(!connecting&&WiFi.begins==disabledBegins);
// SDK 明确返回密码错误时停止自动尝试，通用连接失败仍可后台恢复。
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();advance(2000);advance(350);WiFi.wrongPassword=true;WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);assert(failed&&!connecting&&WiFi.begins==1);advance(600000);assert(!connecting&&WiFi.begins==1);assert(notice.find(L_UI_WIFI_AUTH_FAILED)!=std::string::npos);
WiFi.wrongPassword=false;station();www.args["ssid"]="Home";www.args["password"]="corrected-pass";request("connect");startConnection();WiFi.connection=WL_CONNECTED;advance(1);assert(!failed&&wifiManager.writes==1);WiFi.connection=0;advance(1);advance(2000);assert(backgroundConnecting);
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();advance(2000);advance(350);WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);advance(350);WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);assert(failed&&!connecting&&WiFi.begins==2);advance(5000);assert(backgroundConnecting);
// 新网络密码错误不覆盖原网络，仍自动恢复原先可用的网络。
reset();wifiManager.settings.stationEnabled=true;WiFi.connection=WL_CONNECTED;pollWifiNetwork();station();request("connect");startConnection();WiFi.wrongPassword=true;WiFi.connection=WL_CONNECT_FAILED;advance(3000);advance(1500);assert(!connecting&&std::string(wifiManager.sta->ssid)=="Home"&&std::string(wifiManager.staPwd->password)=="password"&&wifiManager.writes==0);WiFi.wrongPassword=false;advance(5000);assert(backgroundConnecting);
// 地址冲突属于需修改设置的问题，不能在后台反复尝试同一配置。
reset();wifiManager.stationEnabledAtBoot=true;pollWifiNetwork();advance(2000);advance(350);WiFi.local[2]=0;WiFi.connection=WL_CONNECTED;advance(1);assert(failed&&!connecting);advance(600000);assert(!connecting&&WiFi.begins==1);
// 修改地址后遇到冲突时，应回到原先有效的地址设置，允许恢复旧连接。
reset();wifiManager.settings.stationEnabled=true;wifiManager.sta->dhcpEnabled=false;WiFi.connection=WL_CONNECTED;pollWifiNetwork();station();www.args["ssid"]="Home";www.args["password"]="";request("connect");startConnection();WiFi.local[2]=0;WiFi.connection=WL_CONNECTED;advance(1);assert(failed&&!wifiManager.sta->dhcpEnabled);advance(5000);assert(backgroundConnecting);
// 退避调度同样处理毫秒计数回绕，手动操作可短暂让出后台尝试。
{swsWifi::StationReconnect flow;uint32_t start=0xffffff00;flow.init(true,start);assert(!flow.due(uint32_t(start+1999)));assert(flow.due(uint32_t(start+2000)));flow.failed(start,false);assert(!flow.due(uint32_t(start+4999)));assert(flow.due(uint32_t(start+5000)));flow.failed(start,true);assert(!flow.due(uint32_t(start+600000)));flow.online(start);flow.defer(start);assert(!flow.due(uint32_t(start+4999)));assert(flow.due(uint32_t(start+5000)));}
reset();apRequest();www.args["enabled"]="0";request("ap");assert(www.code==400);WiFi.connection=3;wifiManager.settings.stationEnabled=true;request("ap");assert(www.code==202);clockMs=1001;pollWifiNetwork();assert(!apActive()&&wifiManager.writes==1&&std::string(noticeTarget)=="ap");request("status");assert(www.body.find("\"messageTarget\":\"ap\"")!=std::string::npos);
reset();wifiManager.settings.accessPointEnabled=false;station();www.args["enabled"]="0";request("connect");assert(www.code==400);
reset();apRequest();WiFi.connection=3;WiFi.local[2]=0;request("ap");assert(www.code==400);
// 提交关闭热点后突然掉线时，取消操作并保留原配置
reset();apRequest();www.args["ssid"]="Rejected AP";request("ap");WiFi.apFailures=1;advance(1001);assert(failed&&wifiManager.writes==0&&std::string(wifiManager.settings.ap.ssid)=="ONSTEPX"&&apActive()&&!pending);
reset();WiFi.connection=WL_CONNECTED;wifiManager.settings.stationEnabled=true;apRequest();www.args["enabled"]="0";request("ap");WiFi.apCloseOk=false;advance(1001);assert(failed&&wifiManager.writes==0&&wifiManager.settings.accessPointEnabled&&apActive());
reset();apRequest();www.args["enabled"]="0";WiFi.connection=3;wifiManager.settings.stationEnabled=true;request("ap");assert(www.code==202);WiFi.connection=0;clockMs=1001;pollWifiNetwork();assert(apActive()&&wifiManager.writes==0&&failed);
std::cout<<"PASS: authentication, password privacy and reuse, 63-byte STA passwords, non-disruptive save, staged connection, transient failure recovery, one retry/30s deadline, no-IP wait, scan/AP preservation, rollback, manual correction and dropped-link recovery.\n";
}
