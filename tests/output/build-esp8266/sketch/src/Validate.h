#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\Validate.h"
#pragma once

#if NETWORK_LOAD_GUARD != OFF && NETWORK_LOAD_GUARD != ON
  #error "Configuration (Config.h): NETWORK_LOAD_GUARD must be OFF or ON"
#endif

#if DISPLAY_RESET_CONTROLS == FWU
  #if SERIAL_SWAP != OFF
    #error "Configuration (Config.h): Setting DISPLAY_RESET_CONTROLS FWU must be used with SERIAL_SWAP OFF only"
  #endif
#endif

#if ESP32
  #if (AP_ENABLED == true && STA_ENABLED == true)
    #error "AP and Station modes cannot be enabled at the same time with ESP32."
  #endif
#endif
