// -----------------------------------------------------------------------------------
// Align tile
#include "AlignTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"
#include "../../extensions/ui/Template.h"

// create the related webpage tile
void alignTile(String &data)
{

  // 卡片标记用于响应式布局，不改变设备功能判断
  appendPageFormat(data, html_tile_beg, "22em", "15em", L_ALIGN);
  data.concat(F("<div class='sws-card-marker' data-card='align'></div>"));

  data.concat(F("<div style='float: right; text-align: right;' id='align_progress' class='c'>"));
  data.concat(state.alignProgress);
  data.concat(F("</div><br /><hr>"));

  // 天极名称直接引用译文，避免复制到局部数组
  const char *poleName = L_ZENITH;
  if (status.mountType != MT_ALTAZM) poleName = state.latitude < 0 ? L_SCP : L_NCP;

  appendPageFormat(data, html_alignCorrection, state.alignLrStr, state.alignUdStr, poleName);
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
  data.concat(F("<div class='sws-actions'>"));
  data.concat(FPSTR(html_alignAccept));
  for (int i = 0; i < n; i++)
  {
    appendPageFormat(data, html_alignStart, i + 1, sc[i], sc[i], SIDEREAL_CH);
  }
  www.sendContentAndClear(data);

  data.concat(F("</div><hr>"));

  if (status.mountType != MT_ALTAZM)
  {
    appendPageFormat(data, html_collapsable_beg, L_CONTROLS "...");

    data.concat(F("<h3 class='sws-refine-title'>" L_REFINE_POLAR_ALIGN "</h3>"));
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
  if (data.length() >= 768) { www.sendContentAndClear(data); }
}

// pass related data back to OnStep
extern void alignTileGet()
{
  String v;
  v = www.arg("al");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("1")) onStep.commandBool(":A1#");
    if (v.equals("2")) onStep.commandBool(":A2#");
    if (v.equals("3")) onStep.commandBool(":A3#");
    if (v.equals("4")) onStep.commandBool(":A4#");
    if (v.equals("5")) onStep.commandBool(":A5#");
    if (v.equals("6")) onStep.commandBool(":A6#");
    if (v.equals("7")) onStep.commandBool(":A7#");
    if (v.equals("8")) onStep.commandBool(":A8#");
    if (v.equals("9")) onStep.commandBool(":A9#");
    if (v.equals("n")) onStep.commandBool(":A+#");
    if (v.equals("q")) onStep.commandBlind(":Q#");
  }
  // refine polar align
  v = www.arg("rp");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("a")) onStep.commandBool(":MP#");
  }
}
