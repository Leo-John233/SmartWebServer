// -----------------------------------------------------------------------------------
// Tracking tile
#include "TrackingTile.h"
#include "GuideTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// create the related webpage tile
void trackingTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_TRACKING);
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;' id='track' class='c'>"));
  data.concat(state.trackStr);
  data.concat(F("</div><br /><hr>"));

  // 开关和倍率直接输出最终结构，脚本只绑定原有命令与状态同步
  data.concat(F("<div class='sws-track-switch'><span>" L_DEVICE_TRACKING "</span><button class='sws-switch' type='button' role='switch' aria-label='" L_DEVICE_TRACKING "' aria-checked='"));
  data.concat(status.tracking ? F("true") : F("false"));
  data.concat(F("' disabled></button></div><p class='sws-help' role='status' hidden></p>"));
  data.concat(FPSTR(html_trackingEnable));
  data.concat(F("<div class='sws-tracking-rates'>"));
  data.concat(FPSTR(html_trackingPreset));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_guideRates));
  data.concat(FPSTR(html_guidePulseRates));
  data.concat(F("</div>"));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// 跟踪高级设置在原页面分区中直接输出，避免浏览器加载后搬动卡片
void trackingSettingsTile(String &data) {
  char temp[240] = "";
  data.concat(F("<section class='b1 sws-tracking-advanced'><h2>" L_TRACKING_SETTINGS "</h2>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_form_begin, "mount.htm");
  data.concat(temp);
  if (status.mountType != MT_ALTAZM || status.getVersionMajor() >= 10)
  {
    data.concat(FPSTR(html_trackingCompensationType));
    data.concat(FPSTR(html_trackingCompensationAxes));
    www.sendContentAndClear(data);
  }
  data.concat(FPSTR(html_baseFrequency));
  data.concat(FPSTR(html_form_end));

  data.concat(FPSTR(html_collapsable_end));
  data.concat(F("</section>"));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void trackingTileAjax(String &data)
{
  data.concat(keyValueString("track", state.trackStr));

  if (status.mountType != MT_ALTAZM || status.getVersionMajor() >= 10) {
    char temp[16];
    if (status.rateCompensation == RC_NONE) strcpy(temp, ""); else
    if (status.rateCompensation == RC_REFR_RA) strcpy(temp, "RC"); else
    if (status.rateCompensation == RC_REFR_BOTH) strcpy(temp, "RCD"); else
    if (status.rateCompensation == RC_FULL_RA) strcpy(temp, "FC"); else
    if (status.rateCompensation == RC_FULL_BOTH) strcpy(temp, "FCD");
    data.concat(keyValueString("trk_otm", temp));

    data.concat(keyValueBoolSelected("ot_on", status.rateCompensation == RC_FULL_BOTH || status.rateCompensation == RC_FULL_RA));
    data.concat(keyValueBoolSelected("ot_ref", status.rateCompensation == RC_REFR_BOTH || status.rateCompensation == RC_REFR_RA));
    data.concat(keyValueBoolSelected("ot_off", status.rateCompensation == RC_NONE));

    data.concat(keyValueBoolSelected("ot_dul", status.rateCompensation == RC_FULL_BOTH || status.rateCompensation == RC_REFR_BOTH));
    data.concat(keyValueBoolSelected("ot_sgl", status.rateCompensation == RC_FULL_RA || status.rateCompensation == RC_REFR_RA));
  }

  data.concat(keyValueToggleBoolSelected("trk_on", "trk_off", status.tracking));
  data.concat(keyValueBoolSelected("trk_sid", status.tracking && state.trackingSidereal));
  data.concat(keyValueBoolSelected("trk_sol", status.tracking && state.trackingSolar));
  data.concat(keyValueBoolSelected("trk_lun", status.tracking && state.trackingLunar));
  data.concat(keyValueBoolSelected("trk_king", status.tracking && state.trackingKing));

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void trackingTileGet()
{
  String v;

  v = www.arg("tk"); // controller base rate
  if (!v.equals(EmptyStr))
  {
    if (v.equals("f")) onStep.commandBlind(":T+#"); // 0.02hz faster
    if (v.equals("-")) onStep.commandBlind(":T-#"); // 0.02hz slower
    if (v.equals("r")) onStep.commandBlind(":TR#"); // reset
  }

  v = www.arg("rr"); // tracking compensated
  if (!v.equals(EmptyStr))
  {
    if (v.equals("off"))  onStep.commandBool(":Tn#"); // compensated off
    if (v.equals("on"))   onStep.commandBool(":Tr#"); // compensated on
    if (v.equals("doff")) onStep.commandBool(":T1#"); // compensated 1 axis
    if (v.equals("don"))  onStep.commandBool(":T2#"); // compensated 2 axis
    if (v.equals("otk"))  onStep.commandBool(":To#"); // compensated pointing model
  }

  v = www.arg("dr"); // tracking
  if (!v.equals(EmptyStr))
  {
    if (v.equals("T1")) onStep.commandBool(":Te#");  // enable tracking
    if (v.equals("T0")) onStep.commandBool(":Td#");  // disable tracking
    if (v.equals("Ts")) onStep.commandBlind(":TQ#"); // sidereal
    if (v.equals("Tl")) onStep.commandBlind(":TL#"); // lunar
    if (v.equals("Th")) onStep.commandBlind(":TS#"); // solar
    if (v.equals("Tk")) onStep.commandBlind(":TK#"); // king rate
  }
}
