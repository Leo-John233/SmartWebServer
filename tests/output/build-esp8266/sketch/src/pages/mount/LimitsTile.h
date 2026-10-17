#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\mount\\LimitsTile.h"
// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Limits and Backlash tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void limitsTile(String &data);
extern void limitsTileAjax(String &data);
extern void limitsTileGet();

// Misc
const char html_configFormBegin[] PROGMEM = "<div class='content'><br />\n<form method='get' action='/configuration.htm'>";
const char html_configFormEnd[] PROGMEM = "\n</form><br />\n</div><br />\n";

// Overhead and Horizon limits
const char html_configMinAlt[] PROGMEM =
  "<label class='sws-number-field'><span>" L_LIMITS_RANGE_HORIZON "</span><input value='%d' type='number' name='hl' min='-30' max='30'></label>\n";

const char html_configMaxAlt[] PROGMEM =
  "<label class='sws-number-field'><span>" L_LIMITS_RANGE_OVERHEAD "</span><input value='%d' type='number' name='ol' min='60' max='90'></label>\n";

const char html_configBlAxis1[] PROGMEM =
  "<label class='sws-number-field'><span>" L_BACKLASH_RANGE_AXIS1 "</span><input value='%d' type='number' name='b1' min='0' max='3600'></label>\n";

const char html_configBlAxis2[] PROGMEM =
  "<label class='sws-number-field'><span>" L_BACKLASH_RANGE_AXIS2 "</span><input value='%d' type='number' name='b2' min='0' max='3600'></label>\n";

const char html_configPastMerE[] PROGMEM =
  "<label class='sws-number-field'><span>" L_LIMITS_RANGE_MERIDIAN_E "</span><input value='%d' type='number' name='el' min='-270' max='270'></label>\n";

const char html_configPastMerW[] PROGMEM =
  "<label class='sws-number-field'><span>" L_LIMITS_RANGE_MERIDIAN_W "</span><input value='%d' type='number' name='wl' min='-270' max='270'></label>\n";

