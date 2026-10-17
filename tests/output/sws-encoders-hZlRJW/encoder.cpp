#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <map>
#include <cassert>
#include <iostream>
#define ON 1
#define OFF 0
#define ENCODERS ON

#define AXIS1_ENCODER_TICKS_DEG 8000
#define AXIS2_ENCODER_TICKS_DEG 9000
#define AXIS1_ENCODER_REVERSE OFF
#define AXIS2_ENCODER_REVERSE ON
#define AXIS1_ENCODER_DIFF_LIMIT_TO 60
#define AXIS2_ENCODER_DIFF_LIMIT_TO 90
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define DISPLAY_SPECIAL_CHARS ON
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define CONTENT_LENGTH_UNKNOWN -1
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
struct String:std::string{using std::string::string;String(){}String(std::string s):std::string(s){}void concat(const char*s){append(s);}void concat(const String&s){append(s);}bool equals(const char*s){return *this==s;}float toFloat(){return atof(c_str());}};
const char*EmptyStr="";
struct Web{std::map<std::string,String>args;std::string output;String arg(const char*k){return args[k];}void sendContentAndClear(String&s){output+=s;s.clear();}void sendContent(const char*s){output+=s;}void setContentLength(int){}void sendHeader(const char*,const char*){}void send(int,const char*,String){}}www;
struct Axis{float ticksPerDeg;int reverse;long diffTo;};
struct Settings{Axis axis1={8000,OFF,60},axis2={9000,ON,90};bool autoSync=false;};
struct Encoders{Settings settings;int from=0,to=0,origin=0;void syncFromOnStep(){from++;}void syncToOnStep(){to++;}void originFromOnStep(){origin++;}}encoders;
struct Storage{int writes=0;Storage&kv(){return *this;}template<class T>void put(const char*,const T&){writes++;}}storage;
Storage&nv(){return storage;}
struct Convert{bool atoi2(char*s,int16_t*out){*out=atoi(s);return true;}void stripNumericStr(char*){}}convert;
char*dtostrf(double v,int width,int precision,char*out){sprintf(out,"%*.*f",width,precision,v);return out;}
struct State{const char*encAngleAxis1Str="120.0",*encAngleAxis2Str="45.0",*angleAxis1Str="120.0",*angleAxis2Str="45.0",*diffAngleAxis1Str="0",*diffAngleAxis2Str="0";unsigned long lastMountPageLoadTime=0;void updateMount(bool){}}state;
struct Status{bool onStepFound=true;}status;
struct Serial{void setTimeout(int){}}SERIAL_ONSTEP;
struct Device{void serialRecvFlush(){}}onStep;
int webTimeout=200;unsigned long millis(){return 0;}
String keyValueString(String key,String value){return key+"|"+value+"\n";}
String keyValueToggleBoolSelected(String on,String off,bool b){return on+"|"+(b?"selected":"unselected")+"\n"+off+"|"+(b?"unselected":"selected")+"\n";}
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
  "</section><details class='sws-more' open><summary>" L_ALIGN " / " L_PAGE_PEC "</summary><div class='sws-settings-grid sws-advanced-grid'>";

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




const char html_bad_comms_message[] PROGMEM =
// 通信异常使用独立提示区域，避免错误内容破坏卡片布局
"<section class='sws-notice'><h2>" L_DOWN_TITLE "</h2><p>"
L_DOWN_MESSAGE1 L_DOWN_MESSAGE2 "</p><ul>"
" <li>" L_DOWN_MESSAGE3 "</li><br />"
" <li>" L_DOWN_MESSAGE4 "</li><br />"
" <li>" L_DOWN_MESSAGE5 "</li><br />"
" <li>" L_DOWN_MESSAGE6 "</li><br />"
" <li>" L_DOWN_MESSAGE7 "</li><br />"
#if DISPLAY_RESET_CONTROLS != OFF
  "<br /><form method='get' action='/configuration.htm'>"
  "<button name='advanced' type='submit' value='reset'" SWS_CONFIRM_ATTRIBUTE " onclick=\"return confirm(this.dataset.confirm);\" >" L_RESET "</button></form>\n"
#endif
"</ul></section>\n";




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
"var m={},q=[],b=0,g=0;"
"function send(v){q.push(v);run();}"
"function run(){if(b||!q.length)return;b=1;window.swsPendingCommands=(window.swsPendingCommands||0)+1;var v=q.shift(),x=new XMLHttpRequest(),d=0;function f(){if(d)return;d=1;window.swsPendingCommands--;b=0;run();}x.onreadystatechange=function(){if(x.readyState==4)f();};x.onerror=x.ontimeout=f;x.timeout=1000;x.open('GET',o.p+'?'+o.k+'='+encodeURIComponent(v)+'&x='+new Date().getTime(),true);x.send();}"
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
  "var element=document.getElementById('datetime'); if (!element) return;\n"
  "var value = \n"
  "(now.getUTCMonth()+1)+'/'+pad(now.getUTCDate().toString(),2)+'/'+pad(now.getUTCFullYear().toString().substr(-2),2)+' '+\n"
  "pad(now.getUTCHours().toString(),2)+':'+pad(now.getUTCMinutes().toString(),2)+':'+pad(now.getUTCSeconds().toString(),2);\n"
  "if (element.textContent!==value) element.textContent=value;\n"
"}\n"
R"JS(
// 按请求完成时间安排下一次刷新，慢连接只保留一个状态请求。
var auto1Tick=-1;
var autoTimer=null, autoRequest=null, autoFastUntil=0, autoFailures=0, autoStopped=false;
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
  if (autoRequest) {
    var request=autoRequest;
    autoRequest=null;
    request.abort();
  }
}
window.swsPausePolling=function() { autoStopped=true; autoPause(); };
function autoRun() {
  if (autoStopped || document.hidden || autoRequest) return;
  // 命令正在发送时让出下一次刷新机会，不改变命令的参数或发送顺序。
  if (window.swsPendingCommands) { autoSchedule(autoRate()); return; }
  var request=new XMLHttpRequest();
  autoRequest=request;
  function finish() {
    if (autoRequest!==request) return;
    autoRequest=null;
    if (request.status===200) {
      autoFailures=0;
      pageReady(ajaxPage).call(request);
    } else autoFailures=Math.min(autoFailures+1,6);
    autoSchedule(autoFailures ? Math.min(30000,autoRate()*Math.pow(2,autoFailures)) : autoRate());
  }
  request.onreadystatechange=function() { if (request.readyState===4) finish(); };
  request.onerror=request.ontimeout=finish;
  request.timeout=5000;
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
)JS" "</script>\n";

// SWS standard page





#define PAGE_CONTROLLER 0
#define PAGE_MOUNT 1
#define PAGE_ROTATOR 2
#define PAGE_FOCUSER 3
#define PAGE_AUXILIARY 4
#define PAGE_ENCODERS 5
#define PAGE_NETWORK 6

void pageHeader(int selected);

const char html_onstep_header_begin[] PROGMEM = "<div class='t'><table width='100%%'><tr><td><b><font size='5'>";
const char html_onstep_header_title[] PROGMEM = "</font></b></td><td align='right'><b>Smart Web Server ";
const char html_onstep_header_links[] PROGMEM = ")</b></td></tr></table>";
const char html_onstep_header_end[] PROGMEM = "<span id='sws-sidebar-label' hidden>" L_SIDEBAR_NOTE "</span></div>\n";
const char html_onstep_page_begin[] PROGMEM = "<div class='b'>\n";

const char html_links_idx_begin[] PROGMEM = "<a href='/index.htm' ";
const char html_links_idx_end[] PROGMEM = ">" L_CONTROLLER "</a>";

const char html_links_mnt_begin[] PROGMEM = "<a href='/mount.htm' ";
const char html_links_mnt_end[] PROGMEM = ">" L_PAGE_MOUNT "</a>";

const char html_links_rot_begin[] PROGMEM = "<a href='/rotator.htm' ";
const char html_links_rot_end[] PROGMEM = ">" L_ROTATOR "</a>";

const char html_links_foc_begin[] PROGMEM = "<a href='/focuser.htm' ";
const char html_links_foc_end[] PROGMEM = ">" L_FOCUSER "</a>";

const char html_links_aux_begin[] PROGMEM = "<a href='/auxiliary.htm' ";
const char html_links_aux_end[] PROGMEM = ">" L_PAGE_AUXILIARY "</a>";

const char html_links_enc_begin[] PROGMEM = "<a href='/enc.htm' ";
const char html_links_enc_end[] PROGMEM = ">" L_PAGE_ENCODERS "</a>";

const char html_links_net_begin[] PROGMEM = "<a href='/net.htm' ";
const char html_links_net_end[] PROGMEM = ">" L_PAGE_NETWORK "</a>";

const char html_links_selected[] PROGMEM =" style='background-color: " COLOR_LINK_SELECTED_BACKGROUND ";' ";

void pageHeader(int){}bool servePageShell(int){return false;}
// -----------------------------------------------------------------------------------
// Axis tile





extern void encAxisTile(int axis, String &data);
extern void encAxisTileAjax(String &data);
extern void encAxisTileGet();

const char html_encoderAxisBegin[] PROGMEM =
  "<div class='b1 sws-encoder-axis'><div class='sws-device-card-head'><h2>%s</h2></div>";

const char html_encoderAngle[] PROGMEM =
  "<div class='sws-encoder-metric'><span>" L_ENCODER "</span><strong id='enc_a%d' class='c'>%s</strong></div>";

const char html_mountAngle[] PROGMEM =
  "<div class='sws-encoder-metric'><span>OnStep</span><strong id='raw_a%d' class='c'>%s</strong></div>"
  "<div class='sws-encoder-metric'><span>&Delta;</span><strong id='diff_a%d' class='c'>%s</strong></div>";

const char html_indexDriverStatus[] PROGMEM =
  L_DRIVER " " L_STATUS ": <span id='dvr_stat%d' class='c'>%s</span><br />";

const char html_encAxisValueTpd[] PROGMEM =
  "<div><dt>" L_ENC_SET_TPD "</dt><dd class='c'>%s</dd></div>";

const char html_encAxisValueReverse[] PROGMEM =
  "<div><dt>" L_ADV_SET_REV "</dt><dd class='c'>%s</dd></div>";

const char html_encAxisValueSyncThreshold[] PROGMEM =
  "<div><dt>" L_ENC_SYNC_THLD "</dt><dd class='c'>%ld arc-sec</dd></div>";

const char html_encFormBegin[] PROGMEM =
  "<form method='get' action='/enc.htm'>\n";

const char html_encFormEnd[] PROGMEM =
  "</div></form>\n";

const char html_encAxisTpd[] PROGMEM =
  "<label class='sws-encoder-field'><span>" L_ENC_SET_TPD "</span>"
  "<input name='a%dcpd' value='%s' type='number' min='%d' max='%ld' step='any'></label>\n";

const char html_encAxisReverse[] PROGMEM =
   "<label class='sws-encoder-field'><span>" L_ADV_SET_REV ", " L_ADV_BOOL "</span>"
  "<input name='a%drev' value='%d' type='number' min='0' max='1' step='1'></label>\n";

const char html_encAxisSyncThreshold[] PROGMEM =
  "<label class='sws-encoder-field'><span>" L_ENC_SYNC_THLD ", " L_ENC_SYNC_THLD_UNITS "</span>"
  "<input name='a%dths' value='%ld' type='number' min='0' max='9999'></label>\n";

const char html_encUpload[] PROGMEM =
  "<button type='submit'>" L_UPLOAD "</button>\n";

const char html_encAxisRevert[] PROGMEM =
  "<button class='sws-secondary' name='revert' value='%d' type='submit'>" L_REVERT "</button>\n";

// -----------------------------------------------------------------------------------
// Encoder Sync tile





extern void syncTile(String &data);
extern void syncTileAjax(String &data);
extern void syncTileGet();

const char html_syncOnStepNow[] PROGMEM =
  "<button onpointerdown=\"s('enc','o2e')\" type='button'>" L_ENC_SYNC_TO_ENC "</button>";

#ifdef ENC_ABSOLUTE
  const char html_zeroEncodersNow[] PROGMEM =
    "<button class='sws-secondary sws-encoder-origin'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('enc','zro')\" type='button'>" L_ENC_ORIGIN_TO_ONS "</button>";
#endif

const char html_syncEncodersNow[] PROGMEM =
  "<button onpointerdown=\"s('enc','e2o')\" type='button'>" L_ENC_SYNC_TO_ONS "</button>";

const char html_syncAuto[] PROGMEM =
  "<div class='sws-encoder-auto'><span>" L_ENC_AUTO_SYNC "</span><div class='sws-encoder-toggle'>"
  "<button id='enc_as_on' onpointerdown=\"s('enc','on')\" type='button'>" L_ON "</button>"
  "<button id='enc_as_off' onpointerdown=\"s('enc','off')\" type='button'>" L_OFF "</button>"
  "</div></div>";

// -----------------------------------------------------------------------------------
// Encoders





#if ENCODERS == ON


#endif

// -----------------------------------------------------------------------------------
// Axis tile





#if ENCODERS == ON

// create the related webpage tile
void encAxisTile(int axis, String &data)
{
  char temp[512] = "";
  char ticksPerDegStr[32] = "";
  float ticksPerDeg = 0;
  bool reverse = false;
  long diffTo = 0;

  if (axis == 0)
  {
    ticksPerDeg = encoders.settings.axis1.ticksPerDeg;
    reverse = encoders.settings.axis1.reverse == ON;
    diffTo = encoders.settings.axis1.diffTo;
    snprintf_P(temp, sizeof(temp), html_encoderAxisBegin, "Axis1 RA/Azm");
  }
  else if (axis == 1)
  {
    ticksPerDeg = encoders.settings.axis2.ticksPerDeg;
    reverse = encoders.settings.axis2.reverse == ON;
    diffTo = encoders.settings.axis2.diffTo;
    snprintf_P(temp, sizeof(temp), html_encoderAxisBegin, "Axis2 Dec/Alt");
  }
  data.concat(temp);
  data.concat(F("<div class='sws-encoder-readings'>"));

  if (axis == 0)
  {
    snprintf_P(temp, sizeof(temp), html_encoderAngle, 1, state.encAngleAxis1Str);
    data.concat(temp);
    snprintf_P(temp, sizeof(temp), html_mountAngle, 1, state.angleAxis1Str, 1, state.diffAngleAxis1Str);
    data.concat(temp);
  }
  else if (axis == 1)
  {
    snprintf_P(temp, sizeof(temp), html_encoderAngle, 2, state.encAngleAxis2Str);
    data.concat(temp);
    snprintf_P(temp, sizeof(temp), html_mountAngle, 2, state.angleAxis2Str, 2, state.diffAngleAxis2Str);
    data.concat(temp);
  }

  data.concat(F("</div><dl class='sws-encoder-values'>"));

  dtostrf(ticksPerDeg, 1, 3, ticksPerDegStr);
  convert.stripNumericStr(ticksPerDegStr);

  snprintf_P(temp, sizeof(temp), html_encAxisValueTpd, ticksPerDegStr);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_encAxisValueReverse, reverse ? L_ON : L_OFF);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_encAxisValueSyncThreshold, diffTo);
  data.concat(temp);
  www.sendContentAndClear(data);

  data.concat(F("</dl>"));

  snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);
  data.concat(FPSTR(html_encFormBegin));

  snprintf_P(temp, sizeof(temp), html_encAxisTpd, axis + 1, ticksPerDegStr, 1, 100000L);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_encAxisReverse, axis + 1, reverse ? 1 : 0);
  data.concat(temp);
  snprintf_P(temp, sizeof(temp), html_encAxisSyncThreshold, axis + 1, diffTo);
  data.concat(temp);
  www.sendContentAndClear(data);

  data.concat(F("<div class='sws-encoder-form-actions'>"));
  data.concat(FPSTR(html_encUpload));
  snprintf_P(temp, sizeof(temp), html_encAxisRevert, axis + 1);
  data.concat(temp);

  data.concat(FPSTR(html_encFormEnd));
  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void encAxisTileAjax(String &data) {
  data.concat(keyValueString("diff_a1", state.diffAngleAxis1Str));
  data.concat(keyValueString("enc_a1", state.encAngleAxis1Str));
  data.concat(keyValueString("raw_a1", state.angleAxis1Str));
  data.concat(keyValueString("diff_a2", state.diffAngleAxis2Str));
  data.concat(keyValueString("enc_a2", state.encAngleAxis2Str));
  data.concat(keyValueString("raw_a2", state.angleAxis2Str));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void encAxisTileGet() {
  String v;
  bool updateNV = false;

  // Max. limits
  v = www.arg("a1ths");
  if (!v.equals(EmptyStr)) {
    int16_t i;
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 9999)) {
      encoders.settings.axis1.diffTo = i;
      updateNV = true;
    }
  }
  v = www.arg("a2ths");
  if (!v.equals(EmptyStr)) {
    int16_t i;
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 9999)) {
      encoders.settings.axis2.diffTo = i;
      updateNV = true;
    }
  }

  // Counts per degree
  v = www.arg("a1cpd");
  if (!v.equals(EmptyStr)) {
    double d = v.toFloat();
    if (d >= 1.0 && d <= 1000000.0) {
      encoders.settings.axis1.ticksPerDeg = d;
      updateNV = true;
    }
  }
  v = www.arg("a2cpd");
  if (!v.equals(EmptyStr)) {
    double d = v.toFloat();
    if (d >= 1.0 && d <= 1000000.0) {
      encoders.settings.axis2.ticksPerDeg = d;
      updateNV = true;
    }
  }

  v = www.arg("a1rev");
  if (!v.equals(EmptyStr)) {
    if (v.equals("0")) {
      encoders.settings.axis1.reverse = OFF;
      updateNV = true;
    }
    if (v.equals("1")) {
      encoders.settings.axis1.reverse = ON;
      updateNV = true;
    }
  }

  v = www.arg("a2rev");
  if (!v.equals(EmptyStr)) {
    if (v.equals("0")) {
      encoders.settings.axis2.reverse = OFF;
      updateNV = true;
    }
    if (v.equals("1")) {
      encoders.settings.axis2.reverse = ON;
      updateNV = true;
    }
  }

  v = www.arg("revert");
  if (!v.equals(EmptyStr)) {
    if (v.equals("1")) {
      encoders.settings.axis1.ticksPerDeg = AXIS1_ENCODER_TICKS_DEG;
      encoders.settings.axis1.reverse = AXIS1_ENCODER_REVERSE;
      encoders.settings.axis1.diffTo = AXIS1_ENCODER_DIFF_LIMIT_TO;
      updateNV = true;
    }
    if (v.equals("2")) {
      encoders.settings.axis2.ticksPerDeg = AXIS2_ENCODER_TICKS_DEG;
      encoders.settings.axis2.reverse = AXIS2_ENCODER_REVERSE;
      encoders.settings.axis2.diffTo = AXIS2_ENCODER_DIFF_LIMIT_TO;
      updateNV = true;
    }
  }

  if (updateNV) {
    nv().kv().put("ENCODER_SETTINGS", encoders.settings);
  }
}

#endif

// -----------------------------------------------------------------------------------
// Encoder Sync tile





#if ENCODERS == ON

// create the related webpage tile
void syncTile(String &data)
{
  data.concat(F("<div class='b1 sws-encoder-sync'><div class='sws-device-card-head'><h2>" L_ENC_SYNC_CONTROL "</h2></div><div class='sws-encoder-sync-grid'><div class='sws-encoder-sync-actions'>"));
  data.concat(FPSTR(html_syncOnStepNow));
  data.concat(FPSTR(html_syncEncodersNow));
  #ifdef ENC_ABSOLUTE
    data.concat(FPSTR(html_zeroEncodersNow));
  #endif
  data.concat(F("</div>"));
  data.concat(FPSTR(html_syncAuto));
  data.concat(F("</div>"));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void syncTileAjax(String &data)
{
  data.concat(keyValueToggleBoolSelected("enc_as_on", "enc_as_off", encoders.settings.autoSync));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void syncTileGet()
{
  String v;

  v = www.arg("enc");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("o2e")) encoders.syncFromOnStep();
    if (v.equals("e2o")) encoders.syncToOnStep();
    #ifdef ENC_ABSOLUTE
      if (v.equals("zro")) encoders.originFromOnStep();
    #endif
    if (v.equals("on")) encoders.settings.autoSync = true;
    if (v.equals("off")) encoders.settings.autoSync = false;
    nv().kv().put("ENCODER_SETTINGS", encoders.settings);
  }
}

#endif

// -----------------------------------------------------------------------------------
// Encoders





#if ENCODERS == ON

void processEncodersGet();

void handleEncoders()
{
  if (servePageShell(PAGE_ENCODERS)) return;
  char temp[400] = "";

  state.updateMount(true);

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  processEncodersGet();

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  String data = FPSTR(html_head_begin);

  // send a standard http response header
  data.concat(FPSTR(html_main_css_begin));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_core));
  data.concat(FPSTR(html_main_css_control));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_buttons));
  data.concat(FPSTR(html_main_css_collapse));
  data.concat(FPSTR(html_main_css_end));
  data.concat(FPSTR(html_head_end));
  www.sendContentAndClear(data);

  // show this page
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_ENCODERS);
  data.concat(FPSTR(html_onstep_page_begin));

  // OnStep wasn't found, show warning and info.
  if (!status.onStepFound)
  {
    data.concat(FPSTR(html_bad_comms_message));
    data.concat(FPSTR(html_page_and_body_end));
    www.sendContentAndClear(data);
    www.sendContent("");
    return;
  }

  // scripts
  snprintf_P(temp, sizeof(temp), html_script_ajax_get, "enc-ajax-get.txt");
  data.concat(temp);

  data.concat(F("<script>var ajaxPage='enc-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // 同步状态与两轴参数分区排列，避免窄屏并排挤压
  data.concat(FPSTR(html_layout_encoders_begin));
  syncTile(data);
  data.concat(FPSTR(html_layout_encoders_settings));
  encAxisTile(0, data);
  encAxisTile(1, data);
  data.concat(FPSTR(html_layout_encoders_end));

  data.concat(FPSTR(html_page_and_body_end));

  data.concat(FPSTR(html_script_collapsible));

  www.sendContentAndClear(data);
  www.sendContent("");
}

void encAjaxGet()
{
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processEncodersGet();

  www.sendContent("");
}

void encAjax()
{
  String data = "";

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  syncTileAjax(data);
  encAxisTileAjax(data);
  
  www.sendContent("");

  state.lastMountPageLoadTime = millis();
}

void processEncodersGet()
{
  String v;

  syncTileGet();
  encAxisTileGet();

  state.lastMountPageLoadTime = millis();
}
#endif

int main(){handleEncoders();std::cout<<www.output;
 www.args={{"a2cpd","12345"},{"a2rev","0"},{"a2ths","120"}};encAxisTileGet();assert(encoders.settings.axis2.ticksPerDeg==12345);assert(encoders.settings.axis2.reverse==OFF);assert(encoders.settings.axis2.diffTo==120);assert(encoders.settings.axis1.ticksPerDeg==8000);
 www.args={{"revert","2"}};encAxisTileGet();assert(encoders.settings.axis2.ticksPerDeg==9000);assert(encoders.settings.axis2.reverse==ON);assert(encoders.settings.axis2.diffTo==90);
 www.args={{"enc","o2e"}};syncTileGet();assert(encoders.from==1);www.args={{"enc","e2o"}};syncTileGet();assert(encoders.to==1);www.args={{"enc","on"}};syncTileGet();assert(encoders.settings.autoSync);www.args={{"enc","off"}};syncTileGet();assert(!encoders.settings.autoSync);
 }