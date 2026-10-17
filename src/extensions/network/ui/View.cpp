#include "../NetworkWifi.h"
#include "../../ui/Presentation.h"

#if OPERATIONAL_MODE == WIFI
#include "../../../pages/htmlHeaders.h"
#include "View.h"
#include "Script.h"
// 界面资源与配网处理分开；按小块发送，避免整页复制到 RAM
void renderWifiNetwork(String &data) {
  www.sendContentAndClear(data);
  sendPageResource(html_network_wifi, sizeof(html_network_wifi) - 1);
  sendPageResource(html_network_wifi_script, sizeof(html_network_wifi_script) - 1);
}
#endif
