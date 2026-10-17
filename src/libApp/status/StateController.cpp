// update controller state
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"
#include "../../libApp/misc/Misc.h"
#include "../../locales/Locale.h"
#include "../boardIo/BoardIo.h"
#include "../boardIo/OnStepProtection.h"
#if OPERATIONAL_MODE == WIFI
  #include "../../lib/wifi/WifiManager.h"
#endif

// 多个故障同时出现时，译文拼接也必须限制在固定缓冲区内
static void appendControllerText(char *destination, size_t capacity, const char *text) {
  const size_t used = strlen(destination);
  if (used < capacity) snprintf(destination + used, capacity - used, "%s", text);
}

static void formatControllerDriver(const DriverStatusEx &driver, char *destination, size_t capacity) {
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

static void decodeControllerDriver(const char *reply, DriverStatusEx &driver) {
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
  axisStatusUpdate();
  int numAxes = 2;
  if (status.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    formatControllerDriver(driver[axis], driverStatusStr[axis], sizeof(driverStatusStr[axis])); Y;
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

}

// attempt to get the driver status for all 9 axes, mark the ones that return false so we don't attempt to process again
void State::axisStatusUpdate() {
  static int driverStatusFailedAttempts[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  int maxAxes = 2;
  if (status.getVersionMajor() >= 10) maxAxes = 9;
  for (int axis = 0; axis < maxAxes; axis++) {
    if (axis == 0 && boardIo.brakeAvailable()) { updateBrakeDriverStatus(); continue; }
    if (driverStatusFailedAttempts[axis] < 3) {
      char cmd[40];
      char reply[80]; // 命令通道最多写入 79 字节及结束符，异常长反馈也不能越界
      snprintf(cmd, sizeof(cmd), ":GXU%d#", axis + 1);
      const bool received = onStep.command(cmd, reply);
      if (received && reply[0] != '0') {
        driverStatusFailedAttempts[axis] = 0;
        decodeControllerDriver(reply, driver[axis]);
      } else {
        driverStatusFailedAttempts[axis]++;
        driver[axis].valid = false;
      }
      Y;
    }
  }
}

void State::updateBrakeDriverStatus() {
  if (!boardIo.brakeAvailable()) return;
  char reply[80];
  // 页面只读同一次采样，不额外查询，也不因连续读取失败而停止刷新
  const bool received = status.onStepFound && status.mountFound == SD_TRUE &&
                        onStepProtection.latestDriverReply(reply, sizeof(reply));
  if (received) decodeControllerDriver(reply, driver[0]);
  else driver[0].valid = false;
  formatControllerDriver(driver[0], driverStatusStr[0], sizeof(driverStatusStr[0]));
}
