// -----------------------------------------------------------------------------------
// Rotator Slewing tile
#include "SlewingTile.h"
#include <stdlib.h>

#include "../KeyValue.h"
#include "../Pages.common.h"

// 位置读数、目标输入与手动移动分别分组
void rotatorSlewingTile(String &data)
{
  char temp[240] = "";
  data.concat(F("<div class='b1 sws-accessory-motion-card'><div class='sws-device-card-head'><h2>" L_ROTATOR "</h2><span id='rot_sta' class='sws-accessory-status c'>"));
  data.concat(state.rotatorSlewing ? L_ACTIVE : L_INACTIVE);
  data.concat(F("</span></div><div class='sws-accessory-position'><span>" L_CURRENT "</span><strong id='rotatorpos' class='c'>"));
  data.concat(state.rotatorPositionStr);
  data.concat(F("</strong></div><div class='sws-accessory-goto'>"));
  data.concat(FPSTR(html_rotPosition));
  data.concat(FPSTR(html_rotStop));
  data.concat(F("</div><p class='sws-accessory-hint'>" L_HOLD_TO_MOVE "</p><div class='sws-accessory-jog'>"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_2));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_rotateCcw));
  data.concat(FPSTR(html_rotateCw));
  data.concat(F("</div>"));
  if (status.getVersionMajor() >= 10) {
    data.concat(FPSTR(html_rotateGotoSelect));
    snprintf_P(temp, sizeof(temp), html_rotateSlewSpeed, state.rotateSlewSpeedStr);
    data.concat(temp);
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
  String v;

  // stop movement
  v = www.arg("rot");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("stop")) onStep.commandBlind(":rQ#");         // stop
    if (v.equals("ccwf")) onStep.commandBlind(":r3#:rc#:r<#"); // rate 3, move ccw
    if (v.equals("ccw"))  onStep.commandBlind(":r1#:rc#:r<#"); // rate 1, move ccw
    if (v.equals("cw"))   onStep.commandBlind(":r1#:rc#:r>#"); // rate 1, move cw
    if (v.equals("cwf"))  onStep.commandBlind(":r3#:rc#:r>#"); // rate 3, move cw

    if (v.equals("vs"))   onStep.commandBlind(":r5#");         // goto rate very slow
    if (v.equals("s"))    onStep.commandBlind(":r6#");         // goto rate slow
    if (v.equals("n"))    onStep.commandBlind(":r7#");         // goto rate normal
    if (v.equals("f"))    onStep.commandBlind(":r8#");         // goto rate fast
    if (v.equals("vf"))   onStep.commandBlind(":r9#");         // goto rate very fast
  }

  // set position
  v = www.arg("rs");
  if (!v.equals(EmptyStr))
  {
    // 与网页数值输入一致，仅接受完整十进制数和有效角度范围
    const char *text = v.c_str();
    char *end;
    bool decimal = true;
    for (const char *c = text; *c; c++) {
      if (!((*c >= '0' && *c <= '9') || *c == '+' || *c == '-' || *c == '.' || *c == 'e' || *c == 'E')) decimal = false;
    }
    double f = strtod(text, &end);
    if (decimal && end != text && *end == 0 && f >= -360.0 && f <= 360.0)
    {
      char temp[80], temp1[40];
      convert.doubleToDms(temp1, f, true, true, PM_HIGH);
      snprintf(temp, sizeof(temp), ":rS%s#", temp1);
      onStep.commandBool(temp);
    }
  }

}
