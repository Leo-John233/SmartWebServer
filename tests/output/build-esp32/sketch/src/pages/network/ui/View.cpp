#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\network\\ui\\View.cpp"
#include "../NetworkWifi.h"
#include "../../ui/Presentation.h"

#if OPERATIONAL_MODE == WIFI
#include "../../htmlHeaders.h"
#include "View.h"
#include "Script.h"
// 界面资源与配网处理分开；按小块发送，避免整页复制到 RAM。
void renderWifiNetwork(String &data) {
  www.sendContentAndClear(data);
  sendPageResource(html_network_wifi);
  sendPageResource(html_network_wifi_script);
}
#endif
