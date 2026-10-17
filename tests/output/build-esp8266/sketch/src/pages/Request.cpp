#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\Request.cpp"
#include "Request.h"
#include "Pages.common.h"
#include "../libApp/boardIo/Activity.h"

void registerPageHandler(const char *path, void (*handler)()) {
  #if OPERATIONAL_MODE == WIFI
    www.on(path, [handler]() {
      swsNetworkActivity();
      handler();
      swsNetworkActivity();
    });
  #else
    www.on(path, handler);
  #endif
}
