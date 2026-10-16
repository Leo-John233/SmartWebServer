// -----------------------------------------------------------------------------------
// Focuser Home tile
#include "HomeTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// 两种设备只提供参考位置操作，不包含赤道仪停放功能
void focuserHomeTile(String &data)
{
  data.concat(F("<div class='b1 sws-accessory-home-card'><div class='sws-device-card-head'><h2>" L_HOME_ACTIONS "</h2></div><p class='sws-accessory-hint'>" L_HOME_REFERENCE_HELP "</p><div class='sws-accessory-home-actions'>"));
  data.concat(FPSTR(html_focuserHome));
  data.concat(F("</div></div>"));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void focuserHomeTileAjax(String &data)
{
  UNUSED(data);
}

// pass related data back to OnStep
extern void focuserHomeTileGet()
{
  String v;
  v = www.arg("dr");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("FH")) onStep.commandBlind(":FH#"); // reset focuser at home (half travel)
    if (v.equals("Fh")) onStep.commandBlind(":Fh#"); // move focuser to home (half travel)
  }

}
