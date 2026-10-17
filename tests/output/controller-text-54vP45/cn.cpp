
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdint>
#define DISPLAY_WEATHER 0
#define DISPLAY_INTERNAL_TEMPERATURE 0
#define ON 1
#define WIFI 1
#define OPERATIONAL_MODE 0
#define ERR_NONE 0
#define Y
unsigned long millis(){return 1;}
struct Status {
  bool onStepFound=true; int lastError=1;
  int getVersionMajor(){return 10;}
  void getLastErrorMessage(char *out,size_t size){memset(out,'x',size-1);out[size-1]=0;}
}status;
struct Device {bool command(const char*,char*){return false;}}onStep;
struct Channel {bool openLoad=false,shortToGround=false;};
struct Driver {bool valid=true,communicationFailure=false,standstill=false,overTemperature=false,overTemperaturePreWarning=false,fault=false;Channel outputA,outputB;};
struct State {
  char before[16]; Driver driver[9];
  char siteTemperatureStr[16],sitePressureStr[16],siteHumidityStr[16],siteDewPointStr[16];
  char controllerTemperatureStr[16],lastErrorStr[80],workLoadStr[20],signalStrengthStr[20],driverStatusStr[9][40];
  char after[16]; unsigned long lastControllerPageLoadTime=0;
  void updateController(bool); void axisStatusUpdate(){}
};
#define DISPLAY_LANGUAGE L_cn
// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define PROD_ABV                     "SWS"

#define METRIC                       ON
#define IMPERIAL                     -200
#define LOCALE_DEFAULT               -201
#define AUTO_ON                      -202
#define AUTO_OFF                     -203
#define FWU                          -204

// ajax page update feature
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 500     // fast update rate
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 5000    // time before return to normal update rate
#define AJAX_PAGE_LAZY_GET_MS         1000    // wait time for lazy get

// various auxillary features
#define SWITCH                       1
#define ANALOG_OUTPUT                2
#define DEW_HEATER                   3
#define INTERVALOMETER               4
#define COVER_SWITCH                 7

// The settings below are for initialization only, afterward they are stored and recalled from EEPROM and must
// be changed in the web interface OR with a reset (for initialization again) as described in the Config.h comments
#define TIMEOUT_WEB                  200
#define TIMEOUT_COMMAND              200

// task manager
#define TASKS_MAX                    40      // up to 40 tasks
#define TASKS_SKIP_MISSED

// NV -------------------------------------------------------------------------------------------------------------------

// unique volume signature for this volume/partition layout
#define NV_VOLUME_SIGNATURE         0x0001u

// -----------------------------------------------------------------------------------
// Locales
#pragma once

// see Strings_xx.h for individual locale translations

// languages (one unique numeric code required for each)
#define L_ca 1 // Catalan
#define L_cn 2 // Chinese
#define L_de 3 // German
#define L_en 4 // English
#define L_es 5 // Spanish
#define L_fr 6 // French
#define L_it 7 // Italian
#define L_jp 8 // Japanese
#define L_us 9 // English (US, imperial units)
#define L_ro 10 // Romanian

// -----------------------------------------------------------------------------------
// Locale
#pragma once




// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_ca
#include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ca.h"
#define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_cn
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_cn.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_de
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_es
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_es.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_fr
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_fr.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_it
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_it.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_jp
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_jp.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS IMPERIAL
#endif
#if DISPLAY_LANGUAGE == L_ro
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ro.h"
  #define UNITS METRIC
#endif

// 未提供专用停止文字时沿用当前语言的停止描述
#ifndef BUTTON_STOP
  #define BUTTON_STOP L_STOP
#endif

// 未提供本地翻译时保留通用同步文字
#ifndef L_COORD_SYNC
  #define L_COORD_SYNC "Sync"
#endif

// misc. locale support functions
#ifndef DISPLAY_UNITS
  #define DISPLAY_UNITS UNITS
#else
  #if DISPLAY_UNITS == LOCALE_DEFAULT
    #undef DISPLAY_UNITS
    #define DISPLAY_UNITS UNITS
  #endif
#endif

#if DISPLAY_UNITS == IMPERIAL
  #define nativeToCelsius(t) ((t - 32.0F)*(5.0F/9.0F))
  #define celsiusToNative(t) (t*(9.0F/5.0F) + 32.0F)
  #define nativeToCelsiusRelative(t) (t*(5.0F/9.0F))
  #define celsiusToNativeRelative(t) (t*(9.0F/5.0F))
  #define TEMPERATURE_UNITS_ABV "F"
#else
  #define nativeToCelsius(t) (t)
  #define celsiusToNative(t) (t)
  #define nativeToCelsiusRelative(t) (t)
  #define celsiusToNativeRelative(t) (t)
  #define TEMPERATURE_UNITS_ABV "C"
#endif

// 其他语言未提供新标签时使用默认文字
#ifndef L_HOME_ACTIONS
#define L_HOME_ACTIONS "Home"
#endif
#ifndef L_PARK_ACTIONS
#define L_PARK_ACTIONS "Parking"
#endif
#ifndef L_COORD_GOTO
#define L_COORD_GOTO "Coordinate GOTO"
#endif
#ifndef L_COORD_SYSTEM
#define L_COORD_SYSTEM "Coordinate system"
#endif
#ifndef L_COORD_JNOW
#define L_COORD_JNOW "RA / Dec (JNOW)"
#endif
#ifndef L_COORD_RA
#define L_COORD_RA "RA"
#endif
#ifndef L_COORD_DEC
#define L_COORD_DEC "DEC"
#endif
#ifndef L_COORD_SIGN
#define L_COORD_SIGN "Sign"
#endif
#ifndef L_COORD_H
#define L_COORD_H "h"
#endif
#ifndef L_COORD_D
#define L_COORD_D "deg"
#endif
#ifndef L_COORD_M
#define L_COORD_M "min"
#endif
#ifndef L_COORD_S
#define L_COORD_S "sec"
#endif
#ifndef L_COORD_SEND
#define L_COORD_SEND "Sending coordinates"
#endif
#ifndef L_COORD_OK
#define L_COORD_OK "Command accepted"
#endif
#ifndef L_COORD_REJECT
#define L_COORD_REJECT "Command rejected, check device status"
#endif
#ifndef L_COORD_INVALID
#define L_COORD_INVALID "Enter valid coordinates; minutes and seconds must be zero at 90 degrees"
#endif
#ifndef L_COORD_NETWORK
#define L_COORD_NETWORK "Connection lost, check device before retrying"
#endif
#ifndef L_CAT_PREV
#define L_CAT_PREV "Previous"
#endif
#ifndef L_CAT_NEXT
#define L_CAT_NEXT "Next"
#endif
#ifndef L_CAT_TARGET
#define L_CAT_TARGET "Set target"
#endif
#ifndef L_CAT_HELP
#define L_CAT_HELP "Catalog help"
#endif

#ifndef L_COORD_AZ
#define L_COORD_AZ "Azimuth"
#endif
#ifndef L_COORD_ALT
#define L_COORD_ALT "Altitude"
#endif
#ifndef L_GOTO_ACTION
#define L_GOTO_ACTION "GOTO"
#endif

#ifndef L_GOTO_TARGET
#define L_GOTO_TARGET "Go to current target"
#endif
#ifndef L_SYNC_TARGET
#define L_SYNC_TARGET "Sync current target"
#endif

#ifndef L_TRK_AXES
#define L_TRK_AXES "Compensation axes"
#endif

#ifndef L_DEVICE_TRACKING
#define L_DEVICE_TRACKING "Device tracking"
#endif

#ifndef L_SIDEBAR_NOTE
#define L_SIDEBAR_NOTE "OnStep smart control"
#endif


static void appendControllerText(char *destination, size_t capacity, const char *text) {
  const size_t used = strlen(destination);
  if (used < capacity) snprintf(destination + used, capacity - used, "%s", text);
}

void State::updateController(bool now)
{
  if (!now && millis() - lastControllerPageLoadTime > 2000) return;

  char temp[80], temp1[80];

  // Ambient conditions
  #if DISPLAY_WEATHER == ON
    if (!onStep.command(":GX9A#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(siteTemperatureStr, temp, 16); siteTemperatureStr[15] = 0; Y;
    if (!onStep.command(":GX9B#", temp)) strcpy(temp, "?"); else localePressure(temp);
    strncpy(sitePressureStr, temp, 16); sitePressureStr[15] = 0; Y;
    if (!onStep.command(":GX9C#", temp)) strcpy(temp, "?"); else localeHumidity(temp);
    strncpy(siteHumidityStr, temp, 16); siteHumidityStr[15] = 0; Y;
    if (!onStep.command(":GX9E#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(siteDewPointStr, temp, 16); siteDewPointStr[15] = 0; Y;
  #endif

  // Driver status
  int numAxes = 2;
  if (status.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    if (driver[axis].valid) {
      strcpy(temp1, "");
      if (driver[axis].communicationFailure) appendControllerText(temp1, sizeof(temp1), L_COMMS_FAILURE ", ");
      if (!driver[axis].communicationFailure) {
        if (driver[axis].standstill) appendControllerText(temp1, sizeof(temp1), L_STANDSTILL ", "); else {
          if (driver[axis].outputA.openLoad || driver[axis].outputB.openLoad) {
            appendControllerText(temp1, sizeof(temp1), L_OPEN_LOAD " ");
            if (driver[axis].outputA.openLoad) appendControllerText(temp1, sizeof(temp1),"A");
            if (driver[axis].outputB.openLoad) appendControllerText(temp1, sizeof(temp1),"B");
            appendControllerText(temp1, sizeof(temp1),", ");
          }
        }
        if (driver[axis].outputA.shortToGround || driver[axis].outputB.shortToGround) {
          appendControllerText(temp1, sizeof(temp1), L_SHORT_GND " ");
          if (driver[axis].outputA.shortToGround) appendControllerText(temp1, sizeof(temp1),"A");
          if (driver[axis].outputB.shortToGround) appendControllerText(temp1, sizeof(temp1),"B");
          appendControllerText(temp1, sizeof(temp1),", ");
        }
        if (driver[axis].overTemperature) appendControllerText(temp1, sizeof(temp1), L_SHUTDOWN_OVER " 150C, ");
        if (driver[axis].overTemperaturePreWarning) appendControllerText(temp1, sizeof(temp1), L_PRE_WARNING " &gt;120C, ");
      }
      if (strlen(temp1) > 2) temp1[strlen(temp1) - 2] = 0;

      if (driver[axis].fault) appendControllerText(temp1, sizeof(temp1), " (" L_DRIVER_FAULT ")");
      if (strlen(temp1) == 0) strcpy(temp1, "Ok");

      char *thisDriverStatusStr = driverStatusStr[axis];
      strncpy(thisDriverStatusStr, temp1, 40); thisDriverStatusStr[39] = 0; Y;
    }
  }

  // MCU Temperature
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    if (!onStep.command(":GX9F#", temp)) strcpy(temp, "?"); else localeTemperature(temp);
    strncpy(controllerTemperatureStr, temp, 16); controllerTemperatureStr[15] = 0; Y;
  #endif

  // General Error
  strcpy(temp, "");
  if (status.lastError != ERR_NONE) appendControllerText(temp, sizeof(temp), "<font class=\"y\">"); 
  status.getLastErrorMessage(temp1, sizeof(temp1));
  if (!status.onStepFound) appendControllerText(temp, sizeof(temp), "?"); else appendControllerText(temp, sizeof(temp), temp1);
  if (status.lastError != ERR_NONE) appendControllerText(temp, sizeof(temp), "</font>"); 
  strncpy(lastErrorStr, temp, 80); lastErrorStr[79] = 0; Y;

  // Loop time
  if (status.getVersionMajor() < 10) {
    if (!onStep.command(":GXFA#", temp)) strcpy(temp, "?%");
    strncpy(workLoadStr, temp, 20); workLoadStr[19] = 0; Y;
  }

  // wifi signal strength
  #if OPERATIONAL_MODE == WIFI
    long signal_strength_dbm = WiFi.RSSI();
    long signal_strength_qty = 2*(signal_strength_dbm + 100);
    if (signal_strength_qty > 100) signal_strength_qty = 100; 
    else if (signal_strength_qty < 0) signal_strength_qty = 0;
    snprintf(temp, sizeof(temp), "%lddBm (%ld%%)", signal_strength_dbm, signal_strength_qty);
    strncpy(signalStrengthStr, temp, 20); signalStrengthStr[19] = 0; Y;
  #endif

  // update the axis status
  axisStatusUpdate();
}

// attempt to get the driver status for all 9 axes, mark the ones that return false so we don't attempt to process again

int main(){
  State state{};memset(state.before,0x5a,16);memset(state.after,0x6b,16);
  for(unsigned flags=0;flags<512;flags++){
    for(auto &driver:state.driver){
      driver.communicationFailure=flags&1;driver.standstill=flags&2;
      driver.outputA.openLoad=flags&4;driver.outputB.openLoad=flags&8;
      driver.outputA.shortToGround=flags&16;driver.outputB.shortToGround=flags&32;
      driver.overTemperature=flags&64;driver.overTemperaturePreWarning=flags&128;driver.fault=flags&256;
    }
    state.updateController(true);
    assert(state.lastErrorStr[79]==0);
    for(auto &text:state.driverStatusStr)assert(text[39]==0);
    for(int i=0;i<16;i++){assert(state.before[i]==0x5a);assert(state.after[i]==0x6b);}
  }
  struct {char text[80];char guard[16];}bounded{};memset(bounded.guard,0x7c,16);
  for(int n=0;n<20;n++)appendControllerText(bounded.text,sizeof(bounded.text),L_SHUTDOWN_OVER " 150C, " L_PRE_WARNING " >120C, ");
  assert(bounded.text[79]==0);for(char c:bounded.guard)assert(c==0x7c);
}