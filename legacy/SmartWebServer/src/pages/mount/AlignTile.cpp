// -----------------------------------------------------------------------------------
// Align tile
#include "AlignTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../api/SemanticActions.h"

// create the related webpage tile
void alignTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_ALIGN);
  data.concat(temp);

  data.concat(F("<div style='float: right; text-align: right;' id='align_progress' class='c'>"));
  data.concat(state.alignProgress);
  data.concat(F("</div><br /><hr>"));

  char poleName[8] = L_ZENITH;
  if (status.mountType != MT_ALTAZM) { if (state.latitude < 0) strcpy(poleName, L_SCP); else strcpy(poleName, L_NCP); }

  snprintf_P(temp, sizeof(temp), html_alignCorrection, state.alignLrStr, state.alignUdStr, poleName);
  data.concat(temp);
  www.sendContentAndClear(data);

  byte sc[3];
  int n = 1;
  if (status.alignMaxStars < 3)
  {
    n = 1;
    sc[0] = 1;
  }
  else if (status.alignMaxStars < 4)
  {
    n = 3;
    sc[0] = 1;
    sc[1] = 2;
    sc[2] = 3;
  }
  else if (status.alignMaxStars < 6)
  {
    n = 3;
    sc[0] = 1;
    sc[1] = 3;
    sc[2] = 4;
  }
  else if (status.alignMaxStars < 8)
  {
    n = 3;
    sc[0] = 1;
    sc[1] = 3;
    sc[2] = 6;
  }
  else
  {
    n = 3;
    sc[0] = 1;
    sc[1] = 3;
    sc[2] = 9;
  }
  data.concat(FPSTR(html_alignAccept));
  for (int i = 0; i < n; i++)
  {
    char temp2[120] = "";
    sprintf_P(temp2, html_alignStart, i + 1, sc[i], sc[i], SIDEREAL_CH);
    data.concat(temp2);
  }
  www.sendContentAndClear(data);

  data.concat(F("<hr>"));

  if (status.mountType != MT_ALTAZM)
  {
    snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
    data.concat(temp);

    data.concat(L_REFINE_POLAR_ALIGN ":<br /><br />");
    data.concat(FPSTR(html_alignRefine));

    data.concat(FPSTR(html_collapsable_end));
  }
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void alignTileAjax(String &data)
{
  data.concat(keyValueString("align_progress", state.alignProgress));
  data.concat(keyValueString("align_lr", state.alignLrStr));
  data.concat(keyValueString("align_ud", state.alignUdStr));
  data.concat(keyValueBoolEnabled("alg1", !status.tracking && !status.parked && status.atHome));
  data.concat(keyValueBoolEnabled("alg2", !status.tracking && !status.parked && status.atHome));
  data.concat(keyValueBoolEnabled("alg3", !status.tracking && !status.parked && status.atHome));
  data.concat(keyValueBoolEnabled("alga", status.tracking && status.aligning));
  data.concat(keyValueBoolEnabled("rpa", status.tracking && !status.aligning));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void alignTileGet()
{
  String value = www.arg("al");
  if (!value.equals(EmptyStr)) performMountAlignAction("al", value);
  value = www.arg("rp");
  if (!value.equals(EmptyStr)) performMountAlignAction("rp", value);
}
