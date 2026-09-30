// -----------------------------------------------------------------------------------
// Tracking tile
#include "TrackingTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

// create the related webpage tile
void trackingTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_TRACKING);
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;' id='track' class='c'>"));
  data.concat(state.trackStr);
  data.concat(F("</div><br /><hr>"));

  data.concat(FPSTR(html_trackingEnable));
  data.concat(FPSTR(html_trackingPreset));
  www.sendContentAndClear(data);
  data.concat(F("<hr>"));

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
  data.concat(FPSTR(html_tile_end));
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
  const char* keys[] = {"tk", "rr", "dr"};
  for (unsigned int index = 0; index < sizeof(keys) / sizeof(keys[0]); index++) {
    String value = www.arg(keys[index]);
    if (!value.equals(EmptyStr)) performMountTrackingAction(keys[index], value);
  }
}
