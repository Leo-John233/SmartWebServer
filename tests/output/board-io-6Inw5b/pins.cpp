#define OFF -1
#define ON -2
#define WIFI 2
#define OPERATIONAL_MODE WIFI
#define ESP8266
#define FWU -204
#define DISPLAY_RESET_CONTROLS OFF
#define AXIS1_ENCODER OFF
#define AXIS2_ENCODER ON
#include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/pinmaps/Pins.Esp8266.h"
#include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/libApp/boardIo/Config.h"
static_assert(RA_BRAKE_PIN==5 && SWS_RA_BRAKE_PIN==5,"IO5 brake mapping");
static_assert(AXIS2_ENCODER_A_PIN==OFF && VGPIO_PIN_2==OFF,"IO5 not reused by encoder or virtual GPIO");
static_assert(SWS_ACTIVITY_LED_PIN==2,"IO2 activity LED mapping");
