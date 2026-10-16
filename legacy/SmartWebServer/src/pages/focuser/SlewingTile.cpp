// -----------------------------------------------------------------------------------
// Focuser Slewing tile
#include "SlewingTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// 位置读数、目标输入与手动移动分别分组
void focuserSlewingTile(String &data)
{
  char temp[240] = "";
  data.concat(F("<div class='b1 sws-accessory-motion-card'><div class='sws-device-card-head'><h2>" L_FOCUSER "</h2><span id='foc_sta' class='sws-accessory-status c'>"));
  data.concat(state.focuserSlewing ? L_ACTIVE : L_INACTIVE);
  data.concat(F("</span></div><div class='sws-accessory-position'><span>" L_CURRENT "</span><strong id='focuserpos' class='c'>"));
  data.concat(state.focuserPositionStr);
  data.concat(F("</strong></div><div class='sws-accessory-goto'>"));
  data.concat(FPSTR(html_focPosition));
  if (status.getVersionMajor() >= 10) data.concat(FPSTR(html_focStop));
  data.concat(F("</div><p class='sws-accessory-hint'>" L_HOLD_TO_MOVE "</p><div class='sws-accessory-jog'>"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_2));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_focuserIn));
  data.concat(FPSTR(html_focuserOut));
  data.concat(F("</div>"));
  if (status.getVersionMajor() >= 10) {
    data.concat(FPSTR(html_focuserGotoSelect));
    snprintf_P(temp, sizeof(temp), html_focuserSlewSpeed, state.focuserSlewSpeedStr);
    data.concat(temp);
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
  String v;

  v = www.arg("foc");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("stop")) onStep.commandBlind(":FQ#");     // stop
    if (v.equals("inf"))  onStep.commandBlind(":F4#:F-#"); // rate fast, move in
    if (v.equals("in"))   onStep.commandBlind(":F2#:F-#"); // rate slow, move in
    if (v.equals("out"))  onStep.commandBlind(":F2#:F+#"); // rate slow, move out
    if (v.equals("outf")) onStep.commandBlind(":F4#:F+#"); // rate fast, move out

    if (v.equals("vs"))   onStep.commandBlind(":F5#");     // goto rate very slow
    if (v.equals("s"))    onStep.commandBlind(":F6#");     // goto rate slow
    if (v.equals("n"))    onStep.commandBlind(":F7#");     // goto rate normal
    if (v.equals("f"))    onStep.commandBlind(":F8#");     // goto rate fast
    if (v.equals("vf"))   onStep.commandBlind(":F9#");     // goto rate very fast
  }

  // set position
  v = www.arg("fs");
  if (!v.equals(EmptyStr))
  {
    int p = v.toInt();
    if (p >= -500000 || p <= 500000)
    {
      char temp[80];
      snprintf(temp, sizeof(temp), ":FS%d#", p);
      onStep.commandBool(temp);
    }
  }

}
