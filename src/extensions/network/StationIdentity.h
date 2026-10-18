#pragma once

namespace swsWifi {
  // 在 STA 开启后、申请 DHCP 地址前设置名称，避免 SDK 使用默认 ESP 名称
  inline void applyStationHostname() {
    char name[] = HOST_NAME;
    strtohostname(name);
    WiFi.hostname(name);
  }
}
