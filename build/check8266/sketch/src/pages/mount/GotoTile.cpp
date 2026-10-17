#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\mount\\GotoTile.cpp"
// -----------------------------------------------------------------------------------
// Goto tile
#include "GotoTile.h"
#include "CoordinateInput.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#define KV_GTO_STATUS 0
#define KV_GTO_I1 1
#define KV_GTO_I2 2
#define KV_GTO_T1 3
#define KV_GTO_T2 4
#define KV_GTO_ACTIVE 5
#define KV_GTO_BZR_OFF 6
#define KV_GTO_BZR_ON 7
#define KV_GTO_MFA_OFF 8
#define KV_GTO_MFA_ON 9
#define KV_GTO_MFP_OFF 10
#define KV_GTO_MFP_ON 11
#define KV_GTO_RATE 12
#define KV_GTO_RATE_VF 13
#define KV_GTO_RATE_F 14
#define KV_GTO_RATE_N 15
#define KV_GTO_RATE_S 16
#define KV_GTO_RATE_VS 17

const char *goto_key[18] =
{
  "gto_rate",
  "gto_rate_vf",
  "gto_rate_f",
  "gto_rate_n",
  "gto_rate_s",
  "gto_rate_vs"
};

// create the related webpage tile
void gotoTile(String &data)
{
  char temp[640] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_GOTO_ACTION);
  data.concat(temp);

  data.concat(F("<div style='float: right; text-align: right;' id='gto_status' class='c'>"));
  snprintf(temp, sizeof(temp), "%s", status.inGoto ? L_SLEWING : L_INACTIVE);
  data.concat(temp);
  data.concat(F("</div><div class='sws-pier-status'>" L_PIER_SIDE " <span id='gto_pier'>"));
  data.concat(state.pierSideStr);
  data.concat(F("</span></div><hr>"));

  // 坐标信息在左，常用定位操作在右
  data.concat(F("<div class='sws-goto-overview'>"));
  data.concat(FPSTR(html_mountPositionLabels));
  snprintf_P(temp, sizeof(temp), html_mountPositionAxis1, state.indexAzmStr, state.indexRaStr, state.targetRaStr);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_mountPositionAxis2, state.indexAltStr, state.indexDecStr, state.targetDecStr);
  data.concat(temp);

  www.sendContentAndClear(data);

  // 四个操作按钮组成两行，窄屏时整体移到坐标下方
  data.concat(F("<div class='sws-actions'>"));
  data.concat(FPSTR(html_gotoGo));
  data.concat(FPSTR(html_gotoStop));
  data.concat(FPSTR(html_gotoContinue));
  data.concat(FPSTR(html_gotoSync));
  data.concat(F("</div></div>"));
  data.concat(FPSTR(html_coordinateInput));
  data.concat(F("<hr>"));
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  // 定位选项采用独立分组，长文字在窄屏自然换行
  data.concat(F("<div class='sws-option-grid'><div class='sws-option-row'>"));
  // Slew speed
  snprintf_P(temp, sizeof(temp), html_slewSpeed, state.slewSpeedStr);
  data.concat(temp);
  data.concat(FPSTR(html_slewSpeedSelect));
  data.concat(F("</div>"));

  // Goto Buzzer
  data.concat(FPSTR(html_gotoBuzzer));
  www.sendContentAndClear(data);

  // Goto Meridian Flips
  if (status.mountType == MT_GEM || (status.getVersionMajor() >= 10 && status.meridianFlips))
  {
    if (status.mountType == MT_ALTAZM) {
      data.concat(FPSTR(html_gotoMfNow));
      snprintf_P(temp, sizeof(temp), html_gotoMfPause, L_ORIENTATION_CHANGE_PAUSE);
      data.concat(temp);
    } else {
      data.concat(FPSTR(html_gotoMfAuto));
      if (status.hasMeridianFlipHomeModes() && (status.mountType == MT_GEM || status.mountType == MT_FORK)) {
        data.concat(FPSTR(html_gotoMfHome));
      } else {
        snprintf_P(temp, sizeof(temp), html_gotoMfPause, L_MERIDIAN_FLIP_PAUSE);
        data.concat(temp);
      }
    }

    www.sendContentAndClear(data);
  }

  // Goto Preferred Pier Side
  if (status.mountType != MT_ALTAZM || (status.getVersionMajor() >= 10 && status.meridianFlips))
  {
    data.concat(F("<br />"));

    if (status.mountType == MT_ALTAZM) {
      snprintf_P(temp, sizeof(temp), html_gotoPreferredPierSide1, L_ORIENTATION_CHANGE_PPS, L_NORMAL, L_ALTERNATE);
      data.concat(temp);
    } else {
      snprintf_P(temp, sizeof(temp), html_gotoPreferredPierSide1, L_MERIDIAN_FLIP_PPS, L_EAST, L_WEST);
      data.concat(temp);
    }
    www.sendContentAndClear(data);

    data.concat(FPSTR(html_gotoPreferredPierSide2));
  }

  data.concat(F("</div>"));
  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void gotoTileAjax(String &data)
{

  // 状态与镜筒侧分别更新，完整保留中文字符串
  data.concat(keyValueString("gto_status", status.inGoto ? L_SLEWING : L_INACTIVE));
  data.concat(keyValueString("gto_pier", state.pierSideStr));

  data.concat(keyValueString("gto_t1", state.targetRaStr));
  data.concat(keyValueString("gto_t2", state.targetDecStr));
  data.concat(keyValueString("gto_i1", state.indexRaStr));
  data.concat(keyValueString("gto_i2", state.indexDecStr));
  data.concat(keyValueString("gto_az1", state.indexAzmStr));
  data.concat(keyValueString("gto_az2", state.indexAltStr));

  data.concat(keyValueBoolEnabled("gto_active", status.inGoto));

  data.concat(keyValueToggleBoolSelected("gto_bzr_on", "gto_bzr_off", status.buzzerEnabled));

  if (status.mountType == MT_GEM || (status.getVersionMajor() >= 10 && status.meridianFlips))
  {
    data.concat(keyValueBoolEnabled("gto_mfa_on", true));
    data.concat(keyValueBoolEnabled("gto_mfa_off", true));
    data.concat(keyValueToggleBoolSelected("gto_mfa_on", "gto_mfa_off", status.autoMeridianFlips));
    if (status.hasMeridianFlipHomeModes() && (status.mountType == MT_GEM || status.mountType == MT_FORK)) {
      data.concat(keyValueBoolSelected("gto_mfh_off", status.meridianFlipHomeMode == MFHM_OFF));
      data.concat(keyValueBoolSelected("gto_mfh_visit", status.meridianFlipHomeMode == MFHM_VISIT));
      data.concat(keyValueBoolSelected("gto_mfh_pause", status.meridianFlipHomeMode == MFHM_PAUSE));
    } else {
      data.concat(keyValueToggleBoolSelected("gto_mfp_on", "gto_mfp_off", status.meridianFlipHomeMode == MFHM_PAUSE));
    }
  } else {
    data.concat(keyValueBoolEnabled("gto_mfa_on", false));
    data.concat(keyValueBoolEnabled("gto_mfa_off", false));
  }

  if (status.mountType != MT_ALTAZM || (status.getVersionMajor() >= 10 && status.meridianFlips))
  {
    data.concat(keyValueBoolSelected("gto_pps_east", state.preferredPierSideChar == 'E'));
    data.concat(keyValueBoolSelected("gto_pps_west", state.preferredPierSideChar == 'W'));
    data.concat(keyValueBoolSelected("gto_pps_best", state.preferredPierSideChar == 'B'));
    data.concat(keyValueBoolSelected("gto_pps_auto", state.preferredPierSideChar == 'A'));
  }

  data.concat(keyValueString("gto_rate", state.slewSpeedStr));

  if (!isnan(state.slewSpeedNominal) && !isnan(state.slewSpeedCurrent))
  {
    float rateRatio = state.slewSpeedNominal / state.slewSpeedCurrent;
    char rate_key[5][12] = {"gto_rate_vf", "gto_rate_f", "gto_rate_n", "gto_rate_s", "gto_rate_vs"};
    bool rate_en[5] = {false, false, false, false, false};

    if (rateRatio > 1.75F) { rate_en[0] = true; }
    else if (rateRatio > 1.25F) { rate_en[1] = true; }
    else if (rateRatio > 0.75F) { rate_en[2] = true; }
    else if (rateRatio > 0.583F) { rate_en[3] = true; }
    else rate_en[4] = true;
    for (int i = 0; i < 5; i++) {
      String s = keyValueBoolSelected(rate_key[i], rate_en[i]);
      data.concat(s);
    }
  }

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void gotoTileGet()
{
  String v;
  char temp[80];

  v = www.arg("goto");
  if (!v.equals(EmptyStr)) {
    if (v.equals("vs")) onStep.commandBool(":SX93,5#");      // very slow, 0.5 x
    if (v.equals("s"))  onStep.commandBool(":SX93,4#");      // slow,      0.75x
    if (v.equals("n"))  onStep.commandBool(":SX93,3#");      // normal,    1.0 x
    if (v.equals("f"))  onStep.commandBool(":SX93,2#");      // fast,      1.5 x
    if (v.equals("vf")) onStep.commandBool(":SX93,1#");      // very fast, 2.0 x

    if (v.equals("bzr_on"))  onStep.commandBool(":SX97,1#"); // alert buzzer on
    if (v.equals("bzr_off")) onStep.commandBool(":SX97,0#"); // alert buzzer off

    if (v.equals("af_now")) onStep.commandBool(":MN#");      // auto-flip, now
    if (v.equals("af_on")) onStep.commandBool(":SX95,1#");   // auto-flip, on
    if (v.equals("af_off")) onStep.commandBool(":SX95,0#");  // auto-flip, off

    if (v.equals("mp_on")) onStep.commandBool(":SX98,1#");   // meridian-flip, pause at home on
    if (v.equals("mp_off")) onStep.commandBool(":SX98,0#");  // meridian-flip, pause at home off
    if (v.equals("mp_cnt")) onStep.commandBool(":SX99,1#");  // meridian flip, pause->continue

    if (status.hasMeridianFlipHomeModes() && (status.mountType == MT_GEM || status.mountType == MT_FORK)) {
      if (v.equals("mh_off"))   onStep.commandBool(":SX94,0#"); // meridian-flip, direct slew
      if (v.equals("mh_visit")) onStep.commandBool(":SX94,1#"); // meridian-flip, visit home
      if (v.equals("mh_pause")) onStep.commandBool(":SX94,2#"); // meridian-flip, pause at home
    }

    if (v.equals("pps_e")) onStep.commandBool(":SX96,E#");   // meridian-flip, preferred pier side East
    if (v.equals("pps_w")) onStep.commandBool(":SX96,W#");   // meridian-flip, preferred pier side West
    if (v.equals("pps_b")) onStep.commandBool(":SX96,B#");   // meridian flip, preferred pier side Best
    if (v.equals("pps_a")) onStep.commandBool(":SX96,A#");   // meridian flip, preferred pier side Auto

    if (v.equals("go")) onStep.command(":MS#", temp);        // goto start
    if (v.equals("stop")) onStep.commandBlind(":Q#");        // goto/slew stop
  }
}

// 一次请求内校验并设置完整坐标，任一步失败均不继续启动移动或同步
void gotoCoordinatesGet() {
  const char *names[7] = {"ra_h", "ra_m", "ra_s", "dec_sign", "dec_d", "dec_m", "dec_s"};
  String parts[7]; const char *values[7];
  for (int i = 0; i < 7; ++i) { parts[i] = www.arg(names[i]); values[i] = parts[i].c_str(); }
  char ra[24], dec[24], reply[80] = "";
  String action = www.arg("coord_action");
  if ((!action.equals("go") && !action.equals("sync")) || !coordinateCommands(values, ra, dec)) {
    www.sendContent("invalid"); return;
  }
  if (!status.onStepFound || !onStep.commandBool(ra) || !onStep.commandBool(dec)) {
    www.sendContent("rejected"); return;
  }
  bool accepted;
  if (action.equals("sync")) accepted = onStep.commandBool(":CS#");
  else accepted = onStep.command(":MS#", reply) && reply[0] == '0';
  www.sendContent(accepted ? "ok" : "rejected");
}
