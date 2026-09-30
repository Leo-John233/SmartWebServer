// -----------------------------------------------------------------------------------
// Rotator Slewing tile
#include "SlewingTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

// create the related webpage tile
void rotatorSlewingTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "13em", L_SLEWING);
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;' id='rot_sta' class='c'>"));
  if (state.rotatorSlewing) data.concat(L_ACTIVE); else data.concat(L_INACTIVE);
  data.concat(F("</div><br /><hr>"));

  data.concat(L_CURRENT ": <span id='rotatorpos' class='c'>");
  data.concat(state.rotatorPositionStr);
  data.concat(F("</span><br /><br />"));

  data.concat(FPSTR(html_rotPosition));
  data.concat(FPSTR(html_rotStop));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_script_motion_controls_1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_2));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_rotateCcw));
  data.concat(FPSTR(html_rotateCw));

  data.concat(F("<hr>"));

  if (status.getVersionMajor() >= 10) {
    snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
    data.concat(temp);

    snprintf_P(temp, sizeof(temp), html_rotateSlewSpeed, state.rotateSlewSpeedStr);
    data.concat(temp);
    data.concat(FPSTR(html_rotateGotoSelect));

    data.concat(FPSTR(html_collapsable_end));
  }

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void rotatorSlewingTileAjax(String &data)
{
  data.concat(keyValueString("rot_sta", state.rotatorSlewing ? L_ACTIVE : L_INACTIVE));
  data.concat(keyValueString("rotatorpos", state.rotatorPositionStr));
  data.concat(keyValueString("rot_rate", state.rotateSlewSpeedStr));

  String s;
  s = keyValueBoolSelected("rot_rate_vs", state.rotatorGotoRate == 1); data.concat(s);
  s = keyValueBoolSelected("rot_rate_s", state.rotatorGotoRate == 2); data.concat(s);
  s = keyValueBoolSelected("rot_rate_n", state.rotatorGotoRate == 3); data.concat(s);
  s = keyValueBoolSelected("rot_rate_f", state.rotatorGotoRate == 4); data.concat(s);
  s = keyValueBoolSelected("rot_rate_vf", state.rotatorGotoRate == 5); data.concat(s);

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void rotatorSlewingTileGet()
{
  String value = www.arg("rot");
  if (!value.equals(EmptyStr)) performRotatorAction("rot", value);
  value = www.arg("rs");
  if (!value.equals(EmptyStr)) performRotatorAction("rs", value);
}
