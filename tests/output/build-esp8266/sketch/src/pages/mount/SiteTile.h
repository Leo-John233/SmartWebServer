#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\mount\\SiteTile.h"
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
  "<p class='sws-browser-clock' hidden><span>" L_TIME_BROWSER_UT "</span><strong id='datetime'></strong><script>update_date_time();</script></p>";
const char html_localTime[] PROGMEM = "<div class='sws-site-clock'><span class='sws-field-label'>" L_SITE_TIME "</span><input id='sws-local-time' readonly aria-label='" L_SITE_TIME "' value='—'></div>";
const char html_date[] PROGMEM = "<div class='sws-astronomy-time'><div class='sws-clock-value'><span class='sws-field-label'>" L_UT "</span><strong><span id='date_ut' class='c'>%s</span>";
const char html_time[] PROGMEM = " <span id='time_ut' class='c'>%s</span></strong></div>";
const char html_sidereal[] PROGMEM = "<div class='sws-sidereal-clock'><span>" L_LST "</span><strong id='time_lst' class='c'>%s</strong></div></div>";
const char html_site[] PROGMEM =
  "<div hidden><span id='site_lat'>%s</span><span id='site_long'>%s</span></div>";
const char html_setDateTime[] PROGMEM =
  "<form style='display: inline' method='get' action='/mount.htm' onsubmit='SetDateTime();'>\n"
  "<div class='sws-clock-action'>"
  "<small><button name='qb' class='bb' value='st' type='submit'>" L_COORD_SYNC "</button></small>"
  "</div>\n"
  "<input id='dm' type='hidden' name='dm'><input id='dd' type='hidden' name='dd'><input id='dy' type='hidden' name='dy'>"
  "<input id='th' type='hidden' name='th'><input id='tm' type='hidden' name='tm'><input id='ts' type='hidden' name='ts'>\n"
  "</form>\n";

const char html_longMsg[] PROGMEM = "<fieldset class='sws-field-group'><legend>" L_LONG "</legend><div class='sws-unit-inputs'>";
const char html_latMsg[] PROGMEM = "<fieldset class='sws-field-group'><legend>" L_LAT "</legend><div class='sws-unit-inputs'>";
const char html_offsetMsg[] PROGMEM = "<fieldset class='sws-field-group'><legend>" L_TIMEZONE "</legend><div class='sws-unit-inputs'>";
const char html_offsetMin[] PROGMEM =
"&nbsp;<select name='u2'><option value='0' %s>00</option><option value='30' %s>30</option><option value='45' %s>45</option></select>&nbsp;m\n";
const char html_offsetFooterMsg[] PROGMEM = "</div><p class='sws-help'>" L_LOCATION_MESSAGE_UTC_OFFSET "</p></fieldset>";
