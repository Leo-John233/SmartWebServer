// -----------------------------------------------------------------------------------
// Web server
#pragma once

#include "../../../Common.h"
#include "../WifiManager.h"

#if OPERATIONAL_MODE == WIFI && WEB_SERVER == ON

  #if defined(ESP32)
    #include <WebServer.h>
  #elif defined(ESP8266)
    #include <ESP8266WebServer.h>
    // 浏览器离开或连接失效后，首次短写即关闭连接，后续分段不再反复等待发送超时。
    class SwsWebServer : public ESP8266WebServer {
      public:
        explicit SwsWebServer(int port) : ESP8266WebServer(port) {}
      protected:
        size_t _currentClientWrite(const char* data, size_t length) override {
          _currentClient.setTimeout(1500);
          const size_t sent = _currentClient.write(data, length);
          if (sent != length) _currentClient.stop();
          return sent;
        }
        size_t _currentClientWrite_P(PGM_P data, size_t length) override {
          _currentClient.setTimeout(1500);
          const size_t sent = _currentClient.write_P(data, length);
          if (sent != length) _currentClient.stop();
          return sent;
        }
    };
  #else
    #error "Configuration (Config.h): No Wifi support is present for this device"
  #endif

  #if defined(ESP32)
    extern WebServer www;
    #define sendContentAndClear(x) sendContent(x); x = "";
  #elif defined(ESP8266)
    extern SwsWebServer www;
    #define sendContentAndClear(x) sendContent(x); x = ""; delay(1);
  #endif

#endif
