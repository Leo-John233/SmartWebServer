// -----------------------------------------------------------------------------------
// Goto tile
#include "GotoTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

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
  char temp[400] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", "Goto");
  data.concat(temp);

  data.concat(F("<div style='float: right; text-align: right;' id='gto_status' class='c'>"));
  snprintf(temp, sizeof(temp), "%s || %c", status.inGoto ? L_SLEWING : L_INACTIVE, state.pierSideStr[0]);
  data.concat(temp);
  data.concat(F("</div><br /><hr>"));

  data.concat(FPSTR(html_mountPositionLabels));
  snprintf_P(temp, sizeof(temp), html_mountPositionAxis1, state.indexAzmStr, state.indexRaStr, state.targetRaStr);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_mountPositionAxis2, state.indexAltStr, state.indexDecStr, state.targetDecStr);
  data.concat(temp);

  www.sendContentAndClear(data);

  data.concat(FPSTR(html_gotoGo));
  data.concat(F("&nbsp;&nbsp;&nbsp;"));
  data.concat(FPSTR(html_gotoStop));
  data.concat(F("&nbsp;&nbsp;&nbsp;"));
  data.concat(FPSTR(html_gotoContinue));
  data.concat(F("<hr>"));
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  // Slew speed
  snprintf_P(temp, sizeof(temp), html_slewSpeed, state.slewSpeedStr);
  data.concat(temp);
  data.concat(FPSTR(html_slewSpeedSelect));

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

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void gotoTileAjax(String &data)
{

  char pss[2] = "N";
  pss[0] = state.pierSideStr[0];

  data.concat(keyValueString("gto_status", status.inGoto ? L_SLEWING : L_INACTIVE, " || ", pss));

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
  String value = www.arg("goto");
  if (!value.equals(EmptyStr)) performMountGotoAction(value);
}
