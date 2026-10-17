#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\lib\\wifi\\webServer\\WebServer.cpp"
// -----------------------------------------------------------------------------------
// Web server

#include "WebServer.h"

#if OPERATIONAL_MODE == WIFI && WEB_SERVER == ON

  #if defined(ESP32)
    WebServer www(80);
  #elif defined(ESP8266)
    ESP8266WebServer www(80);
  #endif

#endif
