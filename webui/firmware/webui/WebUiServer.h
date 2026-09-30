#pragma once

#include "../Common.h"

#if defined(ESP8266) && OPERATIONAL_MODE == WIFI && WEB_SERVER == ON

#include "WebAssets.generated.h"

void handleWebUi();
void handleWebUiAsset();
void handleUiConfig();

#endif
