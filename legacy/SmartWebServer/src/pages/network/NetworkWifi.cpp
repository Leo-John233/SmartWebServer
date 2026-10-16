#include "NetworkWifi.h"

#if OPERATIONAL_MODE == WIFI
#include "../../lib/wifi/WifiManager.h"

extern bool loginRequired;

namespace {
bool scanning = false, scanAddedSta = false, connecting = false, failed = false;
uint8_t pending = 0; // 1 表示连接路由器，2 表示设置热点，先返回网页响应再切换网络
unsigned long scheduledAt = 0, connectStarted = 0, scanStarted = 0, lastRetry = 0;
// 上游将路由器参数和密码独立保存，提交时一起暂存并在应用时更新
WifiSettings proposed;
StationSettings proposedStation;
StationPassword proposedPassword;
String notice;

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
  WiFi.enableSTA(true);
  WiFi.disconnect();
  IPAddress zero((uint32_t)0);
  bool configured = s.dhcpEnabled ? WiFi.config(zero, zero, zero) :
    WiFi.config(IPAddress(s.ip), IPAddress(s.gw), IPAddress(s.sn), IPAddress(s.gw));
  if (!configured) return false;
  WiFi.begin(s.ssid, wifiManager.staPwd->password);
  connectStarted = millis();
  connecting = true;
  wifiManager.networkBusy = true;
  failed = false;
  notice = "正在连接路由器，最多等待 25 秒；若页面断开，请重新连接设备热点";
  return true;
}
void connectionFailed(const String &reason) {
  connecting = false;
  failed = true;
  wifiManager.networkBusy = false;
  WiFi.disconnect();
  bool available = apActive() || startAp();
  notice = reason + (available ? " 设备热点仍可用于重新配网" : " 热点启动失败，请重启设备\n");
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
  json += ",\"apSsid\":" + quote(a.ssid) + ",\"message\":" + quote(notice) + "}";
  return json;
}
}

void renderWifiNetwork(String &data) {
  // 分段发送页面，避免一次复制整页造成设备内存紧张
  www.sendContentAndClear(data);
  const size_t length = sizeof(html_network_wifi) - 1;
  char chunk[513];
  for (size_t offset = 0; offset < length; offset += 512) {
    size_t count = length - offset < 512 ? length - offset : 512;
    memcpy_P(chunk, html_network_wifi + offset, count); chunk[count] = 0;
    www.sendContent(chunk);
  }
}

void handleWifiNetwork() {
  // 沿用网络页面的登录状态，未登录时不能扫描或修改配置
  if (loginRequired) { error(403, "请先输入管理密码登录"); return; }
  String action = www.arg("action");
  if (action == "status") { reply(200, configJson()); return; }
  if (www.method() != HTTP_POST) { error(405, "请使用 POST 提交"); return; }
  if (pending || connecting) { error(409, "连接正在进行，请稍候"); return; }

  if (action == "scan") {
    if (!scanning) {
      WiFi.persistent(false);
      scanAddedSta = !(WiFi.getMode() & WIFI_STA);
      WiFi.enableSTA(true); // 保留热点和已有路由器连接，只开启扫描需要的无线接口
      wifiManager.networkBusy = true;
      WiFi.scanDelete();
      int result = WiFi.scanNetworks(true, false);
      if (result == -2) { endScan(); error(503, "无法启动扫描，请稍后重试"); return; }
      scanning = true; scanStarted = millis();
    }
    int count = WiFi.scanComplete();
    if (count == -1) { reply(200, "{\"running\":true}"); return; }
    if (count < 0) { endScan(); error(503, "扫描失败，请重试"); return; }
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
  if (scanning) { error(409, "请等待扫描完成"); return; }
  if (action != "connect" && action != "ap") { error(400, "未知操作"); return; }
  proposed = wifiManager.settings;
  proposedStation = *wifiManager.sta;
  proposedPassword = *wifiManager.staPwd;
  bool enabled = www.arg("enabled") == "1";
  String ssid = www.arg("ssid"), password = www.arg("password");
  if (ssid.length() == 0 || ssid.length() > 31 || password.length() > 31) {
    error(400, "Wi-Fi 名称不能为空；名称和密码均不能超过 31 字节（中文会占多个字节）"); return;
  }
  if (action == "connect") {
    StationSettings &s = proposedStation;
    char *savedPassword = proposedPassword.password;
    bool open = www.arg("open") == "1";
    if (enabled && !open && ((password.length() && password.length() < 8) ||
        (!password.length() && (ssid != s.ssid || strlen(savedPassword) < 8)))) {
      error(400, "请输入 Wi-Fi 密码，无密码的网络请勾选无需密码"); return;
    }
    proposed.stationEnabled = enabled;
    s.dhcpEnabled = www.arg("dhcp") == "1";
    if (!s.dhcpEnabled && !addresses(s.ip, s.gw, s.sn)) { error(400, "请检查静态 IP、网关和子网掩码"); return; }
    if (enabled && !s.dhcpEnabled && overlap(s.ip, s.sn, proposed.ap.ip, proposed.ap.sn)) {
      error(400, "STA 与设备热点网段重叠，请更改其中一个网段"); return;
    }
    if (!enabled && !proposed.accessPointEnabled) { error(400, "请先启用设备热点，再关闭 STA"); return; }
    // 关闭状态下更换网络名称时清除旧密码，防止下次误用其他网络的密码
    bool changedNetwork = ssid != s.ssid;
    strcpy(s.ssid, ssid.c_str());
    if (open) savedPassword[0] = 0;
    else if (password.length()) strcpy(savedPassword, password.c_str());
    else if (changedNetwork) savedPassword[0] = 0;
    pending = 1;
  } else {
    AccessPointSettings &a = proposed.ap;
    if (password.length() && password.length() < 8) { error(400, "热点密码需为 8–31 字节"); return; }
    if (!enabled && (WiFi.status() != WL_CONNECTED || !proposed.stationEnabled)) {
      error(400, "请先成功连接路由器，再关闭设备热点"); return;
    }
    if (!addresses(a.ip, a.gw, a.sn)) { error(400, "请检查热点 IP、网关和子网掩码"); return; }
    if (enabled && WiFi.status() == WL_CONNECTED) {
      IPAddress ip = WiFi.localIP(), mask = WiFi.subnetMask();
      if (overlap(a.ip, a.sn, &ip[0], &mask[0])) { error(400, "热点与路由器网段重叠，请更改热点网段"); return; }
    }
    String channel = www.arg("channel");
    int ch = channel.toInt();
    if (ch < 1 || ch > 11 || String(ch) != channel) { error(400, "热点信道应为 1–11"); return; }
    strcpy(a.ssid, ssid.c_str());
    if (password.length()) strcpy(a.pwd, password.c_str());
    a.channel = ch;
    proposed.accessPointEnabled = enabled;
    pending = 2;
  }
  scheduledAt = millis(); failed = false;
  notice = "设置已收到，正在应用，请稍候";
  reply(202, "{\"message\":" + quote(notice) + "}");
}

void pollWifiNetwork() {
  unsigned long now = millis();
  // 浏览器关闭后也会释放扫描缓存，并恢复原来的无线模式
  if (scanning && now - scanStarted > 45000) endScan();
  if (pending && now - scheduledAt >= 1000) {
    uint8_t operation = pending; pending = 0;
    // 延迟应用期间若路由器掉线，取消关闭热点，保留可用入口
    if (operation == 2 && !proposed.accessPointEnabled && WiFi.status() != WL_CONNECTED) {
      failed = true;
      notice = "路由器已断开，设备热点保持开启";
      return;
    }
    wifiManager.settings = proposed;
    wifiManager.setStation(wifiManager.stationNumber);
    if (operation == 1) {
      *wifiManager.sta = proposedStation;
      *wifiManager.staPwd = proposedPassword;
    }
    WiFi.persistent(false);
    if (operation == 1) {
      wifiManager.webManaged = true;
      // 配网期间保留热点，连接失败后仍能通过热点修改设置
      if (wifiManager.settings.stationEnabled) {
        if (!apActive()) startAp();
        if (!configureStation()) connectionFailed("无法应用 IP 设置");
      } else {
        WiFi.disconnect(); WiFi.enableSTA(false);
        if (!apActive()) startAp();
        notice = "已断开路由器，设备热点仍可使用";
      }
    } else {
      if (wifiManager.settings.accessPointEnabled) {
        if (!startAp()) { failed = true; notice = "热点设置应用失败，请检查配置并重试"; }
        else notice = "热点设置已保存，连接路由器后会自动调整信道";
      } else { WiFi.softAPdisconnect(true); notice = "热点已关闭，请通过路由器分配的 IP 访问设备"; }
    }
    wifiManager.writeSettings();
  }
  if (connecting) {
    if (WiFi.status() == WL_CONNECTED) {
      IPAddress ip = WiFi.localIP(), mask = WiFi.subnetMask();
      AccessPointSettings &a = wifiManager.settings.ap;
      if (apActive() && overlap(a.ip, a.sn, &ip[0], &mask[0])) {
        connectionFailed("路由器分配的地址与设备热点网段冲突，请先修改热点网段");
      } else {
        connecting = false; wifiManager.networkBusy = false; failed = false;
        notice = "连接成功，局域网访问地址：http://" + WiFi.localIP().toString();
        lastRetry = now;
      }
    } else if (now - connectStarted > 25000) {
      connectionFailed("没有连上 Wi-Fi，请检查密码或靠近路由器后重试");
    }
  }
  #if STA_AUTO_RECONNECT == true
    // 网页配网后使用较长重试间隔，给扫描和修改密码留出时间
    if (wifiManager.webManaged && wifiManager.settings.stationEnabled && !connecting && !scanning && !pending &&
        WiFi.status() != WL_CONNECTED && now - lastRetry > 30000) {
      lastRetry = now;
      if (!configureStation()) connectionFailed("无法应用 IP 设置");
    }
  #endif
}
#endif
