// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Date/Time/Location tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void siteTile(String &data);
extern void siteTileAjax(String &data);
extern void siteTileGet();

const char html_browserTime[] PROGMEM =
  "<div class='sws-browser-time'><span id=\"datetime\"></span> " L_TIME_BROWSER_UT
  "<script>update_date_time();</script>"
  "</div>\n";

const char html_date[] PROGMEM = "<div class='sws-device-time'><span id='date_ut' class='c'>%s</span>";
const char html_time[] PROGMEM = "&nbsp;<span id='time_ut' class='c'>%s</span>&nbsp;" L_UT;
const char html_sidereal[] PROGMEM = "</div><div class='sws-sidereal-time'>(<span id='time_lst' class='c'>%s</span>&nbsp; " L_LST ")</div>";
const char html_site[] PROGMEM =
"<div class='sws-location-values'><div>" L_LAT "<strong id='site_lat' class='c'>%s</strong></div>"
"<div>" L_LONG "<strong id='site_long' class='c'>%s</strong></div></div>";
const char html_setDateTime[] PROGMEM =
  "<form style='display: inline' method='get' action='/mount.htm'>\n"
  "<div class='sws-clock-action'>"
  "<small><button name='qb' class='bb' value='st' onpointerdown='SetDateTime();' type='submit'>" L_HINT_CLOCK "</button></small>"
  "</div>\n"
  "<input id='dm' type='hidden' name='dm'><input id='dd' type='hidden' name='dd'><input id='dy' type='hidden' name='dy'>"
  "<input id='th' type='hidden' name='th'><input id='tm' type='hidden' name='tm'><input id='ts' type='hidden' name='ts'>\n"
  "</form>\n";

const char html_longMsg[] PROGMEM = "<fieldset class='sws-field-group'><legend>" L_LOCATION_LONG "</legend><div class='sws-unit-inputs'>";
const char html_latMsg[] PROGMEM = "<fieldset class='sws-field-group'><legend>" L_LOCATION_LAT "</legend><div class='sws-unit-inputs'>";
const char html_offsetMsg[] PROGMEM = "<fieldset class='sws-field-group'><legend>" L_LOCATION_RANGE_UTC_OFFSET "</legend><div class='sws-unit-inputs'>";
const char html_offsetMin[] PROGMEM =
"&nbsp;<select name='u2'><option value='0' %s>00</option><option value='30' %s>30</option><option value='45' %s>45</option></select>&nbsp;m\n";
const char html_offsetFooterMsg[] PROGMEM = "</div><p class='sws-help'>" L_LOCATION_MESSAGE_UTC_OFFSET "</p></fieldset>";
