#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\network\\NetworkWifi.h"
#pragma once

#include "../Pages.common.h"

#if OPERATIONAL_MODE == WIFI
void renderWifiNetwork(String &data);
void handleWifiNetwork();
void pollWifiNetwork();
#endif
