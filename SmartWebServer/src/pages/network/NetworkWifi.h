#pragma once

#include "../Pages.common.h"

#if OPERATIONAL_MODE == WIFI
void renderWifiNetwork(String &data);
void handleWifiNetwork();
void pollWifiNetwork();
#endif
