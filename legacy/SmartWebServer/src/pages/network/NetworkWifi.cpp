#include "NetworkWifi.h"

#if OPERATIONAL_MODE == WIFI
#include "../../lib/wifi/WifiManager.h"

extern bool loginRequired;

namespace {
bool scanning = false, scanAddedSta = false, connecting = false, failed = false;
bool stationRetryBlocked = false;
uint8_t pending = 0; // 1 表示连接路由器，2 表示设置热点，先返回网页响应再切换网络
unsigned long scheduledAt = 0, connectStarted = 0, scanStarted = 0, lastRetry = 0;
// 上游将路由器参数和密码独立保存，提交时一起暂存并在应用时更新
WifiSettings proposed;
StationSettings proposedStation;
StationPassword proposedPassword;
StationSettings previousStation;
StationPassword previousPassword;
bool stationCandidate = false;
String notice;
const char *noticeTarget = "sta";

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
  StationSettings &s = *wifiManager.sta;
  // 网页连接由本状态机管理，避免 SDK 重连与网页重试同时争用无线接口。
  WiFi.setAutoReconnect(false);
  WiFi.enableSTA(true);
  WiFi.disconnect();
  IPAddress zero((uint32_t)0);
  bool configured = s.dhcpEnabled ? WiFi.config(zero, zero, zero) :
    WiFi.config(IPAddress(s.ip), IPAddress(s.gw), IPAddress(s.sn), IPAddress(s.gw));
  if (!configured) return false;
  WiFi.begin(s.ssid, wifiManager.staPwd->password);
  connectStarted = millis();
  connecting = true;
  stationRetryBlocked = false;
  wifiManager.networkBusy = true;
  failed = false;
  notice = L_UI_CONNECTING_NOTE;
  noticeTarget = "sta";
  return true;
}
void connectionFailed(const String &reason) {
  connecting = false;
  failed = true;
  stationRetryBlocked = true;
  wifiManager.networkBusy = false;
  WiFi.setAutoReconnect(false);
  WiFi.disconnect();
  // 结束失败的 STA 尝试，保留 AP，停止错误凭据引起的后台扫描和重连。
  WiFi.enableSTA(false);
  // 回滚未验证的凭据，后续保存热点等设置时也不能把错误密码一并写入。
  if (stationCandidate) {
    *wifiManager.sta = previousStation;
    *wifiManager.staPwd = previousPassword;
    stationCandidate = false;
  }
  bool available = apActive() || startAp();
  notice = reason + (available ? L_UI_AP_AVAILABLE : L_UI_AP_FAILED);
  noticeTarget = "sta";
  lastRetry = millis();
}
// 页面只读取配置和运行状态，不返回任何保存的密码
String configJson() {
  StationSettings &s = *wifiManager.sta;
  AccessPointSettings &a = wifiManager.settings.ap;
  String json = "{\"sta\":{\"enabled\":";
  json += wifiManager.settings.stationEnabled ? "true" : "false";
  json += ",\"ssid\":" + quote(s.ssid) + ",\"dhcp\":" + (s.dhcpEnabled ? "true" : "false");
  json += ",\"open\":"; json += wifiManager.staPwd->password[0] == 0 ? "true" : "false";
  json += ",\"ip\":" + quote(ipText(s.ip)) + ",\"gw\":" + quote(ipText(s.gw)) + ",\"sn\":" + quote(ipText(s.sn)) + "},\"ap\":{\"enabled\":";
  json += wifiManager.settings.accessPointEnabled ? "true" : "false";
  json += ",\"ssid\":" + quote(a.ssid) + ",\"channel\":" + String(a.channel);
  json += ",\"ip\":" + quote(ipText(a.ip)) + ",\"gw\":" + quote(ipText(a.gw)) + ",\"sn\":" + quote(ipText(a.sn)) + "}";
  json += ",\"connected\":"; json += WiFi.status() == WL_CONNECTED ? "true" : "false";
  json += ",\"connecting\":"; json += connecting ? "true" : "false";
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
  if (ssid.length() == 0 || ssid.length() > 31 || password.length() > 31) {
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
    wifiManager.settings = proposed;
    wifiManager.setStation(wifiManager.stationNumber);
    if (operation == 1) {
      previousStation = *wifiManager.sta;
      previousPassword = *wifiManager.staPwd;
      stationCandidate = proposed.stationEnabled;
      *wifiManager.sta = proposedStation;
      *wifiManager.staPwd = proposedPassword;
    }
    WiFi.persistent(false);
    if (operation == 1) {
      wifiManager.webManaged = true;
      // 配网期间保留热点，连接失败后仍能通过热点修改设置
      if (wifiManager.settings.stationEnabled) {
        if (!apActive()) startAp();
        if (!configureStation()) connectionFailed(L_UI_APPLY_IP_FAILED);
      } else {
        WiFi.disconnect(); WiFi.enableSTA(false);
        if (!apActive()) startAp();
        notice = L_UI_STA_DISCONNECTED;
        wifiManager.writeSettings();
      }
    } else {
      if (wifiManager.settings.accessPointEnabled) {
        if (!startAp()) { failed = true; notice = L_UI_AP_APPLY_FAILED; }
        else notice = L_UI_AP_SAVED;
      } else { WiFi.softAPdisconnect(true); notice = L_UI_AP_CLOSED; }
    }
    // 连接参数验证成功后再持久化，失败密码不能覆盖原有可用配置。
    if (operation == 2) wifiManager.writeSettings();
  }
  if (connecting) {
    const int connection = WiFi.status();
    if (connection == WL_CONNECTED) {
      IPAddress ip = WiFi.localIP(), mask = WiFi.subnetMask();
      AccessPointSettings &a = wifiManager.settings.ap;
      if (apActive() && overlap(a.ip, a.sn, &ip[0], &mask[0])) {
        connectionFailed(L_UI_DHCP_OVERLAP);
      } else {
        connecting = false; wifiManager.networkBusy = false; failed = false;
        notice = L_UI_CONNECTED_ADDRESS + WiFi.localIP().toString();
        lastRetry = now;
        if (stationCandidate) {
          stationCandidate = false;
          wifiManager.writeSettings();
        }
      }
    } else if (connection == WL_CONNECT_FAILED || connection == WL_NO_SSID_AVAIL) {
      connectionFailed(L_UI_CONNECT_FAILED);
    } else if (now - connectStarted > 25000) {
      connectionFailed(L_UI_CONNECT_FAILED);
    }
  }
  #if STA_AUTO_RECONNECT == true
    // 成功连接后意外掉线可重试；明确失败后等待用户修改，避免反复锁住配网入口。
    if (wifiManager.webManaged && !stationRetryBlocked && wifiManager.settings.stationEnabled && !connecting && !scanning && !pending &&
        WiFi.status() != WL_CONNECTED && now - lastRetry > 30000) {
      lastRetry = now;
      if (!configureStation()) connectionFailed(L_UI_APPLY_IP_FAILED);
    }
  #endif
}
#endif
