// -----------------------------------------------------------------------------------
// Controller Status tile
#include "StatusTile.h"
#include "AmbientTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../lib/tasks/OnTask.h"
#include "../boardIo/BoardIo.h"
#include "../../libApp/boardIo/BoardIo.h"

// create the related webpage tile
void statusTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_text_beg, "22em", "11em", L_STATE);
  data.concat(temp);
  // 状态和环境数据共享紧凑指标区，设置仍保留在独立展开区
  data.concat(F("<br /><hr><div class='sws-metrics'>"));

  // General status

  // get hardware name
  if (status.getVersionMajor() > 10 || (status.getVersionMajor() == 10 && status.getVersionMinor() >= 26)) {
    if (onStep.command(":GVH#", temp)) {
      data.concat(F("<div class='sws-metric'><span>" L_HARDWARE "</span><strong class='c'>"));
      data.concat(temp);
      data.concat(F("</strong></div>"));
    }
  }

  snprintf_P(temp, sizeof(temp), html_indexGeneralError, state.lastErrorStr);
  data.concat(temp);

  if (status.getVersionMajor() < 10)
  {
    snprintf_P(temp, sizeof(temp), html_indexWorkload, state.workLoadStr);
    data.concat(temp);
  }

  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    snprintf_P(temp, sizeof(temp), html_indexTemp, L_INTERNAL_TEMP ":", 'm', state.controllerTemperatureStr);
    data.concat(temp);
  #endif

  #if OPERATIONAL_MODE == WIFI
    snprintf_P(temp, sizeof(temp), html_indexSignalStrength, state.signalStrengthStr);
    data.concat(temp);
  #endif

  #if DISPLAY_WEATHER == ON
    ambientTile(data);
  #endif
  data.concat(F("</div><hr>"));
  www.sendContentAndClear(data);

  int mountType = 0;
  #if DRIVE_CONFIGURATION == ON
    if (status.getVersionMajor() >= 5)
    {
      if (!onStep.command(":GXEM#", temp)) strcpy(temp, "0");
      mountType = atoi(temp);
    }
  #endif
  bool displayCollapsable = DISPLAY_RESET_CONTROLS != OFF || mountType != 0 || boardIo.activityAvailable() || boardIo.brakeAvailable();

  if (displayCollapsable)
  {
    snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
    data.concat(temp);
    www.sendContentAndClear(data);
  }

  const int settingColumns = int(boardIo.activityAvailable()) + int(boardIo.brakeAvailable()) + int(mountType >= 1 && mountType <= 3);
  if (settingColumns) {
    snprintf_P(temp, sizeof(temp), html_indexSettingsBegin, settingColumns);
    data.concat(temp);
  }
  boardIoTile(data);

  #if DRIVE_CONFIGURATION == ON
    if (mountType >= 1 && mountType <= 3)
    {
      char mountForm[1024];
      snprintf_P(mountForm, sizeof(mountForm), html_indexMountType,
        mountType == 1 ? " selected" : "", mountType == 2 ? " selected" : "", mountType == 3 ? " selected" : "");
      data.concat(mountForm);
      www.sendContentAndClear(data);
    }
  #endif

  if (settingColumns) data.concat(F("</div>"));

  #if DISPLAY_RESET_CONTROLS != OFF
    if (mountType != 0) data.concat(F("<br /><hr>"));
    snprintf_P(temp, sizeof(temp), html_form_begin, "index.htm");
    data.concat(temp);
    data.concat(L_RESET_TITLE "<br/><br/>");
    data.concat(F("<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('boot','reset')\" type='button'>" L_RESET "!</button><br />"));
    data.concat(F("<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('boot','wipe')\" type='button'>" L_WIPE_RESET "!</button>"));
    #if defined(BOOT0_PIN) && DISPLAY_RESET_CONTROLS == FWU
      data.concat(F(" &nbsp;&nbsp;<button " SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('boot','fwu')\" type='button'>" L_RESET_FWU "!</button>"));
    #endif
    data.concat(FPSTR(html_form_end));
    data.concat(F("<br/>\n"));
    data.concat(FPSTR(html_resetNotes));
    www.sendContentAndClear(data);
  #endif

  if (displayCollapsable)
  {
    data.concat(FPSTR(html_collapsable_end));
    www.sendContentAndClear(data);
  }

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void statusTileAjax(String &data)
{
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    data.concat(F("tphd_m|"));
    data.concat(state.controllerTemperatureStr);
    data.concat("\n");
  #endif

  data.concat(F("last_err|"));
  data.concat(state.lastErrorStr);
  data.concat("\n");

  if (status.getVersionMajor() < 10)
  {
    data.concat(F("work_load|"));
    data.concat(state.workLoadStr);
    data.concat("\n");
  }

  #if OPERATIONAL_MODE == WIFI
    data.concat(F("signal|"));
    data.concat(state.signalStrengthStr);
    data.concat("\n");
  #endif

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void statusTileGet()
{
  char temp[80] = "";

  String ssa = www.arg("boot");
  #if DISPLAY_RESET_CONTROLS != OFF
    if (ssa.equals("reset"))
    {
      delay(1000);
      #if !defined(RESET_PIN) || RESET_PIN == OFF
        onStep.commandBlind(":ERESET#");
      #else
        digitalWrite(RESET_PIN, RESET_PIN_STATE);
        pinMode(RESET_PIN, OUTPUT);
        delay(250);
        pinMode(RESET_PIN, INPUT);
      #endif

      // and also attempt to reset the SWS
      tasks.yield(1000);
      HAL_RESET();

      return;
    }
    if (ssa.equals("wipe"))
    {
      delay(1000);
      #if !defined(RESET_PIN) || RESET_PIN == OFF
        onStep.commandString(":ENVRESET#");
        delay(5000);
        onStep.commandBlind(":ERESET#");
      #else
        onStep.commandString(":ENVRESET#");
        delay(5000);
        digitalWrite(RESET_PIN, RESET_PIN_STATE);
        pinMode(RESET_PIN, OUTPUT);
        delay(250);
        pinMode(RESET_PIN, INPUT);
      #endif

      delay(250);
      return;
    }

    #ifdef BOOT0_PIN
      if (ssa.equals("fwu"))
      {
        digitalWrite(BOOT0_PIN, HIGH);
        pinMode(BOOT0_PIN, OUTPUT);

        #if !defined(RESET_PIN) || RESET_PIN == OFF
          onStep.commandBlind(":ERESET#");
        #else
          digitalWrite(RESET_PIN, RESET_PIN_STATE);
          pinMode(RESET_PIN, OUTPUT);
          delay(250);
          pinMode(RESET_PIN, INPUT);
        #endif

        delay(250);
        pinMode(BOOT0_PIN, INPUT);
        return;
      }
    #endif
  #endif

  String ssm = www.arg("mountt");
  if (!ssm.equals(EmptyStr))
  {
    snprintf(temp, sizeof(temp), ":SXEM,%s#", ssm.c_str());
    onStep.commandBool(temp);
  }
}
