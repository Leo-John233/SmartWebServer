#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#define PROGMEM
#define ON 1
#define OFF 0
#define DISPLAY_SPECIAL_CHARS ON
#define AJAX_PAGE_LAZY_GET_MS 200
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
#define MT_ALTAZM 3
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
struct String:std::string{using std::string::string;void concat(const char*s){append(s);}};
struct Web{std::string output;void sendContentAndClear(String&s){output+=s;s.clear();}}www;
struct Status{int focuserCount=2,mountType=MT_ALTAZM;bool derotatorFound=true;bool focuserPresent[6]={true,true,false,false,false,false};int getVersionMajor(){return 10;}}status;
struct State{bool focuserSlewing=false,rotatorSlewing=false,focuserTcfEnable=true;int focuserSelected=1;
 const char*focuserPositionStr="12500 µm",*rotatorPositionStr="45°30′",*focuserSlewSpeedStr="0.30mm/s",*rotateSlewSpeedStr="3.0&deg;/s",*focuserTemperatureStr="18.6 °C",*focuserBacklashStr="12",*focuserDeadbandStr="8",*focuserTcfCoefStr="-2.5";void updateFocuser(){}}state;
struct Device{bool command(const char*,char*out){strcpy(out,"16");return true;}}onStep;
#define DISPLAY_LANGUAGE L_fr
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
  "</section><section class='sws-target'>";

const char html_layout_mount_tracking[] PROGMEM =
  "</section><section class='sws-tracking'>";

const char html_layout_mount_home[] PROGMEM =
  "</section></div><section class='sws-home'>";

const char html_layout_mount_site[] PROGMEM =
  "</section><section><div class='sws-section-head'><h2 class='sws-section-title'>" L_LOCATION_TITLE " / " L_PAGE_LIBRARY "</h2><p></p></div><div class='sws-settings-grid sws-site-library'>";

const char html_layout_mount_advanced[] PROGMEM =
  "</div></section><details class='sws-more' open><summary>" L_ALIGN " / " L_LIMITS_TITLE " / " L_PAGE_PEC "</summary><div class='sws-settings-grid sws-advanced-grid'>";

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
  "var xhttp = new XMLHttpRequest();"
  "xhttp.open('GET','%s?'+key+'='+encodeURIComponent(v1)+'&x='+new Date().getTime(), true);"
  "xhttp.send();"
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
"var m={},q=[],b=0,g=0;"
"function send(v){q.push(v);run();}"
"function run(){if(b||!q.length)return;b=1;var v=q.shift(),x=new XMLHttpRequest(),d=0;function f(){if(d)return;d=1;b=0;run();}x.onreadystatechange=function(){if(x.readyState==4)f();};x.onerror=x.ontimeout=f;x.timeout=1000;x.open('GET',o.p+'?'+o.k+'='+encodeURIComponent(v)+'&x='+new Date().getTime(),true);x.send();}"
"function active(){for(var k in m)if(m[k])return true;return false;}"
"function stopValue(a){return typeof o.s=='function'?o.s(a):o.s;}"
"function allStopValue(){return typeof o.a=='function'?o.a():o.a;}"
"function haltValue(){return typeof o.h=='function'?o.h():o.h||allStopValue()||stopValue('');}"
"function repeat(a,h){return o.g?h==g&&!active():!m[a];}"
"function stop(a){if(!m[a])return;m[a]=0;var h=++g,v=stopValue(a);send(v);setTimeout(function(){if(repeat(a,h))send(v)},150);setTimeout(function(){if(repeat(a,h))send(v)},500);}"
"function clearActive(){var r=0;for(var k in m){if(m[k])r=1;m[k]=0;}return r;}";

const char html_script_motion_controls_2[] PROGMEM =
"function repeatSend(v){send(v);setTimeout(function(){send(v)},150);setTimeout(function(){send(v)},500);}"
"function stopAll(){var v=allStopValue();if(v){if(clearActive()){g++;repeatSend(v);}return;}for(var k in m)stop(k);}"
"function halt(){clearActive();g++;repeatSend(haltValue());}"
"function down(e,v,a){if(e&&e.preventDefault)e.preventDefault();a=a||v;m[a]=1;g++;send(v);}"
"function up(e,a){if(e&&e.preventDefault)e.preventDefault();stop(a);}"
"function pointerDown(e,v,a){if(e.pointerType=='touch')return;down(e,v,a);}"
"function pointerUp(e,a){if(e.pointerType=='touch')return;up(e,a);}"
"function windowUp(e){if(e&&e.pointerType=='touch')return;stopAll();}"
"function cancel(e){if(e&&e.preventDefault)e.preventDefault();return false;}"
"window.addEventListener('pointerup',windowUp);"
"window.addEventListener('pointercancel',windowUp);"
"window.addEventListener('touchend',stopAll);"
"window.addEventListener('touchcancel',stopAll);"
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
"document.getElementById('dd').value = d.getDate();"
"document.getElementById('dm').value = d.getMonth();"
"document.getElementById('dy').value = d.getFullYear();"
"document.getElementById('th').value = d.getHours();"
"document.getElementById('tm').value = d.getMinutes();"
"document.getElementById('ts').value = d.getSeconds();"
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
  "document.getElementById('datetime').innerHTML = \n"
  "(now.getUTCMonth()+1)+'/'+pad(now.getUTCDate().toString(),2)+'/'+pad(now.getUTCFullYear().toString().substr(-2),2)+' '+\n"
  "pad(now.getUTCHours().toString(),2)+':'+pad(now.getUTCMinutes().toString(),2)+':'+pad(now.getUTCSeconds().toString(),2);\n"
"}\n"
"var auto1Tick=-1;\n"
"var auto2Tick=0;\n"
"var auto2Rate=" STR(AJAX_PAGE_UPDATE_RATE_MS) "/10;\n"
"var auto1=setInterval(autoRun,10);\n"
"function autoFastRun() {\n"
  "auto2Rate=" STR(AJAX_PAGE_UPDATE_RATE_FAST_MS) "/10\n"
  "auto2Tick=" STR(AJAX_PAGE_UPDATE_FAST_SHED_MS) "/10;\n"
"}\n"
"function autoRun() {\n"
  "auto1Tick++;\n"
  "var i;\n"
  "if (auto2Tick>=0) auto2Tick--;\n"
  "if (auto2Tick==0) auto2Rate=" STR(AJAX_PAGE_UPDATE_RATE_MS) "/10;\n"
  "if (auto1Tick%auto2Rate==0) {\n"
    "nocache='?nocache='+Math.random()*1000000;\n"
    "var request = new XMLHttpRequest();\n"
    "request.onreadystatechange = pageReady(ajaxPage);\n"
    "request.open('GET',ajaxPage.toLowerCase()+nocache,true); request.send(null);\n"
  "}\n"
"}\n"
"function pageReady(aPage) {\n"
  "return function() {\n"
    "if ((this.readyState==4)&&(this.status==200)) {\n"
      "lines=this.responseText.split('\\n');\n"
      "for (var i=0; i<lines.length; i++) {\n"
        "var j=lines[i].indexOf('|');\n"
        "var m=0;\n"
        "if (j==-1) {j=lines[i].indexOf('&');m=1;}\n"
        "var v=lines[i].slice(j+1);\n"
        "var k=lines[i].slice(0,j);\n"
        "if (k=='call' && v=='update_date_time') {\n"
          "update_date_time();\n"
        "} else {\n"
          "if (k!='') {"
            "var e=document.getElementById(k);\n"
            "if (e!=null) {\n"
              "if (m==1) e.value=v; else\n"
              "if (v=='selected') { e.dataset.selected='true'; } else\n"
              "if (v=='unselected') { e.dataset.selected='false'; } else\n"
              "if (v=='disabled') e.disabled=true; else\n"
              "if (v=='enabled') e.disabled=false; else\n"
              "e.innerHTML=v;\n"
            "}\n"
          "}\n"
        "}\n"
      "}\n"
    "}\n"
  "}\n"
"}\n"
"</script>\n";

// -----------------------------------------------------------------------------------
// Rotator Goto tile





extern void rotatorSlewingTile(String &data);
extern void rotatorSlewingTileAjax(String &data);
extern void rotatorSlewingTileGet();

const char html_rotPosition[] PROGMEM =
"<label class='sws-accessory-target'><span>" L_TARGET "</span><span class='sws-accessory-input'><input id='rot_set' value='0.0' type='number' min='-360' max='360' step='any' required><span>&deg;</span></span></label>"
"<button class='bbh' onpointerdown=\"{const e=document.getElementById('rot_set');if(e.reportValidity())s('rs',e.value);}\" type='button'>" L_SET_LABEL "</button>";

const char html_rotStop[] PROGMEM =
"<button class='bbh sws-accessory-stop' onpointerdown=\"if(window.rmc)rmc.halt();else s('rot','stop')\" type='button' title='" L_HINT_EMERGENCY_STOP "'>" L_STOP "</button>";

const char html_rotateCcw[] PROGMEM =
  "<script>"
  "var rmc=motionControls({p:'rotator-ajax-get.txt',k:'rot',s:'stop',g:1});"
  "function rt(e,v){rmc.t(e,v);}"
  "function ru(e,v){rmc.u(e,v);}"
  "function rp(e,v){rmc.p(e,v);}"
  "function rr(e,v){rmc.r(e,v);}"
  "function rc(e){return rmc.c(e);}"
  "</script>"
  "<button class='bbh' ontouchstart=\"rt(event,'ccwf')\" ontouchend=\"ru(event,'ccwf')\" ontouchcancel=\"ru(event,'ccwf')\" ontouchmove=\"return rc(event)\" onpointerdown=\"rp(event,'ccwf')\" onpointerup=\"rr(event,'ccwf')\" onpointercancel=\"rr(event,'ccwf')\" oncontextmenu=\"return rc(event)\" type='button'><span>" ARROW_LL "</span><small>" L_ROT_CCW "</small></button>"
  "<button class='bbh' ontouchstart=\"rt(event,'ccw')\" ontouchend=\"ru(event,'ccw')\" ontouchcancel=\"ru(event,'ccw')\" ontouchmove=\"return rc(event)\" onpointerdown=\"rp(event,'ccw')\" onpointerup=\"rr(event,'ccw')\" onpointercancel=\"rr(event,'ccw')\" oncontextmenu=\"return rc(event)\" type='button'><span>" ARROW_L "</span><small>" L_ROT_CCW "</small></button>";

const char html_rotateCw[] PROGMEM =
  "<button class='bbh' ontouchstart=\"rt(event,'cw')\" ontouchend=\"ru(event,'cw')\" ontouchcancel=\"ru(event,'cw')\" ontouchmove=\"return rc(event)\" onpointerdown=\"rp(event,'cw')\" onpointerup=\"rr(event,'cw')\" onpointercancel=\"rr(event,'cw')\" oncontextmenu=\"return rc(event)\" type='button'><span>" ARROW_R "</span><small>" L_ROT_CW "</small></button>"
  "<button class='bbh' ontouchstart=\"rt(event,'cwf')\" ontouchend=\"ru(event,'cwf')\" ontouchcancel=\"ru(event,'cwf')\" ontouchmove=\"return rc(event)\" onpointerdown=\"rp(event,'cwf')\" onpointerup=\"rr(event,'cwf')\" onpointercancel=\"rr(event,'cwf')\" oncontextmenu=\"return rc(event)\" type='button'><span>" ARROW_RR "</span><small>" L_ROT_CW "</small></button><br />";

const char html_rotateSlewSpeed[] PROGMEM =
"<div class='sws-accessory-live-rate'><span>" L_POSITIONING_SPEED "</span><strong id='rot_rate' class='c'>%s</strong></div>";

const char html_rotateGotoSelect[] PROGMEM =
"<div class='sws-choice-source' data-label='" L_POSITIONING_RATE "'>"
  "<button id='rot_rate_vf' class='btns_right' onpointerdown=\"s('rot','vf')\" type='button'>" L_VFAST "</button>"
  "<button id='rot_rate_f' class='btns_mid' onpointerdown=\"s('rot','f')\"  type='button'>" L_FAST "</button>"
  "<button id='rot_rate_n' class='btns_mid' onpointerdown=\"s('rot','n')\"  type='button'>" L_NORMAL "</button>"
  "<button id='rot_rate_s' class='btns_mid' onpointerdown=\"s('rot','s')\"  type='button'>" L_SLOW "</button>"
  "<button id='rot_rate_vs' class='btns_left' onpointerdown=\"s('rot','vs')\" type='button'>" L_VSLOW "</button>"
  "</div>\n";

// -----------------------------------------------------------------------------------
// Rotator Home tile





extern void rotatorHomeTile(String &data);
extern void rotatorHomeTileAjax(String &data);
extern void rotatorHomeTileGet();

const char html_rotatorHome[] PROGMEM =
  "<button onpointerdown=\"s('rot','home')\" type='button'>"
  L_RETURN_HOME "</button>"
  "<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('rot','reset')\" type='button'>"
  L_RESET_HOME "</button>";

// -----------------------------------------------------------------------------------
// Rotator Backlash tile





extern void rotatorBacklashTile(String &data);
extern void rotatorBacklashTileAjax(String &data);
extern void rotatorBacklashTileGet();

const char html_backlashValue[] PROGMEM =
"<div class='sws-accessory-metrics'><div><span>" L_BACKLASH "</span><strong class='c'>%d " L_UNIT_STEPS "</strong></div></div>";

const char html_configBlAxis3[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_BACKLASH_RANGE_AXIS3UP "</span><input value='%d' type='number' name='b3' min='0' max='32767' required></label>";

// -----------------------------------------------------------------------------------
// DeRotator tile





extern void deRotatorTile(String &data);
extern void deRotatorTileAjax(String &data);
extern void deRotatorTileGet();

const char html_controlDeRotate[] PROGMEM =
"<div class='sws-accessory-derotate'>"
"<button id='rot_on' class='btns_right' onpointerdown=\"s('rot','on')\" type='button'>" L_ON "</button>"
"<button id='rot_off' class='btns_left' onpointerdown=\"s('rot','off')\" type='button'>" L_OFF "</button><br />"
"<button id='rot_rev' onpointerdown=\"s('rot','rev')\" type='button'>" L_HINT_ROTATOR_REV "</button><br />"
"<button onpointerdown=\"s('rot','par')\" type='button'>" L_HINT_ROTATOR_PAR "</button></div>";

// -----------------------------------------------------------------------------------
// Rotator Slewing tile





// 位置读数、目标输入与手动移动分别分组
void rotatorSlewingTile(String &data)
{
  char temp[240] = "";
  data.concat(F("<div class='b1 sws-accessory-motion-card'><div class='sws-device-card-head'><h2>" L_ROTATOR "</h2><span id='rot_sta' class='sws-accessory-status c'>"));
  data.concat(state.rotatorSlewing ? L_ACTIVE : L_INACTIVE);
  data.concat(F("</span></div><div class='sws-accessory-position'><span>" L_CURRENT "</span><strong id='rotatorpos' class='c'>"));
  data.concat(state.rotatorPositionStr);
  data.concat(F("</strong></div><div class='sws-accessory-goto'>"));
  data.concat(FPSTR(html_rotPosition));
  data.concat(FPSTR(html_rotStop));
  data.concat(F("</div><p class='sws-accessory-hint'>" L_HOLD_TO_MOVE "</p><div class='sws-accessory-jog'>"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_1));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_motion_controls_2));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_rotateCcw));
  data.concat(FPSTR(html_rotateCw));
  data.concat(F("</div>"));
  if (status.getVersionMajor() >= 10) {
    data.concat(FPSTR(html_rotateGotoSelect));
    snprintf_P(temp, sizeof(temp), html_rotateSlewSpeed, state.rotateSlewSpeedStr);
    data.concat(temp);
  }
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Rotator Home tile




// 两种设备只提供参考位置操作，不包含赤道仪停放功能
void rotatorHomeTile(String &data)
{
  data.concat(F("<div class='b1 sws-accessory-home-card'><div class='sws-device-card-head'><h2>" L_HOME_ACTIONS "</h2></div><p class='sws-accessory-hint'>" L_HOME_REFERENCE_HELP "</p><div class='sws-accessory-home-actions'>"));
  data.concat(FPSTR(html_rotatorHome));
  data.concat(F("</div></div>"));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// Rotator Backlash tile




// create the related webpage tile
void rotatorBacklashTile(String &data)
{
  char temp[240] = "";
  char temp1[32] = "";

  if (!onStep.command(":rb#", temp1)) strcpy(temp1, "0");
  int backlash = atoi(temp1);

  data.concat(F("<div class='b1 sws-accessory-parameter-card'><div class='sws-device-card-head'><h2>" L_BACKLASH "</h2></div>"));

  snprintf_P(temp, sizeof(temp), html_backlashValue, backlash);
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS);
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_form_begin, "rotator.htm");
  data.concat(temp);

  snprintf_P(temp, sizeof(temp), html_configBlAxis3, backlash);
  data.concat(temp);
  data.concat(F("<div class='sws-accessory-form-actions'><button type='submit'>" L_SAVE_SETTINGS "</button></div>\n"));

  data.concat(FPSTR(html_form_end));
  www.sendContentAndClear(data);


  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


// -----------------------------------------------------------------------------------
// DeRotator tile




// create the related webpage tile
void deRotatorTile(String &data)
{
  char temp[240] = "";

  snprintf_P(temp, sizeof(temp), html_tile_beg, "22em", "13em", L_DEROTATOR);
  data.concat(temp);
  data.concat(F("<br /><hr>"));

  data.concat(FPSTR(html_controlDeRotate));
  www.sendContentAndClear(data);

  data.concat(F("<hr>"));

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}


int main(){String data;std::cout<<"PREVIEW_Slewing\n";www.output.clear();rotatorSlewingTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Home\n";www.output.clear();rotatorHomeTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_Backlash\n";www.output.clear();rotatorBacklashTile(data);std::cout<<www.output<<data;data.clear();std::cout<<"PREVIEW_DeRotator\n";www.output.clear();if(status.derotatorFound)deRotatorTile(data);std::cout<<www.output<<data;data.clear();}