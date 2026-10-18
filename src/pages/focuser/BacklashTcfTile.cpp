// -----------------------------------------------------------------------------------
// Focuser Backlash and TCF tile
#include "BacklashTcfTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// create the related webpage tile
void focuserBacklashTcfTile(String &data)
{
  char temp[240] = "";

  data.concat(F("<div class='b1 sws-accessory-parameter-card'><div class='sws-device-card-head'><h2>" L_BACKLASH " / " L_TCF_COEF "</h2><span id='f_temp' class='sws-accessory-status c'>"));
  snprintf_P(temp, sizeof(temp), L_TEMPERATURE " %s", state.focuserTemperatureStr);
  data.concat(temp);
  data.concat(F("</span></div><div class='sws-accessory-metrics'>"));

  snprintf_P(temp, sizeof(temp), html_backlashValue, state.focuserBacklashStr);
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_tcfEnableValue, state.focuserTcfEnable ? L_ON : L_OFF);
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_tcfDeadbandValue, state.focuserDeadbandStr);
  data.concat(temp);
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_tcfCoefValue, state.focuserTcfCoefStr);
  data.concat(temp);

  data.concat(F("</div>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS);
  data.concat(temp);



  if (status.focuserPresent[state.focuserSelected - 1]) {

    snprintf_P(temp, sizeof(temp), html_form_begin, "focuser.htm");
    data.concat(temp);

    // Backlash
    snprintf_P(temp, sizeof(temp), html_backlash, state.focuserBacklashStr);
    data.concat(temp);

    // TCF Enable
    snprintf_P(temp, sizeof(temp), html_tcfEnable, state.focuserTcfEnable ? "" : "selected", state.focuserTcfEnable ? "selected" : "");
    data.concat(temp);

    // TCF Deadband
    snprintf_P(temp, sizeof(temp), html_tcfDeadband, state.focuserDeadbandStr);
    data.concat(temp);
    www.sendContentAndClear(data);

    // TCF Coef
    snprintf_P(temp, sizeof(temp), html_tcfCoef, state.focuserTcfCoefStr);
    data.concat(temp);
    data.concat(F("<div class='sws-accessory-form-actions'><button type='submit'>" L_SAVE_SETTINGS "</button></div>\n"));

    data.concat(FPSTR(html_form_end));

    www.sendContentAndClear(data);
  }

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void focuserBacklashTcfTileAjax(String &data)
{
  char temp[32];

  snprintf(temp, sizeof(temp), L_TEMPERATURE " %s", state.focuserTemperatureStr);
  data.concat(keyValueString("f_temp", temp));
  data.concat(keyValueString("f_bl", state.focuserBacklashStr, " " L_UNIT_STEPS));
  data.concat(keyValueString("f_tcf_en", state.focuserTcfEnable ? L_ON : L_OFF));
  data.concat(keyValueString("f_tcf_db", state.focuserDeadbandStr, " " L_UNIT_STEPS));
  data.concat(keyValueString("f_tcf_coef", state.focuserTcfCoefStr));

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void focuserBacklashTcfTileGet()
{
  String v;
  char cmd[32] = "";

  // send backlash
  v = www.arg("foc_bl");
  if (!v.equals(EmptyStr) && v.toInt() >= 0 && v.toInt() <= 32767) {
    snprintf(cmd, sizeof(cmd), ":Fb%d#", (int16_t)v.toInt());
    onStep.commandBool(cmd);
  }

  // send TCF en
  v = www.arg("foc_tcf_en");
  if (!v.equals(EmptyStr) && (v.equals("0") || v.equals("1"))) {
    snprintf(cmd, sizeof(cmd), ":Fc%s#", v.c_str());
    onStep.commandBool(cmd);
  }

  // send TCF deadband
  v = www.arg("foc_tcf_db");
  if (!v.equals(EmptyStr) && v.toInt() >= 1 && v.toInt() <= 32767) {
    snprintf(cmd, sizeof(cmd), ":Fd%d#", (int16_t)v.toInt());
    onStep.commandBool(cmd);
  }

  // send TCF coef
  v = www.arg("foc_tcf_coef");
  if (!v.equals(EmptyStr) && v.toFloat() >= -999.0 && v.toFloat() <= 999.0) {
    snprintf(cmd, sizeof(cmd), ":FC%s#", v.c_str());
    onStep.commandBool(cmd);
  }
}
