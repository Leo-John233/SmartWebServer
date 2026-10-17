#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define sprintf_P sprintf
#define ON 1
#define DISPLAY_SPECIAL_CHARS ON
#define OFF 0
#define AJAX_PAGE_LAZY_GET_MS 200
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define MT_GEM 1
#define MT_FORK 2
#define MT_ALTAZM 3
#define HOME_OFFSET_RANGE_AXIS1 3600
#define HOME_OFFSET_RANGE_AXIS2 3600
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
using byte=unsigned char;
struct String:std::string {using std::string::string;void concat(const char*s){append(s);}};
struct Web {std::string output;void sendContentAndClear(String&s){output+=s;s.clear();}} www;
struct Status {bool hasHomeSense=true,pecEnabled=true,inGoto=false,meridianFlips=true,guiding=false;int guideRate=6,mountType=MT_GEM,alignMaxStars=9;int getVersionMajor(){return 10;}int getVersionMinor(){return 28;}bool hasMeridianFlipHomeModes(){return true;}} status;
struct State {const char *dateStr="10/16/26",*timeStr="10:08:38",*lastStr="19:52:26",*latitudeStr="+31°00′00″",*longitudeStr="-121°00′00″",*alignProgress="",*alignLrStr="0″",*alignUdStr="0″",*pierSideStr="中天翻转，由西向东",*indexAzmStr="120°",*indexRaStr="23h57m00s",*targetRaStr="05h35m17s",*indexAltStr="45°",*indexDecStr="+90°00′00″",*targetDecStr="-05°23′28″",*slewSpeedStr="3°/s";const char*trackStr="";double latitude=31;} state;
struct Device {const char* commandString(const char*){return "126";}bool command(const char*s,char*out){const char*v="0";if(!strcmp(s,":GtH#"))v="+31*00:00";if(!strcmp(s,":GgH#"))v="-121*00:00";if(!strcmp(s,":GG#"))v="-08:00";if(!strcmp(s,":h?#"))v="1,-120,-240";if(!strcmp(s,":Go#"))v="80";if(!strcmp(s,":Gh#"))v="-10";if(!strcmp(s,":GXE7#"))v="12800";strcpy(out,v);return true;}} onStep;
struct Convert {void stripNumericStr(char*s){int n=atoi(s);sprintf(s,"%d",n);}} convert;
const char*getHomeParkStateStr(){return "待机";}
const char GuideRatesStr[][8] = {"1/4X", "1/2X", "1X", "2X", "4X", "8X", "20X", "48X", "1/2 Max", "Max"};
#define DISPLAY_LANGUAGE L_en
// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define PROD_ABV                     "SWS"

#define METRIC                       ON
#define IMPERIAL                     -200
#define LOCALE_DEFAULT               -201
#define AUTO_ON                      -202
#define AUTO_OFF                     -203
#define FWU                          -204

// ajax page update feature
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 500     // fast update rate
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 5000    // time before return to normal update rate
#define AJAX_PAGE_LAZY_GET_MS         1000    // wait time for lazy get

// various auxillary features
#define SWITCH                       1
#define ANALOG_OUTPUT                2
#define DEW_HEATER                   3
#define INTERVALOMETER               4
#define COVER_SWITCH                 7

// The settings below are for initialization only, afterward they are stored and recalled from EEPROM and must
// be changed in the web interface OR with a reset (for initialization again) as described in the Config.h comments
#define TIMEOUT_WEB                  200
#define TIMEOUT_COMMAND              200

// task manager
#define TASKS_MAX                    40      // up to 40 tasks
#define TASKS_SKIP_MISSED

// NV -------------------------------------------------------------------------------------------------------------------

// unique volume signature for this volume/partition layout
#define NV_VOLUME_SIGNATURE         0x0001u

// -----------------------------------------------------------------------------------
// Locales
#pragma once

// see Strings_xx.h for individual locale translations

// languages (one unique numeric code required for each)
#define L_ca 1 // Catalan
#define L_cn 2 // Chinese
#define L_de 3 // German
#define L_en 4 // English
#define L_es 5 // Spanish
#define L_fr 6 // French
#define L_it 7 // Italian
#define L_jp 8 // Japanese
#define L_us 9 // English (US, imperial units)
#define L_ro 10 // Romanian

// -----------------------------------------------------------------------------------
// Locale
#pragma once




// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_ca
#include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ca.h"
#define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_cn
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_cn.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_de
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_es
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_es.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_fr
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_fr.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_it
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_it.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_jp
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_jp.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_en.h"
  #define UNITS IMPERIAL
#endif
#if DISPLAY_LANGUAGE == L_ro
  #include "E:/GitHub/SmartWebServer/legacy/SmartWebServer/src/locales/Strings_ro.h"
  #define UNITS METRIC
#endif

// 未提供专用停止文字时沿用当前语言的停止描述
#ifndef BUTTON_STOP
  #define BUTTON_STOP L_STOP
#endif

// 未提供本地翻译时保留通用同步文字
#ifndef L_COORD_SYNC
  #define L_COORD_SYNC "Sync"
#endif

// misc. locale support functions
#ifndef DISPLAY_UNITS
  #define DISPLAY_UNITS UNITS
#else
  #if DISPLAY_UNITS == LOCALE_DEFAULT
    #undef DISPLAY_UNITS
    #define DISPLAY_UNITS UNITS
  #endif
#endif

#if DISPLAY_UNITS == IMPERIAL
  #define nativeToCelsius(t) ((t - 32.0F)*(5.0F/9.0F))
  #define celsiusToNative(t) (t*(9.0F/5.0F) + 32.0F)
  #define nativeToCelsiusRelative(t) (t*(5.0F/9.0F))
  #define celsiusToNativeRelative(t) (t*(9.0F/5.0F))
  #define TEMPERATURE_UNITS_ABV "F"
#else
  #define nativeToCelsius(t) (t)
  #define celsiusToNative(t) (t)
  #define nativeToCelsiusRelative(t) (t)
  #define celsiusToNativeRelative(t) (t)
  #define TEMPERATURE_UNITS_ABV "C"
#endif

// 其他语言未提供新标签时使用默认文字
#ifndef L_HOME_ACTIONS
#define L_HOME_ACTIONS "Home"
#endif
#ifndef L_PARK_ACTIONS
#define L_PARK_ACTIONS "Parking"
#endif
#ifndef L_COORD_GOTO
#define L_COORD_GOTO "Coordinate GOTO"
#endif
#ifndef L_COORD_SYSTEM
#define L_COORD_SYSTEM "Coordinate system"
#endif
#ifndef L_COORD_JNOW
#define L_COORD_JNOW "RA / Dec (JNOW)"
#endif
#ifndef L_COORD_RA
#define L_COORD_RA "RA"
#endif
#ifndef L_COORD_DEC
#define L_COORD_DEC "DEC"
#endif
#ifndef L_COORD_SIGN
#define L_COORD_SIGN "Sign"
#endif
#ifndef L_COORD_H
#define L_COORD_H "h"
#endif
#ifndef L_COORD_D
#define L_COORD_D "deg"
#endif
#ifndef L_COORD_M
#define L_COORD_M "min"
#endif
#ifndef L_COORD_S
#define L_COORD_S "sec"
#endif
#ifndef L_COORD_SEND
#define L_COORD_SEND "Sending coordinates"
#endif
#ifndef L_COORD_OK
#define L_COORD_OK "Command accepted"
#endif
#ifndef L_COORD_REJECT
#define L_COORD_REJECT "Command rejected, check device status"
#endif
#ifndef L_COORD_INVALID
#define L_COORD_INVALID "Enter valid coordinates; minutes and seconds must be zero at 90 degrees"
#endif
#ifndef L_COORD_NETWORK
#define L_COORD_NETWORK "Connection lost, check device before retrying"
#endif
#ifndef L_CAT_PREV
#define L_CAT_PREV "Previous"
#endif
#ifndef L_CAT_NEXT
#define L_CAT_NEXT "Next"
#endif
#ifndef L_CAT_TARGET
#define L_CAT_TARGET "Set target"
#endif
#ifndef L_CAT_HELP
#define L_CAT_HELP "Catalog help"
#endif

#ifndef L_COORD_AZ
#define L_COORD_AZ "Azimuth"
#endif
#ifndef L_COORD_ALT
#define L_COORD_ALT "Altitude"
#endif
#ifndef L_GOTO_ACTION
#define L_GOTO_ACTION "GOTO"
#endif

#ifndef L_GOTO_TARGET
#define L_GOTO_TARGET "Go to current target"
#endif
#ifndef L_SYNC_TARGET
#define L_SYNC_TARGET "Sync current target"
#endif

#ifndef L_TRK_AXES
#define L_TRK_AXES "Compensation axes"
#endif

#ifndef L_DEVICE_TRACKING
#define L_DEVICE_TRACKING "Device tracking"
#endif

#ifndef L_SIDEBAR_NOTE
#define L_SIDEBAR_NOTE "OnStep smart control"
#endif




// 统一配色。保留原 SWS COLOR_* 覆盖方式，样式和实时刷新共用这些值。
// 默认调色板使用暗红与灰黑，实时状态刷新也沿用同一主题
#ifndef COLOR_BODY_BACKGROUND
#define COLOR_BODY_BACKGROUND "#101010"
#endif

#ifndef COLOR_FOREGROUND
#define COLOR_FOREGROUND "#ababab"
#endif
#ifndef COLOR_LIGHT_FOREGROUND
#define COLOR_LIGHT_FOREGROUND "#c1c1c1"
#endif
#ifndef COLOR_ALERT
#define COLOR_ALERT "#d6b786"
#endif
#ifndef COLOR_BACKGROUND
#define COLOR_BACKGROUND "#111111"
#endif
#ifndef COLOR_LIGHT_BACKGROUND
#define COLOR_LIGHT_BACKGROUND "#222222"
#endif
#ifndef COLOR_BORDER
#define COLOR_BORDER "#383838"
#endif

#ifndef COLOR_LINK
#define COLOR_LINK "#7a7a7a"
#endif
#ifndef COLOR_LINK_BACKGROUND
#define COLOR_LINK_BACKGROUND "#252525"
#endif
#ifndef COLOR_LINK_SELECTED_BACKGROUND
#define COLOR_LINK_SELECTED_BACKGROUND "#474b51"
#endif
#ifndef COLOR_LINK_BORDER
#define COLOR_LINK_BORDER "#636363"
#endif

#ifndef COLOR_CONTROL
#define COLOR_CONTROL "#761b1b"
#endif
#ifndef COLOR_CONTROL_BACKGROUND
#define COLOR_CONTROL_BACKGROUND "#181818"
#endif
#ifndef COLOR_CONTROL_DISABLED
#define COLOR_CONTROL_DISABLED "#7e7e7e"
#endif
#ifndef COLOR_CONTROL_DISABLED_BACKGROUND
#define COLOR_CONTROL_DISABLED_BACKGROUND "#292929"
#endif

#ifndef COLOR_COLLAPSIBLE
#define COLOR_COLLAPSIBLE "#999"
#endif
#ifndef COLOR_COLLAPSIBLE_BACKGROUND
#define COLOR_COLLAPSIBLE_BACKGROUND "#252525"
#endif
#ifndef COLOR_COLLAPSIBLE_ACTIVE_BACKGROUND
#define COLOR_COLLAPSIBLE_ACTIVE_BACKGROUND "#303030"
#endif
#ifndef COLOR_COLLAPSIBLE_CONTENT_BACKGROUND
#define COLOR_COLLAPSIBLE_CONTENT_BACKGROUND "#1c1c1c"
#endif

#ifndef COLOR_SERVO_BACKGROUND_1
#define COLOR_SERVO_BACKGROUND_1 "#222222"
#endif
#ifndef COLOR_SERVO_BACKGROUND_2
#define COLOR_SERVO_BACKGROUND_2 "#4c1b1b"
#endif
#ifndef COLOR_SERVO_BACKGROUND_3
#define COLOR_SERVO_BACKGROUND_3 "#8f2222"
#endif
#ifndef COLOR_SERVO_PEN_1
#define COLOR_SERVO_PEN_1 "#303030"
#endif
#ifndef COLOR_SERVO_PEN_2
#define COLOR_SERVO_PEN_2 "#444444"
#endif
#ifndef COLOR_SERVO_PEN_3
#define COLOR_SERVO_PEN_3 "#666666"
#endif
#ifndef COLOR_SERVO_PEN_4
#define COLOR_SERVO_PEN_4 "#cccccc"
#endif

#ifndef SWS_COLOR_PANEL
#define SWS_COLOR_PANEL "#191919"
#endif
#ifndef SWS_COLOR_TEXT
#define SWS_COLOR_TEXT "#d4d4d4"
#endif
#ifndef SWS_COLOR_MUTED
#define SWS_COLOR_MUTED "#a1a1a1"
#endif
#ifndef SWS_COLOR_READOUT
#define SWS_COLOR_READOUT SWS_COLOR_TEXT
#endif



// 编辑卡片共用内嵌图标，不依赖字体或外部资源。
#define SWS_EDIT_ICON "<svg viewBox='0 0 24 24' aria-hidden='true'><path d='m16 3 5 5-12 12-6 1 1-6Zm-2 2 5 5' fill='currentColor'/></svg>"

// 语言仍由 DISPLAY_LANGUAGE / Locale.h 在编译时选择。
// 将 C++ 字符串转为已转义的 JavaScript 字符串，避免引号破坏脚本。
#define SWS_JS_TEXT_INNER(text) #text
#define SWS_JS_TEXT(text) SWS_JS_TEXT_INNER(text)

// 文案放进 HTML 属性，由事件读取；语言里的单引号不会进入 JavaScript。
#define SWS_CONFIRM_ATTRIBUTE " data-confirm=\"" L_ARE_YOU_SURE "?\" "



// 页面分区模板。设备能力条件、表单提交与通信仍由原页面处理。
// 这里只保存 HTML；语言宏由原始 Locale.h 在编译时展开。
const char html_layout_controller_begin[] PROGMEM =
  "<div class='sws-controller'><section><div class='sws-section-head'><h2 class='sws-section-title'>" L_CONTROLLER " " L_STATUS "</h2><p></p></div><div class='sws-overview-grid'>";

const char html_layout_controller_axes[] PROGMEM =
  "</div></section><section><div class='sws-section-head'><h2 class='sws-section-title'>" L_DRIVER "</h2><p></p></div><div class='sws-axis-grid'>";

const char html_layout_controller_axes_end[] PROGMEM =
  "</div></section>";

const char html_layout_controller_advanced[] PROGMEM =
  "<details class='sws-more' open><summary>" L_ADV_SET_TITLE "</summary>";

const char html_layout_controller_advanced_end[] PROGMEM =
  "</details>";

const char html_layout_controller_end[] PROGMEM =
  "</div>";

const char html_layout_mount_begin[] PROGMEM =
  "<div class='sws-mount'><div class='sws-control-grid'><section class='sws-guide'>";

const char html_layout_mount_target[] PROGMEM =
  "</div><section class='sws-target'>";

const char html_layout_mount_tracking[] PROGMEM =
  "</section><section class='sws-tracking'>";

const char html_layout_mount_location[] PROGMEM =
  "</section></div><div class='sws-site-limits'>";

const char html_layout_mount_home[] PROGMEM =
  "</section><section class='sws-home'>";

const char html_layout_mount_library[] PROGMEM =
  "</section><section class='sws-library-section'>";

const char html_layout_mount_advanced[] PROGMEM =
  "</section><details class='sws-more' open><summary>" L_ALIGN " / " L_PAGE_PEC " / " L_TRACKING_SETTINGS "</summary><div class='sws-settings-grid sws-advanced-grid'>";

const char html_layout_mount_end[] PROGMEM =
  "</div></details></div>";

const char html_layout_focuser_begin[] PROGMEM =
  "<div class='sws-device-layout sws-accessory-layout'><section class='sws-device-selector'>";

const char html_layout_focuser_motion[] PROGMEM =
  "</section><div class='sws-accessory-grid'><div class='sws-accessory-main'><section class='sws-device-primary sws-accessory-motion'>";

const char html_layout_accessory_home[] PROGMEM =
  "</section><section class='sws-accessory-home'>";

const char html_layout_accessory_settings[] PROGMEM =
  "</section></div><section class='sws-accessory-settings'>";

const char html_layout_accessory_end[] PROGMEM =
  "</section></div></div>";

const char html_layout_rotator_begin[] PROGMEM =
  "<div class='sws-device-layout sws-accessory-layout'><div class='sws-accessory-grid'><div class='sws-accessory-main'><section class='sws-device-primary sws-accessory-motion'>";

const char html_layout_encoders_begin[] PROGMEM =
  "<div class='sws-device-layout sws-encoder-layout'><section class='sws-encoder-sync-section'>";

const char html_layout_encoders_settings[] PROGMEM =
  "</section><section><div class='sws-section-head'><h2 class='sws-section-title'>" L_ENC_CONF "</h2></div><div class='sws-encoder-grid'>";

const char html_layout_encoders_end[] PROGMEM =
  "</div></section></div>";

const char html_layout_network_begin[] PROGMEM =
  "<div class=\"sws-network-layout\">";

const char html_layout_network_account[] PROGMEM =
  "<section class='sws-account'>";

const char html_layout_network_account_end[] PROGMEM =
  "</section>";






const char html_head_begin[] PROGMEM = "<!DOCTYPE HTML>\n"
                                  "<html>\n"
                                  "<head>\n"
                                  "<meta charset='utf-8'/>\n"
                                  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n";
const char html_headerPec[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; URL=/pec.htm\">\n";
const char html_headerIdx[] PROGMEM = "<meta http-equiv=\"refresh\" content=\"5; URL=/index.htm\">\n";
const char html_head_end[] PROGMEM = "</head>\n";
const char html_body_begin[] PROGMEM = "<body bgcolor='" COLOR_BODY_BACKGROUND "'>\n";
const char html_page_and_body_end[] PROGMEM = "</div></body></html>\n";

const char html_main_css_begin[] PROGMEM = 
"<STYLE>";

const char html_main_css_core[] PROGMEM =
".clear { clear: both; } .a { background-color: " COLOR_BACKGROUND "; } "
".t { padding: 10px 10px 20px 10px; border: 5px solid " COLOR_BORDER "; margin: 25px 25px 0px 25px;"
"color: " COLOR_FOREGROUND "; background-color: " COLOR_BACKGROUND "; min-width: 36em; } "
"input { text-align:center; padding: 2px; margin: 3px; font-weight: bold; width:5em; background-color: " COLOR_CONTROL "}"
".b { padding: 10px; border-left: 5px solid " COLOR_BORDER "; border-right: 5px solid " COLOR_BORDER "; border-bottom: 5px solid " COLOR_BORDER "; margin: 0px 25px 25px 25px; color: " COLOR_FOREGROUND ";"
"background-color: " COLOR_BACKGROUND "; min-width: 36em; } select { width:4em; font-weight: bold; background-color: " COLOR_CONTROL "; padding: 2px 2px; }"
".c { color: " COLOR_CONTROL "; font-weight: bold; }"
"h1 { text-align: right; } a:hover, a:active { background-color: " COLOR_LINK_BORDER "; } .y { color: " COLOR_ALERT "; font-weight: bold; }"
"a:link, a:visited { background-color: " COLOR_LINK_BACKGROUND "; color: " COLOR_LINK "; border:1px solid " COLOR_LINK_BORDER "; padding: 5px 10px; margin: none; text-align: center; text-decoration: none; display: inline-block; }"
"button:disabled { background-color: " COLOR_CONTROL_DISABLED_BACKGROUND "; color: " COLOR_CONTROL_DISABLED "; } button { background-color: " COLOR_CONTROL "; font-weight: bold; border-radius: 5px; margin: 2px; padding: 4px 8px; }";

const char html_main_css_no_select[] PROGMEM =
"body { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; }";

const char html_main_css_buttons[] PROGMEM = 
".btns_left { margin-left: -1px; border-top-left-radius: 0px; border-bottom-left-radius: 0px; }"
".btns_mid { margin-left: -1px; margin-right: -1px; border-top-left-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 0px; border-bottom-right-radius: 0px; }"
".btns_right { margin-right: -1px; border-top-right-radius: 0px; border-bottom-right-radius: 0px; }";

const char html_main_css_control[] PROGMEM = 
".b1 { float: left; border: 2px solid " COLOR_BORDER "; background-color: " COLOR_CONTROL_BACKGROUND "; text-align: center; margin: 5px; padding: 15px; padding-top: 3px; }"
".gb {  font-weight: bold; font-size: 150%; font-family: 'Times New Roman', Times, serif; width: 60px; height: 50px; padding: 0px; touch-action: none; -webkit-touch-callout: none; }"
".bb {  font-weight: bold; font-size: 105%; } .bbh {  font-weight: bold; font-size: 100%; height: 2.1em; touch-action: none; -webkit-touch-callout: none; }";

const char html_main_css_collapse[] PROGMEM =
".collapsible { background-color: " COLOR_COLLAPSIBLE_BACKGROUND "; color: " COLOR_COLLAPSIBLE "; cursor: pointer; padding: 7px; width: 80%; border: none; text-align: left; outline: none; font-size: 14px; }"
".active, .collapsible:hover { background-color: " COLOR_COLLAPSIBLE_ACTIVE_BACKGROUND "; } .content { padding: 2px 18px; display: none; overflow: hidden; background-color: " COLOR_COLLAPSIBLE_CONTENT_BACKGROUND "; }";

const char html_main_css_end[] PROGMEM = 
"</STYLE>";

// tile: begin takes title and width
const char html_tile_beg[] PROGMEM = "<div class='b1' style='width: %s; min-height: %s;'><div style='float: left;'>%s:</div>\n";
// tile: begin for text takes title and width
const char html_tile_text_beg[] PROGMEM = "<div class='b1' style='text-align: left; width: %s; min-height: %s;'><div style='float: left;'>%s:</div>\n";
// tile: adds status to first line
const char html_tile_status[] PROGMEM = "<div style='float: right; text-align: right;' id='%s' class='c'>%s</div><br />\n";
// tile: end
const char html_tile_end[] PROGMEM = "</div>\n";

// collapsable: begin takes title
const char html_collapsable_beg[] PROGMEM = 
"<div style='text-align: left;'> <button type='button' class='collapsible' style='vertical-align: bottom'>%s</button>"
"<div class='content' style='text-align: left;'><br />\n";
// collapsable: end
const char html_collapsable_end[] PROGMEM = "<br /></div></div>\n";

// form: begin takes webpage for get
const char html_form_begin[] PROGMEM = "<form method='get' action='/%s'>\n";
// form: end
const char html_form_end[] PROGMEM = "</form>\n";

// input: medium width numeric takes name (for get,) value, min, max, unit
const char html_ninput_wide[] PROGMEM =
"<input style='width: 4em;' type='number' name='%s' value='%s' min='%s' max='%s'>&nbsp;%s";
// input: narrow width numeric takes name (for get,) value, min, max, unit
const char html_ninput[] PROGMEM =
"<input style='width: 3.5em;' type='number' name='%s' value='%s' min='%s' max='%s'>&nbsp;%s";




// Javascript for Ajax return
const char html_script_ajax_get[] PROGMEM =
"<script>\n"
"function s(key,v1) {"
  "var x=new XMLHttpRequest();"
  "if(window.swsWatchCommand)swsWatchCommand(x);"
  "x.open('GET','%s?'+key+'='+encodeURIComponent(v1)+'&x='+Date.now(),true);"
  "x.send();"
"}</script>\n";

const char html_script_ajax_shortcuts[] PROGMEM =
"<script>\n"
"function g(v1){s('dr',v1);}"
"function gf(v1){s('dr',v1);autoFastRun();}"
"function sf(key,v1){s(key,v1);autoFastRun();}"
"let lzh; function sz(key,v1){clearTimeout(lzh);lzh=setTimeout(s," STR(AJAX_PAGE_LAZY_GET_MS) ",key,v1);}\n"
"</script>\n";

// Javascript for hold-to-run motion controls
const char html_script_motion_controls_1[] PROGMEM =
"<script>"
"function motionControls(o){"
"var m={},p={},q=[],b=0,g=0;"
"function send(v){q.push(v);run();}"
"function run(){if(b||!q.length)return;b=1;window.swsPendingCommands=(window.swsPendingCommands||0)+1;var v=q.shift(),x=new XMLHttpRequest(),d=0;function f(){if(d)return;d=1;window.swsPendingCommands--;b=0;run();}x.onreadystatechange=function(){if(x.readyState==4)f();};x.onerror=x.ontimeout=f;x.timeout=1000;x.open('GET',o.p+'?'+o.k+'='+encodeURIComponent(v)+'&x='+new Date().getTime(),true);x.send();}"
"function active(){for(var k in m)if(m[k])return true;return false;}"
"function stopValue(a){return typeof o.s=='function'?o.s(a):o.s;}"
"function allStopValue(){return typeof o.a=='function'?o.a():o.a;}"
"function haltValue(){return typeof o.h=='function'?o.h():o.h||allStopValue()||stopValue('');}"
"function repeat(a,h){return h==g&&(o.g?!active():!m[a]);}"
"function stop(a){if(!m[a])return;m[a]=0;var h=++g,v=stopValue(a);send(v);setTimeout(function(){if(repeat(a,h))send(v)},150);setTimeout(function(){if(repeat(a,h))send(v)},500);}"
"function clearActive(){var r=0;for(var k in m){if(m[k])r=1;m[k]=0;}p={};return r;}";

// Pointer Events 统一处理触摸与鼠标；旧浏览器保留 Touch 回退。
const char html_script_motion_controls_2[] PROGMEM =
"function repeatSend(v){var h=g;send(v);setTimeout(function(){if(h==g&&!active())send(v)},150);setTimeout(function(){if(h==g&&!active())send(v)},500);}"
"function stopAll(){var v=allStopValue();if(v){if(clearActive()){g++;repeatSend(v);}return;}for(var k in m)stop(k);}"
"function halt(){clearActive();g++;repeatSend(haltValue());}"
"function legacyTouch(e){return e&&e.type&&e.type.indexOf('touch')===0&&window.PointerEvent;}"
"function down(e,v,a){if(legacyTouch(e))return;if(e&&e.preventDefault)e.preventDefault();a=a||v;if(m[a])return;m[a]=1;g++;send(v);}"
"function up(e,a){if(legacyTouch(e))return;if(e&&e.preventDefault)e.preventDefault();stop(a);}"
"function pointerDown(e,v,a){if(e.button!==undefined&&e.button!==0)return;a=a||v;if(e.pointerId!==undefined)p[e.pointerId]=a;down(e,v,a);if(e.currentTarget&&e.currentTarget.setPointerCapture&&e.pointerId!==undefined){try{e.currentTarget.setPointerCapture(e.pointerId);}catch(_){}}}"
"function pointerUp(e,a){if(e.pointerId!==undefined){a=p[e.pointerId];if(!a)return;delete p[e.pointerId];for(var id in p)if(p[id]===a)return;}up(e,a);}"
"function windowUp(e){if(e&&e.pointerId!==undefined){pointerUp(e);return;}if(legacyTouch(e))return;stopAll();}"
"function cancel(e){if(e&&e.preventDefault)e.preventDefault();return false;}"
"window.addEventListener('pointerup',windowUp);"
"window.addEventListener('pointercancel',windowUp);"
"window.addEventListener('lostpointercapture',windowUp);"
"window.addEventListener('touchend',windowUp);"
"window.addEventListener('touchcancel',windowUp);"
"window.addEventListener('blur',stopAll);"
// 标签切换前通知运动控制释放按键，等待停止请求发送
"window.swsMotionStops=window.swsMotionStops||[];window.swsMotionStops.push(function(){var moving=active()||b||q.length>0;stopAll();if(!moving)return null;return new Promise(function(resolve){setTimeout(function wait(){if(b||q.length){setTimeout(wait,50);return;}resolve();},650);});});"
"document.addEventListener('visibilitychange',function(){if(document.hidden)stopAll();});"
"return{send:send,stop:stop,stopAll:stopAll,halt:halt,t:down,u:up,p:pointerDown,r:pointerUp,c:cancel};"
"}"
"</script>\n";

// Javascript for Date/Time return
const char html_script_ajax_date_time_return[] PROGMEM =
"<script>\n"
"function SetDateTime() {"
"var d1 = new Date();"
"var jan = new Date(d1.getFullYear(), 0, 1);"
"var d = new Date(d1.getTime()-(jan.getTimezoneOffset()-d1.getTimezoneOffset())*60*1000);"
"var f=document.getElementById('sws-site-form');"
"if(f){var h=f.elements.u1.defaultValue,m=Array.from(f.elements.u2.options).find(o=>o.defaultSelected)?.value||'0';var w=(Math.abs(Number(h))*60+Number(m))*(h.startsWith('-')?-1:1);"
"d=new Date(d1.getTime()-w*60000);}"
"document.getElementById('dd').value = (f?d.getUTCDate():d.getDate());"
"document.getElementById('dm').value = (f?d.getUTCMonth():d.getMonth());"
"document.getElementById('dy').value = (f?d.getUTCFullYear():d.getFullYear());"
"document.getElementById('th').value = (f?d.getUTCHours():d.getHours());"
"document.getElementById('tm').value = (f?d.getUTCMinutes():d.getMinutes());"
"document.getElementById('ts').value = (f?d.getUTCSeconds():d.getSeconds());"
"}\n"
"</script>\n";

// Javascript for Collapsibles
const char html_script_collapsible[] PROGMEM =
"<script>"
"var cc = document.getElementsByClassName('collapsible');"
"var i;"
"for (i = 0; i < cc.length; i++) {"
  "cc[i].addEventListener('click', function() {"
    "this.classList.toggle('active');"
    "var ct = this.nextElementSibling;"
    "if (ct.style.display === 'block') { ct.style.display = 'none'; } else { ct.style.display = 'block'; }"
  "});"
"}"
"</script>\n";

// Javascript for Ajax active refresh of controls
// be sure to define "var ajaxPage='control.txt';" etc.
const char html_script_ajax[] PROGMEM =
"<script>\n"
"function pad(num, size) { var s = '000000000' + num; return s.substr(s.length-size); }\n"
"function update_date_time() {\n"
  "var now = new Date();\n"
  "var element=document.getElementById('datetime'); if (!element) return;\n"
  "var value = \n"
  "(now.getUTCMonth()+1)+'/'+pad(now.getUTCDate().toString(),2)+'/'+pad(now.getUTCFullYear().toString().substr(-2),2)+' '+\n"
  "pad(now.getUTCHours().toString(),2)+':'+pad(now.getUTCMinutes().toString(),2)+':'+pad(now.getUTCSeconds().toString(),2);\n"
  "if (element.textContent!==value) element.textContent=value;\n"
"}\n"
R"JS(
// 按请求完成时间安排下一次刷新，慢连接只保留一个状态请求。
var autoTimer=null, autoRequest=null, autoDeadline=null, autoStarted=0, autoFastUntil=0, autoFailures=0, autoStopped=false;
var autoHtmlValues=new WeakMap();
function autoRate() {
  return Date.now()<autoFastUntil ? )JS" STR(AJAX_PAGE_UPDATE_RATE_FAST_MS) R"JS( : )JS" STR(AJAX_PAGE_UPDATE_RATE_MS) R"JS(;
}
function autoSchedule(delay) {
  clearTimeout(autoTimer);
  if (!autoStopped && !document.hidden) autoTimer=setTimeout(autoRun,delay);
}
function autoFastRun() {
  autoFastUntil=Date.now()+)JS" STR(AJAX_PAGE_UPDATE_FAST_SHED_MS) R"JS(;
  if (!autoRequest) autoSchedule(autoRate());
}
function autoPause() {
  clearTimeout(autoTimer);
  clearTimeout(autoDeadline);
  if (autoRequest) {
    var request=autoRequest;
    autoRequest=null;
    request.abort();
  }
}
window.swsPausePolling=function() { autoStopped=true; autoPause(); };
window.swsResumePolling=function() { autoStopped=false; autoFailures=0; autoPause(); autoRun(); };
function autoRun() {
  if (autoStopped || document.hidden || autoRequest) return;
  // 命令正在发送时让出下一次刷新机会，不改变命令的参数或发送顺序。
  if (window.swsPendingCommands) { autoSchedule(autoRate()); return; }
  var request=new XMLHttpRequest();
  autoRequest=request;
  autoStarted=Date.now();
  function finish() {
    if (autoRequest!==request) return;
    clearTimeout(autoDeadline);
    autoRequest=null;
    if (request.status===200) {
      try { pageReady(ajaxPage).call(request); autoFailures=0; }
      catch (error) { autoFailures=Math.min(autoFailures+1,6); }
    } else autoFailures=Math.min(autoFailures+1,6);
    autoSchedule(autoFailures ? Math.min(30000,autoRate()*Math.pow(2,autoFailures)) : autoRate());
  }
  request.onreadystatechange=function() { if (request.readyState===4) finish(); };
  request.onerror=request.ontimeout=finish;
  request.timeout=5000;
  // 独立截止时间防止浏览器漏发完成事件，终止旧请求后才安排下一次刷新。
  autoDeadline=setTimeout(function() { if(autoRequest!==request)return; autoRequest=null; request.abort(); autoFailures=Math.min(autoFailures+1,6); autoSchedule(autoRate()); },5500);
  request.open('GET',ajaxPage.toLowerCase()+'?nocache='+Date.now(),true);
  request.send(null);
}
function pageReady(aPage) {
  return function() {
    if (this.readyState!==4 || this.status!==200) return;
    var lines=this.responseText.split('\n');
    for (var i=0;i<lines.length;i++) {
      var j=lines[i].indexOf('|'), valueMode=false;
      if (j===-1) { j=lines[i].indexOf('&'); valueMode=true; }
      if (j<1) continue;
      var key=lines[i].slice(0,j), value=lines[i].slice(j+1);
      if (key==='call' && value==='update_date_time') { update_date_time(); continue; }
      var element=document.getElementById(key);
      if (!element) continue;
      // 状态没有变化时不触发重排、重绘和控件观察器。
      if (valueMode) {
        if (element.value!==value) element.value=value;
      } else if (value==='selected' || value==='unselected') {
        var selected=String(value==='selected');
        if (element.dataset.selected!==selected) element.dataset.selected=selected;
      } else if (value==='disabled' || value==='enabled') {
        var disabled=value==='disabled';
        if (element.disabled!==disabled) element.disabled=disabled;
      } else if (autoHtmlValues.get(element)!==value) {
        if (element.innerHTML!==value) element.innerHTML=value;
        autoHtmlValues.set(element,value);
      }
    }
  };
}
// 页面完全加载后再轮询，避免状态请求堵在尚未传完的页面后面。
window.addEventListener('load',autoRun);
document.addEventListener('visibilitychange',function() {
  if (document.hidden) autoPause();
  else { autoFailures=0; autoRun(); }
});
window.addEventListener('pagehide',window.swsPausePolling);
window.addEventListener('pageshow',function() { autoStopped=false; autoRun(); });
function autoWake() {
  if(autoStopped || document.hidden)return;
  autoFailures=0;
  if(autoRequest && Date.now()-autoStarted>5000)autoPause();
  if(!autoRequest)autoRun();
}
window.addEventListener('online',autoWake);
window.addEventListener('focus',autoWake);
)JS" "</script>\n";

// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Date/Time/Location tile





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

// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Library tile





extern void libraryTile(String &data);
extern void libraryTileAjax(String &data);
extern void libraryTileGet();

const char html_libCatalogSelect[] PROGMEM =
"<label class='sws-catalog-select'>" L_CATALOG " <select name='selBox' onchange=\"busy(); s('lib_index',this.value);\">"
"<option value='0' selected>-</option>"
"<option value='1' >1</option>"
"<option value='2' >2</option>"
"<option value='3' >3</option>"
"<option value='4' >4</option>"
"<option value='5' >5</option>"
"<option value='6' >6</option>"
"<option value='7' >7</option>"
"<option value='8' >8</option>"
"<option value='9' >9</option>"
"<option value='10' >10</option>"
"<option value='11' >11</option>"
"<option value='12' >12</option>"
"<option value='13' >13</option>"
"<option value='14' >14</option>"
"<option value='15' >15</option>"
"</select></label>\n";

const char html_libShowMessage[] PROGMEM =
"<div id='lib_message' style='margin: 0 auto; width: 14em; margin-top: 0.5em; margin-bottom: 1em; background-color: " COLOR_LIGHT_BACKGROUND "; color: " COLOR_LIGHT_FOREGROUND "; border: 1px solid " COLOR_BORDER "; padding: 2px;'>" L_CAT_NO_CAT "</div>\n";

const char html_catPrevRec[] PROGMEM =
"&nbsp;<button id='cat_prev' type='button' onclick=\"s('lib','cat_prev');\" disabled>" L_CAT_PREV "</button>\n";

const char html_catNextRec[] PROGMEM =
"&nbsp;<button id='cat_next' type='button' onclick=\"s('lib','cat_next');\" disabled>" L_CAT_NEXT "</button>\n";

const char html_catSetTarget[] PROGMEM =
"&nbsp;<button id='cat_select' type='button' onclick=\"s('lib','cat_select');\" disabled>" L_CAT_TARGET "</button>\n";

const char html_catShowMessage[] PROGMEM =
"<div id='cat_message' style='margin: 0 auto; width: 14em; margin-top: 0.5em; margin-bottom: 1em; background-color: " COLOR_LIGHT_BACKGROUND "; color: " COLOR_LIGHT_FOREGROUND "; border: 1px solid " COLOR_BORDER "; padding: 2px;'>" L_CAT_NO_OBJECT "</div>\n";

const char html_libUploadCatalog[] PROGMEM =
"&nbsp;&nbsp;<button id='cat_upload' type='button' onclick=\"busy(); t('cat_upload',document.getElementById('cat_data').value);\" disabled>" L_UPLOAD "</button>\n";

const char html_libDownloadCatalog[] PROGMEM =
"&nbsp;<button id='cat_download' type='button' onclick=\"busy(); s('lib','cat_download');\" disabled>" L_DOWNLOAD "</button>\n";

const char html_libClearCatalog[] PROGMEM =
"&nbsp;<button id='lib_clear' type='button' " SWS_CONFIRM_ATTRIBUTE " onclick=\"busy(); if (confirm(this.dataset.confirm)) s('lib','clear')\">" L_CAT_CLEAR_LIB "</button>\n";

const char html_libEditCatalog[] PROGMEM =
"<label for='cat_data' class='sws-editor-label'>" L_CAT_DATA "</label>"
"<textarea spellcheck='false' id='cat_data' style='background-color: " COLOR_LIGHT_BACKGROUND "; color: #ffffff; border: 1px solid " COLOR_BORDER ";' "
"rows='10' cols='40' oninput=\"document.getElementById('cat_upload').disabled=false;\"></textarea><br /><br />\n";

const char html_libCatalogHelp[] PROGMEM =
"<br /><a href='libraryHelp.htm'>" L_CAT_HELP "</a><br />";

// Javascript for library status
const char html_script_ajax_library[] PROGMEM =
"<script>\n"
"function t(key,v) { if (v.length==0) v='DELETE'; s(key,v.replace(/ /g, '_')); }\n"
"function busy() {\n"
  "document.getElementById('lib_message').innerHTML='Working...';"
  "document.getElementById('cat_upload').disabled=true;"
  "document.getElementById('cat_download').disabled=true;"
"}\n"
"</script>\n";

// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Home/Park tile





extern void homeParkTile(String &data);
extern void homeParkTileAjax(String &data);
extern void homeParkTileGet();

// Homing
const char html_homeReturn[] PROGMEM =
"<button onpointerdown=\"g('qh')\" type='button'>" L_RETURN_HOME "</button>";
const char html_homeReset[] PROGMEM =
"<button class='sws-secondary'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) g('qr')\" type='button'>" L_RESET_HOME "</button>";

// Parking
const char html_unpark[] PROGMEM =
"<button hidden id='unpark' onpointerdown=\"s('pk','u')\" type='button' disabled>" L_UNPARK "</button>";
const char html_park[] PROGMEM =
"<button id='park' onpointerdown=\"s('pk','p')\" type='button' disabled>" L_PARK "</button>";
const char html_setpark[] PROGMEM =
"<button class='sws-secondary' title='" L_SET_PARK_CURRENT_COORDS "' id='setpark' onpointerdown=\"s('pk','s')\" type='button'>" L_SET_PARK "</button><br />";

// Home automatically
const char html_homeAuto[] PROGMEM = 
"<button id='auto_on' class='btns_right' onpointerdown=\"s('ha','1')\" type='button'>" L_ON "</button>"
"<button id='auto_off' class='btns_left' onpointerdown=\"s('ha','0')\" type='button'>" L_OFF "</button><br />\n";

// Home offset
const char html_homeOffsetAxis1[] PROGMEM =
  "<label class='sws-number-field'><span>" L_HOME_OFFSET_RANGE_AXIS1 "</span><input style='width:6em;' value='%ld' type='number' name='hc1' min='-" STR(HOME_OFFSET_RANGE_AXIS1) "' max='" STR(HOME_OFFSET_RANGE_AXIS1) "'></label>\n";
const char html_homeOffsetAxis2[] PROGMEM =
  "<label class='sws-number-field'><span>" L_HOME_OFFSET_RANGE_AXIS2 "</span><input style='width:6em;' value='%ld' type='number' name='hc2' min='-" STR(HOME_OFFSET_RANGE_AXIS2) "' max='" STR(HOME_OFFSET_RANGE_AXIS2) "'></label>\n";

#ifdef HOME_SWITCH_DIRECTION_CONTROL
  const char html_homeReverse[] PROGMEM =
    "<button id='hrev1'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('hr','1')\" type='button'>Axis1 Reverse</button>"
    "<button id='hrev2'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('hr','2')\" type='button'>Axis2 Reverse</button><br />";
#endif

// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Align tile





extern void alignTile(String &data);
extern void alignTileAjax(String &data);
extern void alignTileGet();

const char html_alignCorrection[] PROGMEM =
  "<div class='sws-align-correction'><div><strong id='align_lr' class='c'>%s</strong><span>&amp;</span><strong id='align_ud' class='c'>%s</strong></div><span>" L_ALIGN_MESSAGE " %s</span></div>";

const char html_alignStart[] PROGMEM =
  "<button id='alg%d' class='bbh' onpointerdown=\"s('al','%d')\" type='button' disabled>%d%s</button>";

const char html_alignAccept[] PROGMEM =
  "&nbsp;&nbsp;&nbsp;<button id='alga' class='bbh' onpointerdown=\"s('al','n')\" type='button' disabled>" L_ACCEPT "</button>";

const char html_alignRefine[] PROGMEM =
  "<ol class='sws-refine-steps'><li>" L_REFINE_MESSAGE1 "</li><li>" L_REFINE_MESSAGE2 "</li><li>" L_REFINE_MESSAGE3 "</li></ol><div class='sws-refine-action'>"
  "<button id='rpa'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('rp','a')\" type='button'>" L_REFINE_PA "</button>"
  "</div>";

// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Limits and Backlash tile





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


// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// PEC tile





extern void pecTile(String &data);
extern void pecTileAjax(String &data);
extern void pecTileGet();

const char html_pecStatus[] PROGMEM = 
"<div class='sws-pec-status'><span>" L_PEC_STATUS "</span><strong id='pec_sta'>" L_PEC_UNK "</strong></div>";

const char html_pecControls1[] PROGMEM =
"<button onpointerdown=\"s('pec','play')\" type='submit'>" L_PEC_PLAY "</button>";

const char html_pecControls2[] PROGMEM =
"<button onpointerdown=\"s('pec','stop')\" type='submit'>" L_STOP "</button><br /><br />"
"<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('pec','clr')\" type='submit'>" L_PEC_CLEAR "</button>"
"<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('pec','rec')\" type='submit'>" L_PEC_REC "</button><br />";

const char html_pecControls3[] PROGMEM =
"<p class='sws-help'>" L_PEC_CLEAR_MESSAGE "</p>";

const char html_pecControls4[] PROGMEM =
"<div class='sws-pec-write'><button " SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('pec','wrt')\" type='submit'>" L_PEC_EEWRITE "</button><p class='sws-help'>" L_PEC_EEWRITE_MESSAGE "</p></div>";

const char html_configAxisSpwr[] PROGMEM =
L_PEC_STEPS_PER_WORM_ROTATION ":<br />"
"<input style='width: 7em;' value='%ld' type='number' name='spwr' min='%d' max='%ld' step='1'><br />\n";

// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Goto tile





extern void gotoTile(String &data);
extern void gotoTileAjax(String &data);
extern void gotoTileGet();

#define Axis1 "&alpha;"
#define Axis2 "&delta;"

// 当前坐标和目标坐标采用两列指标，保留原来的状态更新标识
const char html_mountPositionLabels[] PROGMEM = "<div class='sws-position-grid'>";
const char html_mountPositionAxis1[] PROGMEM =
"<div><span>" L_COORD_AZ "</span><strong id='gto_az1'>%s</strong></div>"
"<div><span>" L_CURRENT " RA</span><strong id='gto_i1'>%s</strong></div>"
"<div><span>" L_TARGET " RA</span><strong id='gto_t1'>%s</strong></div>";
const char html_mountPositionAxis2[] PROGMEM =
"<div><span>" L_COORD_ALT "</span><strong id='gto_az2'>%s</strong></div>"
"<div><span>" L_CURRENT " DEC</span><strong id='gto_i2'>%s</strong></div>"
"<div><span>" L_TARGET " DEC</span><strong id='gto_t2'>%s</strong></div></div>";

const char html_mountHorizon[] PROGMEM =
  "Z <span id='gto_az1' class='c'>%s</span>, A <span id='gto_az2' class='c'>%s</span><br />";

const char html_mountPosition[] PROGMEM =
  L_CURRENT ": " Axis1 " <span id='gto_i1' class='c'>%s</span>, " Axis2 " <span id='gto_i2' class='c'>%s</span><br />";

const char html_mountTarget[] PROGMEM =
  L_TARGET ": &nbsp;" Axis1 " <span id='gto_t1' class='c'>%s</span>, " Axis2 " <span id='gto_t2' class='c'>%s</span><br />";

const char html_gotoGo[] PROGMEM =
  "<br /><button onpointerdown=\"s('goto','go')\" type='button' class='bb'>" L_GOTO_TARGET "</button>";

const char html_gotoStop[] PROGMEM =
  "<button onpointerdown=\"s('goto','stop')\" type='button' class='bb' style='height: 3em; width: 5em; color: White;' title='" L_HINT_EMERGENCY_STOP "'>" L_STOP "!</button>";

// 同步操作移至定位动作区，继续使用原有同步命令
const char html_gotoSync[] PROGMEM = "<button class='sws-sync' onpointerdown=\"g('sy')\" type='button'>" L_SYNC_TARGET "</button>";

const char html_gotoContinue[] PROGMEM =
  "<button id='gto_active' onpointerdown=\"s('goto','mp_cnt')\" type='button' disabled class='bb' title='" L_HINT_CONTINUE_GOTO "'>" L_CONTINUE "</button>";

const char html_slewSpeed[] PROGMEM =
  L_MAX_SLEW_SPEED " <span id='gto_rate' class='c'>%s</span>:<br />";

const char html_slewSpeedSelect[] PROGMEM =
  "<button id='gto_rate_vf' class='btns_right' onpointerdown=\"s('goto','vf')\" type='button'>" L_VFAST "</button>"
  "<button id='gto_rate_f' class='btns_mid' onpointerdown=\"s('goto','f')\"  type='button'>" L_FAST "</button>"
  "<button id='gto_rate_n' class='btns_mid' onpointerdown=\"s('goto','n')\"  type='button'>" L_NORMAL "</button>"
  "<button id='gto_rate_s' class='btns_mid' onpointerdown=\"s('goto','s')\"  type='button'>" L_SLOW "</button>"
  "<button id='gto_rate_vs' class='btns_left' onpointerdown=\"s('goto','vs')\" type='button'>" L_VSLOW "</button>"
  "<br /><br />\n";

const char html_gotoBuzzer[] PROGMEM = 
  "<div class='sws-option-row'>"
  L_BUZZER ": <br />"
  "<button id='gto_bzr_on' class='btns_right' onpointerdown=\"s('goto','bzr_on')\"  type='button'>" L_ON "</button>"
  "<button id='gto_bzr_off' class='btns_left' onpointerdown=\"s('goto','bzr_off')\" type='button'>" L_OFF "</button>"
  "<br /><br />\n"
  "</div>";

const char html_gotoMfAuto[] PROGMEM =
  "<div class='sws-option-row'>"
  L_MERIDIAN_FLIP_AUTO ":<br />"
  "<button id='gto_mfa_on' class='btns_right' onpointerdown=\"s('goto','af_on')\"  type='button'>" L_ON "</button>"
  "<button id='gto_mfa_off' class='btns_left' onpointerdown=\"s('goto','af_off')\" type='button'>" L_OFF "</button>&nbsp;&nbsp;"
  "<button onpointerdown=\"s('goto','af_now')\" type='button'>" L_MERIDIAN_FLIP_NOW "</button>"
  "<br /><br />\n"
  "</div>";

const char html_gotoMfNow[] PROGMEM =
  "<div class='sws-option-row'>"
  L_ORIENTATION_AUTO ":<br />"
  "<button onpointerdown=\"s('goto','af_now')\" type='button'>" L_MERIDIAN_FLIP_NOW "</button>"
  "<br /><br />\n"
  "</div>";

const char html_gotoMfPause[] PROGMEM =
  "<div class='sws-option-row'>"
  "%s: <br />"
  "<button id='gto_mfp_on' class='btns_right' onpointerdown=\"s('goto','mp_on')\"  type='button'>" L_ON "</button>"
  "<button id='gto_mfp_off' class='btns_left' onpointerdown=\"s('goto','mp_off')\" type='button'>" L_OFF "</button>"
  "<br />\n"
  "</div>";

const char html_gotoMfHome[] PROGMEM =
  "<div class='sws-option-row'>"
  L_MERIDIAN_FLIP_HOMING ": <br />"
  "<button id='gto_mfh_off' class='btns_right' onpointerdown=\"s('goto','mh_off')\" type='button'>" L_MF_DIRECT "</button>"
  "<button id='gto_mfh_visit' class='btns_mid' onpointerdown=\"s('goto','mh_visit')\" type='button'>" L_MF_VIA_HOME "</button>"
  "<button id='gto_mfh_pause' class='btns_left' onpointerdown=\"s('goto','mh_pause')\" type='button'>" L_MF_PAUSE_HOME "</button>"
  "<br />\n"
  "</div>";

const char html_gotoPreferredPierSide1[] PROGMEM =
  "<div class='sws-option-row'>"
  "%s: <br />"
  "<button id='gto_pps_east' class='btns_right' onpointerdown=\"s('goto','pps_e')\"  type='button'>%s</button>"
  "<button id='gto_pps_west' class='btns_mid' onpointerdown=\"s('goto','pps_w')\" type='button'>%s</button>";

const char html_gotoPreferredPierSide2[] PROGMEM =
  "<button id='gto_pps_best' class='btns_mid' onpointerdown=\"s('goto','pps_b')\" type='button'>" L_BEST "</button>"
  "<button id='gto_pps_auto' class='btns_left' onpointerdown=\"s('goto','pps_a')\" type='button'>" L_AUTO "</button>"
  "<br />\n"
  "</div>";

// 坐标表单使用语言资源，按钮只在有效输入后提交完整坐标
const char html_coordinateInput[] PROGMEM =
"<form id='sws-coordinate-form' class='sws-coordinate-form' data-send='" L_COORD_SEND "' data-ok='" L_COORD_OK "' data-invalid='" L_COORD_INVALID "' data-reject='" L_COORD_REJECT "' data-network='" L_COORD_NETWORK "'>\n"
"<div class='sws-coordinate-grid'><label class='sws-coordinate-system'><span>" L_COORD_SYSTEM "</span><input value='" L_COORD_JNOW "' readonly></label>"
"<div class='sws-coordinate-actions'><button type='submit' name='coord_action' value='go'>" L_GOTO_ACTION "</button><button class='sws-secondary' type='submit' name='coord_action' value='sync'>" L_COORD_SYNC "</button></div>"
"<fieldset><legend>" L_COORD_RA "</legend><div class='sws-coordinate-units'>"
"<label class='sws-coordinate-unit'><input name='ra_h' type='number' min='0' max='23' step='1' inputmode='numeric' required aria-label='" L_COORD_H "'><span aria-hidden='true'>h</span></label>"
"<label class='sws-coordinate-unit'><input name='ra_m' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_M "'><span aria-hidden='true'>m</span></label>"
"<label class='sws-coordinate-unit'><input name='ra_s' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_S "'><span aria-hidden='true'>s</span></label>"
"</div></fieldset><fieldset><legend>" L_COORD_DEC "</legend><div class='sws-coordinate-units'>"
"<select name='dec_sign' aria-label='" L_COORD_SIGN "'><option value='+'>+</option><option value='-'>−</option></select>"
"<label class='sws-coordinate-unit'><input name='dec_d' type='number' min='0' max='90' step='1' inputmode='numeric' required aria-label='" L_COORD_D "'><span aria-hidden='true'>&#176;</span></label>"
"<label class='sws-coordinate-unit'><input name='dec_m' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_M "'><span aria-hidden='true'>&#8242;</span></label>"
"<label class='sws-coordinate-unit'><input name='dec_s' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_S "'><span aria-hidden='true'>&#8243;</span></label>"
"</div></fieldset></div><p role='status' class='sws-coordinate-message'></p></form>";

extern void gotoCoordinatesGet();

// -----------------------------------------------------------------------------------
// Guide tile





extern void guideTile(String &data);
extern void guideTileAjax(String &data);
extern void guideTileGet();

const char html_guidePad[] PROGMEM =
"<script>"
"var gmc=motionControls({p:'mount-ajax-get.txt',k:'dr',s:function(a){return a+'0';},a:'q'});"
"function gstopall(){gmc.stopAll();}"
"function gt(e,v){gmc.t(e,v,v.charAt(0));}"
"function gu(e,v){gmc.u(e,v.charAt(0));}"
"function gp(e,v){gmc.p(e,v,v.charAt(0));}"
"function gs(e,v){gmc.r(e,v.charAt(0));}"
"function gc(e){return gmc.c(e);}"
"</script>"
"<div class='sws-pad'><button class='gb' ontouchstart=\"gt(event,'n1')\" ontouchend=\"gu(event,'n0')\" ontouchcancel=\"gu(event,'n0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'n1')\" onpointerup=\"gs(event,'n0')\" onpointercancel=\"gs(event,'n0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_N "</button>"
"<button class='gb' ontouchstart=\"gt(event,'e1')\" ontouchend=\"gu(event,'e0')\" ontouchcancel=\"gu(event,'e0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'e1')\" onpointerup=\"gs(event,'e0')\" onpointercancel=\"gs(event,'e0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_E "</button>"
"<button class='gb sws-pad-stop' onpointerdown=\"gmc.halt();s('goto','stop')\" type='button'>" BUTTON_STOP "</button>"
"<button class='gb' ontouchstart=\"gt(event,'w1')\" ontouchend=\"gu(event,'w0')\" ontouchcancel=\"gu(event,'w0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'w1')\" onpointerup=\"gs(event,'w0')\" onpointercancel=\"gs(event,'w0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_W "</button>"
"<button class='gb' ontouchstart=\"gt(event,'s1')\" ontouchend=\"gu(event,'s0')\" ontouchcancel=\"gu(event,'s0')\" ontouchmove=\"return gc(event)\" onpointerdown=\"gp(event,'s1')\" onpointerup=\"gs(event,'s0')\" onpointercancel=\"gs(event,'s0')\" oncontextmenu=\"return gc(event)\" type='button'>" BUTTON_S "</button></div>";

const char html_guidePulseRates[] PROGMEM =
"<div class='sws-choice-source sws-guide-choice' data-label='" L_GUIDING_RATE "'>"
"<button id='guide_r0' class='btns_right' onpointerdown=\"g('R0')\" type='button'>0.25X</button>"
"<button id='guide_r1' class='btns_mid' onpointerdown=\"g('R1')\" type='button'>0.5X</button>"
"<button id='guide_r2' class='btns_left' onpointerdown=\"g('R2')\" type='button'>1X</button>"
"</div>";

const char html_guideRates[] PROGMEM =
"<div class='sws-choice-source sws-guide-choice' data-label='" L_MOVEMENT_RATE "'>"
"<button id='guide_r3' class='btns_right' onpointerdown=\"g('R3')\" type='button'>2X</button>"
"<button id='guide_r4' class='btns_mid' onpointerdown=\"g('R4')\" type='button'>4X</button>"
"<button id='guide_r5' class='btns_mid' onpointerdown=\"g('R5')\" type='button'>8X</button>"
"<button id='guide_r6' class='btns_mid' onpointerdown=\"g('R6')\" type='button'>20X</button>"
"<button id='guide_r7' class='btns_mid' onpointerdown=\"g('R7')\" type='button'>48X</button>"
"<button id='guide_r8' class='btns_mid' onpointerdown=\"g('R8')\" type='button'>MAX / 2</button>"
"<button id='guide_r9' class='btns_left' onpointerdown=\"g('R9')\" type='button'>MAX</button>"
"</div>";

// -----------------------------------------------------------------------------------
// Tracking tile





extern void trackingTile(String &data);
extern void trackingTileAjax(String &data);
extern void trackingTileGet();

const char html_trackingEnable[] PROGMEM = 
"<button id='trk_on' data-label='" L_DEVICE_TRACKING "' class='btns_right' onpointerdown=\"s('dr','T1')\" type='button'>" L_ON "</button>"
"<button id='trk_off' class='btns_left' onpointerdown=\"s('dr','T0')\" type='button'>" L_OFF "</button>";

const char html_trackingPreset[] PROGMEM =
"<div class='sws-choice-source' data-label='" L_TRACKING_RATE "'>"
"<br /><button id='trk_sid' class='bbh' style='width: 2em' onpointerdown=\"g('Ts')\" title='" L_HINT_RATE_SIDEREAL "' type='button'>" SIDEREAL_CH "</button>"
"<button id='trk_lun' class='bbh' style='width: 2em' onpointerdown=\"g('Tl')\" title='" L_HINT_RATE_LUNAR "' type='button'>" LUNAR_CH "</button>"
"<button id='trk_sol' class='bbh' style='width: 2em' onpointerdown=\"g('Th')\" title='" L_HINT_RATE_SOLAR "' type='button'>" SOLAR_CH "</button>"
"<button id='trk_king' class='bbh' style='width: 2em' onpointerdown=\"g('Tk')\" title='" L_HINT_RATE_KING "' type='button'>" KING_CH "</button>"
"</div>";

const char html_trackingCompensationType[] PROGMEM =
"<fieldset class='sws-tracking-setting'><legend>" L_TRK_COMP "</legend><div class='sws-tracking-options'>"
"<button id='ot_on'  class='btns_right' onpointerdown=\"s('rr','otk')\"  type='button'>" L_TRK_FULL "</button>"
"<button id='ot_ref' class='btns_mid'   onpointerdown=\"s('rr','on')\"   type='button'>" L_TRK_REFR "</button>"
"<button id='ot_off' class='btns_left'  onpointerdown=\"s('rr','off')\"  type='button'>" L_OFF "</button></div></fieldset>\n";

const char html_trackingCompensationAxes[] PROGMEM =
"<fieldset class='sws-tracking-setting'><legend>" L_TRK_AXES "</legend><div class='sws-tracking-options'>"
"<button id='ot_dul' class='btns_right' onpointerdown=\"s('rr','don')\"  type='button'>" L_TRK_DUAL "</button>"
"<button id='ot_sgl' class='btns_left'  onpointerdown=\"s('rr','doff')\" type='button'>" L_TRK_SINGLE "</button>"
"</div></fieldset>\n";

const char html_baseFrequency[] PROGMEM =
"<fieldset class='sws-tracking-setting'><legend>" L_FREQUENCY_ADJUST "</legend><div class='sws-tracking-options sws-frequency-options'>"
"<button onpointerdown=\"s('tk','f')\" type='button'>+ (0.02Hz " L_TRK_FASTER ")</button>"
"<button onpointerdown=\"s('tk','-')\" type='button'>- (0.02Hz " L_TRK_SLOWER ")</button>"
"<button onpointerdown=\"s('tk','r')\" type='button'>" L_TRK_RESET "</button>"
"</div></fieldset>\n";

// -----------------------------------------------------------------------------------
// Date/Time/Location tile




int get_temp_month;
int get_temp_day;
int get_temp_year;
int get_temp_hour;
int get_temp_minute;
int get_temp_second;

// create the related webpage tile
void siteTile(String &data)
{
  char temp[640] = "";
  char reply[80] = "";
  bool negativeValue;

  data.concat(F("<div class='b1 sws-site sws-editor'><header class='sws-editor-heading'><h2>" L_POSITION_INFO "</h2><button class='sws-secondary sws-edit-button' type='button' data-edit-form='sws-site-form'>" SWS_EDIT_ICON L_EDIT_POSITION "</button></header>"));
  data.concat(F("<form id='sws-site-form' class='sws-settings-editor' method='get' action='/mount.htm'><div class='sws-site-fields'>"));
  // Longitude
  data.concat(FPSTR(html_longMsg));
  if (!onStep.command(status.getVersionMajor() > 3 ? ":GgH#" : ":Gg#", reply))
    strcpy(reply, status.getVersionMajor() > 3 ? "+000*00:00" : "+000*00");
  reply[4] = 0;
  reply[7] = 0;
  reply[10] = 0;
  if (reply[0] == '+') reply[0] = '0';
  negativeValue = reply[0] == '-';
  convert.stripNumericStr(reply);
  if (negativeValue && atoi(reply) == 0) strcpy(reply, "-0");
  snprintf_P(temp, sizeof(temp), html_ninput_wide, "g1", reply, "-180", "180", "&deg;");
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_ninput, "g2", &reply[5], "0", "60", "'");
  data.concat(temp);
  if (status.getVersionMajor() > 3)
  {
    snprintf_P(temp, sizeof(temp), html_ninput, "g3", &reply[8], "0", "60", "\"");
    data.concat(temp);
  }
  data.concat(F("</div></fieldset>"));
  www.sendContentAndClear(data);

  // Latitude
  data.concat(FPSTR(html_latMsg));
  if (!onStep.command(status.getVersionMajor() > 3 ? ":GtH#" : ":Gt#", reply))
    strcpy(reply, status.getVersionMajor() > 3 ? "+00*00:00" : "+00*00");
  reply[3] = 0;
  reply[6] = 0;
  reply[9] = 0;
  if (reply[0] == '+') reply[0] = '0';
  negativeValue = reply[0] == '-';
  convert.stripNumericStr(reply);
  if (negativeValue && atoi(reply) == 0) strcpy(reply, "-0");
  snprintf_P(temp, sizeof(temp), html_ninput_wide, "t1", reply, "-90", "90", "&deg;");
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_ninput, "t2", &reply[4], "0", "60", "'");
  data.concat(temp);
  if (status.getVersionMajor() > 3)
  {
    snprintf_P(temp, sizeof(temp), html_ninput, "t3", &reply[7], "0", "60", "\"");
    data.concat(temp);
  }
  data.concat(F("</div></fieldset>"));
  www.sendContentAndClear(data);

  // UTC Offset
  data.concat(FPSTR(html_offsetMsg));
  if (!onStep.command(":GG#", reply)) strcpy(reply, "+00");
  reply[3] = 0;
  if (reply[0] == '+') reply[0] = '0';
  negativeValue = reply[0] == '-';
  convert.stripNumericStr(reply);
  if (negativeValue && atoi(reply) == 0) strcpy(reply, "-0");
  snprintf_P(temp, sizeof(temp), html_ninput_wide, "u1", reply, "-14", "12", "h");
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_offsetMin, reply[4] == '0' ? "selected" : "", reply[4] == '3' ? "selected" : "", reply[4] == '4' ? "selected" : "");
  data.concat(temp);
  data.concat(FPSTR(html_offsetFooterMsg));
  www.sendContentAndClear(data);

  data.concat(F("</div><div class='sws-editor-footer'><button type='reset' class='sws-secondary'>" L_UI_CANCEL "</button><button type='submit'>" L_SAVE_SETTINGS "</button></div></form>"));
  // 时间同步仍使用原始日期时间参数；位置读数标识保留给原有轮询。
  data.concat(F("<div class='sws-clock-controls'>"));
  data.concat(FPSTR(html_localTime));
  data.concat(FPSTR(html_setDateTime));
  data.concat(F("</div>"));
  snprintf_P(temp, sizeof(temp), html_date, state.dateStr); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_time, state.timeStr); data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_sidereal, state.lastStr); data.concat(temp);
  data.concat(FPSTR(html_browserTime));
  snprintf_P(temp, sizeof(temp), html_site, state.latitudeStr, state.longitudeStr); data.concat(temp);
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Library tile




int currentCatalog = 0;
char currentCatName[12] = "";
String currentObject = L_CAT_NO_OBJECT;
bool catalogIndexChanged = false;
bool downloadCatalogData = false;
bool uploadCatalogData = false;
String showMessage = L_CAT_NO_CAT;
unsigned long currentCatNameShowTime = 0;

// create the related webpage tile
void libraryTile(String &data)
{
  char temp[400] = "";

  // reset everything on page load
  currentCatalog = 0;
  currentCatName[0] = 0;
  currentObject = L_CAT_NO_OBJECT;
  catalogIndexChanged = false;
  downloadCatalogData = false;
  uploadCatalogData = false;
  showMessage = L_CAT_NO_CAT;

  // Scripts
  data.concat(FPSTR(html_script_ajax_library));

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_PAGE_LIBRARY);
  // 卡片标记用于响应式布局，不改变设备功能判断
  data.concat(temp);
  data.concat(F("<div class='sws-card-marker' data-card='library'></div>"));
  data.concat(F("<div style='float: right; text-align: right;' id='lib_free' class='c'>"));
  data.concat(onStep.commandString(":L?#"));
  data.concat(" ");
  data.concat(L_RECS_AVAIL);
  data.concat(F("</div><br />"));
  data.concat(F("<hr>"));

  data.concat(FPSTR(html_libCatalogSelect));
  www.sendContentAndClear(data);
  data.concat(F("<br />"));
  data.concat(FPSTR(html_libShowMessage));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_catPrevRec));
  data.concat(FPSTR(html_catNextRec));
  data.concat(FPSTR(html_catSetTarget));
  data.concat(FPSTR(html_catShowMessage));
  www.sendContentAndClear(data);

  data.concat(F("<hr>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  // 数据框先于操作按钮，上传内容与动作在同一区域
  data.concat(FPSTR(html_libEditCatalog));
  data.concat(F("<div class='sws-actions'>"));
  data.concat(FPSTR(html_libUploadCatalog));
  data.concat(FPSTR(html_libDownloadCatalog));
  data.concat(FPSTR(html_libClearCatalog));
  data.concat(F("</div>"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_libCatalogHelp));

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Home/Park tile




const char *getHomeParkStateStr();

// create the related webpage tile
void homeParkTile(String &data)
{
  char temp[640] = "";
  char reply[120] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_HOME_PARK_TITLE);
  // 卡片标记用于响应式布局，不改变设备功能判断
  data.concat(temp);
  data.concat(F("<div class='sws-card-marker' data-card='homepark'></div>"));
  data.concat(F("<div style='float: right; text-align: right;' id='hp_sta' class='c'>"));
  data.concat(getHomeParkStateStr());
  data.concat(F("</div><br /><hr>"));

  // 归位与停车各占一组，常用操作在同一行排列
  data.concat(F("<div class='sws-home-actions'><div><h3>" L_HOME_ACTIONS "</h3><div class='sws-action-pair'>"));
  data.concat(FPSTR(html_homeReturn));
  data.concat(FPSTR(html_homeReset));
  data.concat(F("</div></div><div><h3>" L_PARK_ACTIONS "</h3><div class='sws-action-pair'>"));
  data.concat(F("<div class='sws-park-toggle'>"));
  data.concat(FPSTR(html_park));
  data.concat(FPSTR(html_unpark));
  data.concat(F("</div>"));
  data.concat(FPSTR(html_setpark));
  data.concat(F("</div></div></div>"));
  data.concat(F("<hr>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  data.concat(F("<div class='sws-option-grid'>"));
  www.sendContentAndClear(data);

  // home options when home sense is available
  if (status.getVersionMajor()*100 + status.getVersionMinor() >= 1020) {
    data.concat(F("<div class='sws-option-row'>" L_HOME_AUTO "<br />"));
    data.concat(FPSTR(html_homeAuto));
    data.concat(F("</div>"));
    www.sendContentAndClear(data);

    data.concat(F("</div>"));
    if (onStep.command(":h?#", reply)) {
      status.hasHomeSense = false;
      int hasHomeSense = 0;
      long homeOffsetAxis1 = 0;
      long homeOffsetAxis2 = 0;
      if (sscanf(reply, "%d,%ld,%ld", &hasHomeSense, &homeOffsetAxis1, &homeOffsetAxis2) == 3) {
        status.hasHomeSense = (bool)hasHomeSense;

        if (status.hasHomeSense) {

          data.concat(FPSTR(html_collapsable_end));
          data.concat(F("<div style='margin-top: 0.5em';></div>"));
          www.sendContentAndClear(data);

          snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
          data.concat(temp);

          #ifdef HOME_SWITCH_DIRECTION_CONTROL
            data.concat(F(L_HOME_REV "<br />"));
            data.concat(html_homeReverse);
          #endif

          snprintf_P(temp, sizeof(temp), html_form_begin, "mount.htm");
          data.concat(temp);

          data.concat(F(L_HOME_OFFSET "<br />"));
          snprintf_P(temp, sizeof(temp), html_homeOffsetAxis1, homeOffsetAxis1);
          data.concat(temp);
          snprintf_P(temp, sizeof(temp), html_homeOffsetAxis2, homeOffsetAxis2);
          data.concat(temp);
          data.concat(F("<button type='submit'>" L_UPLOAD "</button><br />\n"));

          data.concat(FPSTR(html_form_end));
          www.sendContentAndClear(data);
        }
      }
    }
  }

  if (status.getVersionMajor()*100 + status.getVersionMinor() < 1020) data.concat(F("</div>"));
  data.concat(FPSTR(html_collapsable_end));

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Align tile




// create the related webpage tile
void alignTile(String &data)
{
  char temp[640] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_ALIGN);
  // 卡片标记用于响应式布局，不改变设备功能判断
  data.concat(temp);
  data.concat(F("<div class='sws-card-marker' data-card='align'></div>"));

  data.concat(F("<div style='float: right; text-align: right;' id='align_progress' class='c'>"));
  data.concat(state.alignProgress);
  data.concat(F("</div><br /><hr>"));

  // 中文天极名称按 UTF-8 字节预留空间，避免越界破坏相邻数据
  char poleName[32] = L_ZENITH;
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
  data.concat(F("<div class='sws-actions'>"));
  data.concat(FPSTR(html_alignAccept));
  for (int i = 0; i < n; i++)
  {
    char temp2[240] = "";
    sprintf_P(temp2, html_alignStart, i + 1, sc[i], sc[i], SIDEREAL_CH);
    data.concat(temp2);
  }
  www.sendContentAndClear(data);

  data.concat(F("</div><hr>"));

  if (status.mountType != MT_ALTAZM)
  {
    snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
    data.concat(temp);

    data.concat(F("<h3 class='sws-refine-title'>" L_REFINE_POLAR_ALIGN "</h3>"));
    data.concat(FPSTR(html_alignRefine));

    data.concat(FPSTR(html_collapsable_end));
  }
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Limits and Backlash tile




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


// -----------------------------------------------------------------------------------
// PEC tile




// create the related webpage tile
void pecTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_PAGE_PEC);
  // 卡片标记用于响应式布局，不改变设备功能判断
  data.concat(temp);
  data.concat(F("<div class='sws-card-marker' data-card='pec'></div>"));
  data.concat(F("<br /><hr>"));

  if (status.pecEnabled) {
    data.concat(FPSTR(html_pecStatus));
  }

  data.concat(F("<br /><hr>"));

  // display pec controls
  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  if (status.pecEnabled) {
    data.concat(FPSTR(html_pecControls1));
    data.concat(FPSTR(html_pecControls2));
    data.concat(FPSTR(html_pecControls3));
    data.concat(FPSTR(html_pecControls4));
  } else {
    data.concat(L_DISABLED_MESSAGE);
  }

  data.concat(FPSTR(html_collapsable_end));
  www.sendContentAndClear(data);

  // display steps per worm rotation
  data.concat(F("<div style='margin-top: 0.5em';></div>"));
  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);

  char response[80];
  if (status.pecEnabled && onStep.command(":GXE7#", response)) {
    snprintf_P(temp, sizeof(temp), html_form_begin, "mount.htm");
    data.concat(temp);

    long value = strtol(response, NULL, 10);
    snprintf_P(temp, sizeof(temp), html_configAxisSpwr, value, 0, 129600000L);
    data.concat(temp);
    data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button><br />\n"));

    data.concat(FPSTR(html_form_end));
  } else {
    data.concat(L_DISABLED_MESSAGE);
  }
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_collapsable_end));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Goto tile





#define KV_GTO_STATUS 0
#define KV_GTO_I1 1
#define KV_GTO_I2 2
#define KV_GTO_T1 3
#define KV_GTO_T2 4
#define KV_GTO_ACTIVE 5
#define KV_GTO_BZR_OFF 6
#define KV_GTO_BZR_ON 7
#define KV_GTO_MFA_OFF 8
#define KV_GTO_MFA_ON 9
#define KV_GTO_MFP_OFF 10
#define KV_GTO_MFP_ON 11
#define KV_GTO_RATE 12
#define KV_GTO_RATE_VF 13
#define KV_GTO_RATE_F 14
#define KV_GTO_RATE_N 15
#define KV_GTO_RATE_S 16
#define KV_GTO_RATE_VS 17

const char *goto_key[18] =
{
  "gto_rate",
  "gto_rate_vf",
  "gto_rate_f",
  "gto_rate_n",
  "gto_rate_s",
  "gto_rate_vs"
};

// create the related webpage tile
void gotoTile(String &data)
{
  char temp[640] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", L_COORD_GOTO);
  data.concat(temp);

  data.concat(F("<div style='float: right; text-align: right;' id='gto_status' class='c'>"));
  snprintf(temp, sizeof(temp), "%s", status.inGoto ? L_SLEWING : L_INACTIVE);
  data.concat(temp);
  data.concat(F("</div><div class='sws-pier-status'>" L_PIER_SIDE " <span id='gto_pier'>"));
  data.concat(state.pierSideStr);
  data.concat(F("</span></div><hr>"));

  // 坐标信息在左，常用定位操作在右
  data.concat(F("<div class='sws-goto-overview'>"));
  data.concat(FPSTR(html_mountPositionLabels));
  snprintf_P(temp, sizeof(temp), html_mountPositionAxis1, state.indexAzmStr, state.indexRaStr, state.targetRaStr);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_mountPositionAxis2, state.indexAltStr, state.indexDecStr, state.targetDecStr);
  data.concat(temp);

  www.sendContentAndClear(data);

  // 四个操作按钮组成两行，窄屏时整体移到坐标下方
  data.concat(F("<div class='sws-actions'>"));
  data.concat(FPSTR(html_gotoGo));
  data.concat(FPSTR(html_gotoStop));
  data.concat(FPSTR(html_gotoContinue));
  data.concat(FPSTR(html_gotoSync));
  data.concat(F("</div></div>"));
  data.concat(FPSTR(html_coordinateInput));
  data.concat(F("<hr>"));
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  // 定位选项采用独立分组，长文字在窄屏自然换行
  data.concat(F("<div class='sws-option-grid'><div class='sws-option-row'>"));
  // Slew speed
  snprintf_P(temp, sizeof(temp), html_slewSpeed, state.slewSpeedStr);
  data.concat(temp);
  data.concat(FPSTR(html_slewSpeedSelect));
  data.concat(F("</div>"));

  // Goto Buzzer
  data.concat(FPSTR(html_gotoBuzzer));
  www.sendContentAndClear(data);

  // Goto Meridian Flips
  if (status.mountType == MT_GEM || (status.getVersionMajor() >= 10 && status.meridianFlips))
  {
    if (status.mountType == MT_ALTAZM) {
      data.concat(FPSTR(html_gotoMfNow));
      snprintf_P(temp, sizeof(temp), html_gotoMfPause, L_ORIENTATION_CHANGE_PAUSE);
      data.concat(temp);
    } else {
      data.concat(FPSTR(html_gotoMfAuto));
      if (status.hasMeridianFlipHomeModes() && (status.mountType == MT_GEM || status.mountType == MT_FORK)) {
        data.concat(FPSTR(html_gotoMfHome));
      } else {
        snprintf_P(temp, sizeof(temp), html_gotoMfPause, L_MERIDIAN_FLIP_PAUSE);
        data.concat(temp);
      }
    }

    www.sendContentAndClear(data);
  }

  // Goto Preferred Pier Side
  if (status.mountType != MT_ALTAZM || (status.getVersionMajor() >= 10 && status.meridianFlips))
  {
    data.concat(F("<br />"));

    if (status.mountType == MT_ALTAZM) {
      snprintf_P(temp, sizeof(temp), html_gotoPreferredPierSide1, L_ORIENTATION_CHANGE_PPS, L_NORMAL, L_ALTERNATE);
      data.concat(temp);
    } else {
      snprintf_P(temp, sizeof(temp), html_gotoPreferredPierSide1, L_MERIDIAN_FLIP_PPS, L_EAST, L_WEST);
      data.concat(temp);
    }
    www.sendContentAndClear(data);

    data.concat(FPSTR(html_gotoPreferredPierSide2));
  }

  data.concat(F("</div>"));
  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Guide tile




// create the related webpage tile
void guideTile(String &data)
{
  char temp[240] = "";
  char temp1[120] = "";

  snprintf(temp1, sizeof(temp1), L_GUIDE " <span id='guide_rate' class='c'>%s</span>", GuideRatesStr[status.guideRate]);
  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "15em", temp1);
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;' id='guide_sta' class='c'>"));
  if (status.guiding) data.concat(L_SLEWING); else data.concat(L_INACTIVE);
  data.concat(F("</div><br />"));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_script_motion_controls_1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_2));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_guidePad));
  www.sendContentAndClear(data);

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  data.concat(FPSTR("Guide rate, in x sidereal:<br />"));

  snprintf_P(temp, sizeof(temp), html_form_begin, "mount.htm");
  data.concat(temp);

  data.concat(FPSTR(html_guidePulseRates));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_guideRates));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_form_end));

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Tracking tile




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


int main(){String data;state.trackStr=L_INACTIVE;std::cout<<"PREVIEW_Site\n";www.output.clear();siteTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Library\n";www.output.clear();libraryTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_HomePark\n";www.output.clear();homeParkTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Align\n";www.output.clear();alignTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Limits\n";www.output.clear();limitsTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Pec\n";www.output.clear();pecTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Goto\n";www.output.clear();gotoTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Guide\n";www.output.clear();guideTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Tracking\n";www.output.clear();trackingTile(data);std::cout<<www.output<<data;data.clear();}