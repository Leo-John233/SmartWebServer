#include "Request.h"
#include "../../pages/Pages.common.h"
#include "../boardIo/Activity.h"

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
