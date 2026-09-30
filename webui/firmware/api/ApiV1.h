#pragma once

#include "../Common.h"

#if defined(ESP8266) && OPERATIONAL_MODE == WIFI && WEB_SERVER == ON

void handleApiV1Capabilities();
void handleApiV1State();
void handleApiV1Actions();
void handleApiV1NetworkGet();
void handleApiV1NetworkPost();

#endif
