#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\mount\\LimitsTile.cpp"
// -----------------------------------------------------------------------------------
// Limits and Backlash tile
#include "LimitsTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// create the related webpage tile
void limitsTile(String &data)
{
  char temp[640] = "";
  char reply[120] = "";

  data.concat(F("<div class='b1 sws-limits sws-editor'><header class='sws-editor-heading'><h2>" L_LIMITS_SETTINGS "</h2><button class='sws-secondary sws-edit-button' type='button' data-edit-form='sws-limits-form'>" SWS_EDIT_ICON L_EDIT_LIMITS "</button></header>"));
  // get backlash and limits
  if (!onStep.command(":%BR#", reply)) strcpy(reply, "0");
  int backlashAxis1 = (int)strtol(&reply[0], NULL, 10);

  if (!onStep.command(":%BD#", reply)) strcpy(reply, "0");
  int backlashAxis2 = (int)strtol(&reply[0], NULL, 10);

   if (!onStep.command(":Gh#", reply)) strcpy(reply, "0");
  int minAlt = (int)strtol(&reply[0], NULL, 10);

  if (!onStep.command(":Go#", reply)) strcpy(reply, "0");
  int maxAlt = (int)strtol(&reply[0], NULL, 10);

  if (!onStep.command(":GXE9#", reply)) strcpy(reply, "0");
  int degPastMerE = (int)strtol(&reply[0], NULL, 10);
  degPastMerE = round((degPastMerE * 15.0) / 60.0);

  if (!onStep.command(":GXEA#", reply)) strcpy(reply, "0");
  int degPastMerW = (int)strtol(&reply[0], NULL, 10);
  degPastMerW = round((degPastMerW * 15.0) / 60.0);

  data.concat(F("<form id='sws-limits-form' class='sws-settings-editor sws-limits-form' method='get' action='/mount.htm'>"));
  // Overhead and Horizon Limits
  snprintf_P(temp, sizeof(temp), html_configMaxAlt, maxAlt); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_configMinAlt, minAlt); data.concat(temp);

  // Meridian Limits
  if (status.mountType == MT_GEM)
  {
    snprintf_P(temp, sizeof(temp), html_configPastMerE, degPastMerE);
    data.concat(temp);

    snprintf_P(temp, sizeof(temp), html_configPastMerW, degPastMerW);
    data.concat(temp);
  }
  else
    data.concat(F("<br />\n"));

  data.concat(F("<details class='sws-backlash-editor'><summary>" L_BACKLASH "</summary>"));
  snprintf_P(temp, sizeof(temp), html_configBlAxis1, backlashAxis1); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_configBlAxis2, backlashAxis2); data.concat(temp);
  data.concat(F("</details><div class='sws-editor-footer'><button type='reset' class='sws-secondary'>" L_UI_CANCEL "</button><button type='submit'>" L_SAVE_SETTINGS "</button></div></form></div>"));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void limitsTileAjax(String &data)
{
  UNUSED(data);
}

// pass related data back to OnStep
extern void limitsTileGet()
{
  String v;
  char temp[80];

  // Overhead limit
  v = www.arg("ol");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 60 && v.toInt() <= 90)
    {
      snprintf(temp, sizeof(temp), ":So%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Horizon limit
  v = www.arg("hl");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -30 && v.toInt() <= 30)
    {
      snprintf(temp, sizeof(temp), ":Sh%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Meridian limit E
  v = www.arg("el");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -270 && v.toInt() <= 270)
    {
      snprintf(temp, sizeof(temp), ":SXE9,%d#", (int16_t)round((v.toInt() * 60.0) / 15.0));
      onStep.commandBool(temp);
    }
  }

  // Meridian limit W
  v = www.arg("wl");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= -270 && v.toInt() <= 270)
    {
      snprintf(temp, sizeof(temp), ":SXEA,%d#", (int16_t)round((v.toInt() * 60.0) / 15.0));
      onStep.commandBool(temp);
    }
  }

  // Backlash
  v = www.arg("b1");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 0 && v.toInt() <= 3600)
    {
      snprintf(temp, sizeof(temp), ":$BR%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  v = www.arg("b2");
  if (!v.equals(EmptyStr))
  {
    if (v.toInt() >= 0 && v.toInt() <= 3600)
    {
      snprintf(temp, sizeof(temp), ":$BD%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }
}
