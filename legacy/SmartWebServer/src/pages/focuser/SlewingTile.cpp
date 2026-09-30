// -----------------------------------------------------------------------------------
// Focuser Slewing tile
#include "SlewingTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

// create the related webpage tile
void focuserSlewingTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "13em", L_SLEWING);
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;' id='foc_sta' class='c'>"));
  if (state.focuserSlewing) data.concat(L_ACTIVE); else data.concat(L_INACTIVE);
  data.concat(F("</div><br /><hr>"));

  data.concat(L_CURRENT ": <span id='focuserpos' class='c'>");
  data.concat(state.focuserPositionStr);
  data.concat(F("</span><br /><br />"));

  data.concat(FPSTR(html_focPosition));
  if (status.getVersionMajor() >= 10) data.concat(FPSTR(html_focStop));
  data.concat(F("<br />"));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_script_motion_controls_1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_2));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_focuserIn));
  data.concat(FPSTR(html_focuserOut));

  data.concat(F("<hr>"));

  if (status.getVersionMajor() >= 10) {
    snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
    data.concat(temp);

    snprintf_P(temp, sizeof(temp), html_focuserSlewSpeed, state.focuserSlewSpeedStr);
    data.concat(temp);
    data.concat(FPSTR(html_focuserGotoSelect));

    data.concat(FPSTR(html_collapsable_end));
  }

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void focuserSlewingTileAjax(String &data)
{
  data.concat(keyValueString("foc_sta", state.focuserSlewing ? L_ACTIVE : L_INACTIVE));
  data.concat(keyValueString("focuserpos", state.focuserPositionStr));
  data.concat(keyValueString("foc_rate", state.focuserSlewSpeedStr));

  String s;
  s = keyValueBoolSelected("foc_rate_vs", state.focuserGotoRate == 1); data.concat(s);
  s = keyValueBoolSelected("foc_rate_s", state.focuserGotoRate == 2); data.concat(s);
  s = keyValueBoolSelected("foc_rate_n", state.focuserGotoRate == 3 ); data.concat(s);
  s = keyValueBoolSelected("foc_rate_f", state.focuserGotoRate == 4); data.concat(s);
  s = keyValueBoolSelected("foc_rate_vf", state.focuserGotoRate == 5); data.concat(s);

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void focuserSlewingTileGet()
{
  String value = www.arg("foc");
  if (!value.equals(EmptyStr)) performFocuserAction("foc", value);
  value = www.arg("fs");
  if (!value.equals(EmptyStr)) performFocuserAction("fs", value);
}
