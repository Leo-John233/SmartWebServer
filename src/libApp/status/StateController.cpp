// update controller state
#include "State.h"

#include "Status.h"
#include "../../lib/tasks/OnTask.h"
#include "../../libApp/cmd/Cmd.h"
#include "../../libApp/misc/Misc.h"
#include "../../locales/Locale.h"
#include "../../extensions/boardIo/BoardIo.h"
#include "../../extensions/status/DriverStatus.h"
#if OPERATIONAL_MODE == WIFI
  #include "../../lib/wifi/WifiManager.h"
#endif

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
