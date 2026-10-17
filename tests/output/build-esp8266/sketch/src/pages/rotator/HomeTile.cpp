#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\rotator\\HomeTile.cpp"
// -----------------------------------------------------------------------------------
// Rotator Home tile
#include "HomeTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// 两种设备只提供参考位置操作，不包含赤道仪停放功能
void rotatorHomeTile(String &data)
{
  data.concat(F("<div class='b1 sws-accessory-home-card'><div class='sws-device-card-head'><h2>" L_HOME_ACTIONS "</h2></div><p class='sws-accessory-hint'>" L_HOME_REFERENCE_HELP "</p><div class='sws-accessory-home-actions'>"));
  data.concat(FPSTR(html_rotatorHome));
  data.concat(F("</div></div>"));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void rotatorHomeTileAjax(String &data)
{
  UNUSED(data);
}

// pass related data back to OnStep
extern void rotatorHomeTileGet()
{
  String v;
  v = www.arg("rot");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("reset")) onStep.commandBlind(":rF#"); // reset rotator at home
    if (v.equals("home"))  onStep.commandBlind(":rC#"); // move rotator to home
  }
}
