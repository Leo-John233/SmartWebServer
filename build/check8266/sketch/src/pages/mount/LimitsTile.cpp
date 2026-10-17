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

  snprintf_P(temp, sizeof(temp), html_tile_text_beg, "22em", "15em", L_BACKLASH " / " L_LIMITS_TITLE);
  // 卡片标记用于响应式布局，不改变设备功能判断
  data.concat(temp);
  data.concat(F("<div class='sws-card-marker' data-card='limits'></div>"));

  data.concat(F("<br /><hr>"));

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

  // 状态值与设置项使用相同的语言资源，数值单独对齐
  data.concat(F("<div class='sws-limit-values'>"));
  // show values
  snprintf(temp, sizeof(temp), "<div><span>" L_BACKLASH_RANGE_AXIS1 "</span><strong id='limBl1' class='c'>%d</strong></div>", backlashAxis1);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "<div><span>" L_BACKLASH_RANGE_AXIS2 "</span><strong id='limBl2' class='c'>%d</strong></div>", backlashAxis2);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "<div><span>" L_LIMITS_RANGE_HORIZON "</span><strong id='limAltMin' class='c'>%d</strong></div>", minAlt);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "<div><span>" L_LIMITS_RANGE_OVERHEAD "</span><strong id='limAltMax' class='c'>%d</strong></div>", maxAlt);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "<div><span>" L_LIMITS_RANGE_MERIDIAN_E "</span><strong id='limMerE' class='c'>%d</strong></div>", degPastMerE);
  data.concat(temp);
  snprintf(temp, sizeof(temp), "<div><span>" L_LIMITS_RANGE_MERIDIAN_W "</span><strong id='limMerW' class='c'>%d</strong></div>", degPastMerW);
  data.concat(temp);

  data.concat(F("</div><hr>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_form_begin, "mount.htm");
  data.concat(temp);

  // Backlash
  data.concat(F("<div class='sws-limits-form'><fieldset><legend>" L_BACKLASH "</legend>"));

  snprintf_P(temp, sizeof(temp), html_configBlAxis1, backlashAxis1);
  data.concat(temp);
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_configBlAxis2, backlashAxis2);
  data.concat(temp);

  data.concat(F("</fieldset><fieldset><legend>" L_LIMITS_TITLE "</legend>"));

  // Overhead and Horizon Limits
  snprintf_P(temp, sizeof(temp), html_configMinAlt, minAlt);
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_configMaxAlt, maxAlt);
  data.concat(temp);

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

  data.concat(F("</fieldset><div class='sws-form-footer'><button type='submit'>" L_UPLOAD "</button></div></div>\n"));

  data.concat(FPSTR(html_form_end));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
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
