#pragma once

#include "../pages/Pages.common.h"
#include "../api/ApiV1.h"
#include "WebUiServer.h"

#if !defined(ESP8266) || OPERATIONAL_MODE != WIFI || WEB_SERVER != ON
  #error "Vue firmware currently requires ESP8266 WiFi with WEB_SERVER enabled"
#else

// 仅由 Vue 构建装配器接入，原版固件不包含此路由模块
inline void registerVueRoutes() {
  www.on("/ui", handleWebUi);
  www.on(WEB_UI_INDEX_PATH, handleWebUi);
  www.on("/ui/index.html", handleWebUi);
  www.on(WEB_UI_STYLE_PATH, handleWebUiAsset);
  www.on(WEB_UI_SCRIPT_PATH, handleWebUiAsset);
  www.on("/ui-config.json", handleUiConfig);
  www.on("/api/v1/capabilities", handleApiV1Capabilities);
  www.on("/api/v1/state", handleApiV1State);
  www.on("/api/v1/actions", HTTP_POST, handleApiV1Actions);
  www.on("/api/v1/network", HTTP_GET, handleApiV1NetworkGet);
  www.on("/api/v1/network", HTTP_POST, handleApiV1NetworkPost);
  www.on("/", handleWebUi);
}

#endif
