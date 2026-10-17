#pragma once

#include "../../libApp/status/State.h"
#include "../../locales/Locale.h"

// 多个故障同时出现时，译文拼接也必须限制在固定缓冲区内
inline void appendControllerText(char *destination, size_t capacity, const char *text) {
  const size_t used = strlen(destination);
  if (used < capacity) snprintf(destination + used, capacity - used, "%s", text);
}

inline void formatControllerDriver(const DriverStatusEx &driver, char *destination, size_t capacity) {
  if (!driver.valid) { snprintf(destination, capacity, "?"); return; }
  char text[80] = "";
  if (driver.communicationFailure) appendControllerText(text, sizeof(text), L_COMMS_FAILURE ", ");
  else {
    if (driver.standstill) appendControllerText(text, sizeof(text), L_STANDSTILL ", ");
    if (driver.outputA.openLoad || driver.outputB.openLoad) {
      appendControllerText(text, sizeof(text), L_OPEN_LOAD " ");
      if (driver.outputA.openLoad) appendControllerText(text, sizeof(text), "A");
      if (driver.outputB.openLoad) appendControllerText(text, sizeof(text), "B");
      appendControllerText(text, sizeof(text), ", ");
    }
    if (driver.outputA.shortToGround || driver.outputB.shortToGround) {
      appendControllerText(text, sizeof(text), L_SHORT_GND " ");
      if (driver.outputA.shortToGround) appendControllerText(text, sizeof(text), "A");
      if (driver.outputB.shortToGround) appendControllerText(text, sizeof(text), "B");
      appendControllerText(text, sizeof(text), ", ");
    }
    if (driver.overTemperature) appendControllerText(text, sizeof(text), L_SHUTDOWN_OVER " 150C, ");
    if (driver.overTemperaturePreWarning) appendControllerText(text, sizeof(text), L_PRE_WARNING " &gt;120C, ");
  }
  const size_t length = strlen(text);
  if (length > 2) text[length - 2] = 0;
  if (driver.fault) appendControllerText(text, sizeof(text), " (" L_DRIVER_FAULT ")");
  snprintf(destination, capacity, "%s", text[0] ? text : "Ok");
}

inline void decodeControllerDriver(const char *reply, DriverStatusEx &driver) {
  driver.valid = true;
  driver.communicationFailure = strstr(reply, "ST,OA,OB,GA,GB,OT,PW");
  driver.standstill = strstr(reply, "ST");
  driver.outputA.openLoad = strstr(reply, "OA");
  driver.outputB.openLoad = strstr(reply, "OB");
  driver.outputA.shortToGround = strstr(reply, "GA");
  driver.outputB.shortToGround = strstr(reply, "GB");
  driver.overTemperature = strstr(reply, "OT");
  driver.overTemperaturePreWarning = strstr(reply, "PW");
  driver.fault = strstr(reply, "GF");
}
