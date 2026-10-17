#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#define PROGMEM
#define ON 1
#define OFF 0
#define METRIC 1
#define UNITS METRIC
#define DISPLAY_SPECIAL_CHARS ON
#define AJAX_PAGE_LAZY_GET_MS 200
#define AJAX_PAGE_UPDATE_RATE_MS 1000
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 200
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 3000
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define SD_TRUE 1
#define SWITCH 1
#define ANALOG_OUTPUT 2
#define DEW_HEATER 3
#define INTERVALOMETER 4
#define COVER_SWITCH 7
#define CONTENT_LENGTH_UNKNOWN -1
#define celsiusToNativeRelative(t) (t)
#define nativeToCelsiusRelative(t) (t)
#define TEMPERATURE_UNITS_ABV "C"
using std::isnan;
char*dtostrf(double v,int width,int precision,char*out){sprintf(out,"%*.*f",width,precision,v);return out;}
void sprintF(char*out,const char*format,double value){sprintf(out,format,value);}
struct String:std::string{using std::string::string;String(){}String(std::string s):std::string(s){}void concat(const char*s){append(s);}void concat(const String&s){append(s);}bool equals(const char*s){return *this==s;}long toInt(){return atol(c_str());}float toFloat(){return atof(c_str());}};
const char*EmptyStr="";
struct Web{std::string output;std::map<std::string,String>args;void sendContentAndClear(String&s){output+=s;s.clear();}void sendContent(const char*s){output+=s;}void setContentLength(int){}void sendHeader(const char*,const char*){}void send(int,const char*,String){}String arg(const char*k){return args[k];}}www;
struct Feature{const char*name;int purpose;float values[4],voltage,current;};
Feature features[8]={{"Aux1",1,{1,0,0,0},12.1,0.1},{"Aux2",2,{128,0,0,0},12.1,0.4},{"Aux3",3,{1,1.5,4,3.2},12.1,0.8},{"Aux4",4,{0,30,5,10},NAN,NAN},{"Aux5",0,{0,0,0,0},NAN,NAN},{"Aux6",0,{0,0,0,0},NAN,NAN},{"Aux7",0,{0,0,0,0},NAN,NAN},{"Aux8",0,{0,0,0,0},NAN,NAN}};
struct Status{int auxiliaryFound=1;bool onStepFound=true;}status;
struct State{int selected=0;unsigned long lastAuxPageLoadTime=0;void updateAuxiliary(bool,bool){}void selectFeature(int n){selected=n;}const char*featureName(){return features[selected].name;}int featurePurpose(){return features[selected].purpose;}int featureValue1(){return features[selected].values[0];}float featureValue2(){return features[selected].values[1];}float featureValue3(){return features[selected].values[2];}float featureValue4(){return features[selected].values[3];}float featureVoltage(){return features[selected].voltage;}float featureCurrent(){return features[selected].current;}}state;
struct Serial{void setTimeout(int){}}SERIAL_ONSTEP;
struct Device{void serialRecvFlush(){}void commandBool(const char*){}}onStep;
int webTimeout=200;
unsigned long millis(){return 0;}
void handleNotFound(){www.output="NOT_FOUND";}
#define DISPLAY_LANGUAGE L_cn
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

// Pointer Events 统一处理触摸与鼠标；旧浏览器保留 Touch 回退。
const char html_script_motion_controls_2[] PROGMEM =
"function repeatSend(v){send(v);setTimeout(function(){send(v)},150);setTimeout(function(){send(v)},500);}"
"function stopAll(){var v=allStopValue();if(v){if(clearActive()){g++;repeatSend(v);}return;}for(var k in m)stop(k);}"
"function halt(){clearActive();g++;repeatSend(haltValue());}"
"function legacyTouch(e){return e&&e.type&&e.type.indexOf('touch')===0&&window.PointerEvent;}"
"function down(e,v,a){if(legacyTouch(e))return;if(e&&e.preventDefault)e.preventDefault();a=a||v;if(m[a])return;m[a]=1;g++;send(v);}"
"function up(e,a){if(legacyTouch(e))return;if(e&&e.preventDefault)e.preventDefault();stop(a);}"
"function pointerDown(e,v,a){if(e.button!==undefined&&e.button!==0)return;down(e,v,a);if(e.currentTarget&&e.currentTarget.setPointerCapture&&e.pointerId!==undefined){try{e.currentTarget.setPointerCapture(e.pointerId);}catch(_){}}}"
"function pointerUp(e,a){up(e,a);}"
"function windowUp(e){stopAll();}"
"function cancel(e){if(e&&e.preventDefault)e.preventDefault();return false;}"
"window.addEventListener('pointerup',windowUp);"
"window.addEventListener('pointercancel',windowUp);"
"window.addEventListener('lostpointercapture',windowUp);"
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
uint8_t timeToByte(float t) {
  float v = 10;                         // default is 1 second
  if (t <= 0.0162) v=0; else            // 0.0156 (1/64 second)        (0)
  if (t <= 0.0313) v=1; else            // 0.0313 (1/32 second)        (1)
  if (t <= 0.0625) v=2; else            // 0.0625 (1/16 second)        (2)
  if (t <= 1.0) v=2.0+t*8.0; else       // 0.125 seconds to 1 seconds  (2 to 10)
  if (t <= 10.0) v=6.0+t*4.0; else      // 0.25 seconds to 10 seconds  (10 to 46)
  if (t <= 30.0) v=26.0+t*2.0; else     // 0.5 seconds to 30 seconds   (46 to 86)
  if (t <= 120.0) v=56.0+t; else        // 1 second to 120 seconds     (86 to 176)
  if (t <= 600.0) v=168.0+t/15.0; else  // 15 seconds to 300 seconds   (176 to 208)
  if (t <= 3360.0) v=198.0+t/60.0; else // 1 minute to 56 minutes      (208 to 254)
  if (t <= 3600.0) v=255;               // 1 hour                      (255)
  if (v < 0) v=0;
  if (v > 255) v=255;
  return lround(v);
}

float byteToTime(uint8_t b) {
  float v = 1.0;                        // default is 1 second
  if (b == 0) v=0.016125; else          // 0.0156 (1/64 second)        (0)
  if (b == 1) v=0.03125; else           // 0.0313 (1/32 second)        (1)
  if (b == 2) v=0.0625; else            // 0.0625 (1/16 second)        (2)
  if (b <= 10) v=(b-2.0)/8.0; else      // 0.125 seconds to 1 seconds  (2 to 10)
  if (b <= 46) v=(b-6.0)/4.0; else      // 0.25 seconds to 10 seconds  (10 to 46)
  if (b <= 86) v=(b-26.0)/2.0; else     // 0.5 seconds to 30 seconds   (46 to 86)
  if (b <= 176) v=(b-56.0); else        // 1 second to 120 seconds     (86 to 176)
  if (b <= 208) v=(b-168.0)*15.0; else  // 15 seconds to 300 seconds   (176 to 208)
  if (b <= 254) v=(b-198.0)*60.0; else  // 1 minute to 56 minutes      (208 to 254)
  if (b == 255) v=3600.0;               // 1 hour                      (255)
  return v;
}


// -----------------------------------------------------------------------------------
// Telescope auxiliary feature related functions 





const char html_auxOnSwitch[] PROGMEM = 
"<button id='sw%d_on' class='btns_right' style='line-height: 1.2rem' onpointerdown=\"s('x%dv1','1')\" type='button'>" L_ON "</button>";
const char html_auxOffSwitch[] PROGMEM = 
"<button id='sw%d_off' class='btns_left' style='line-height: 1.2rem' onpointerdown=\"s('x%dv1','0')\" type='button'>" L_OFF "</button>\n";

const char html_auxStartStop1[] PROGMEM = "<button onpointerdown=\"s('";
const char html_auxStartStop2[] PROGMEM = "','1')\" type='button'>" L_START "</button><button onpointerdown=\"s('";
const char html_auxStartStop3[] PROGMEM = "','0')\" type='button'>" L_STOP "</button>\n";

const char html_auxAnalog[] PROGMEM ="<input type='range' min='0' max='255' value='";
#if UNITS == METRIC
  const char html_auxHeater[] PROGMEM ="<input type='range' min='-10' max='40' value='";
  #define DEW_HEATER_CONTROL_SCALE 2.0F
#else
  const char html_auxHeater[] PROGMEM ="<input type='range' min='-9' max='36' value='";
  #define DEW_HEATER_CONTROL_SCALE 1.0F
#endif
const char html_auxExposure[] PROGMEM ="<input type='range' min='0' max='255' value='";
const char html_auxDelay[] PROGMEM ="<input type='range' min='10' max='255' value='";
const char html_auxCount[] PROGMEM ="<input type='range' min='1' max='255' value='";
const char html_auxAuxE[] PROGMEM = "</div>\n";

const char html_auxEnd[] PROGMEM = "<br />\n";

// -----------------------------------------------------------------------------------
// Telescope auxiliary feature related functions 





extern void handleNotFound();
void processAuxGet();

// 通道名称来自主控配置，作为文本输出并保留完整名称
void auxiliaryName(String &data, const char *name) {
  for (const char *c = name; *c; ++c) {
    if (*c == '&') data.concat(F("&amp;"));
    else if (*c == '<') data.concat(F("&lt;"));
    else if (*c == '>') data.concat(F("&gt;"));
    else { char text[2] = {*c, 0}; data.concat(text); }
  }
}

// 每行固定为标签、操作和读数，沿用原有状态节点与请求参数
void auxiliaryRowBegin(String &data, int channel, int field, const char *label) {
  char temp[80];
  snprintf(temp, sizeof(temp), "<div class='sws-aux-row'><span class='sws-aux-label' id='aux%d-label%d'>", channel, field);
  data.concat(temp);
  data.concat(label);
  data.concat(F("</span><div class='sws-aux-controls'>"));
}

void auxiliarySliderRow(String &data, int channel, int field, const char *label, const char *control, int value, const char *reading, const char *unit) {
  char temp[160];
  auxiliaryRowBegin(data, channel, field, label);
  data.concat(FPSTR(control));
  snprintf(temp, sizeof(temp), "%d' aria-labelledby='aux%d-title aux%d-label%d' onchange=\"sz('x%dv%d',this.value)\">", value, channel, channel, field, channel, field);
  data.concat(temp);
  data.concat(F("</div><span class='sws-aux-value'>"));
  snprintf(temp, sizeof(temp), "<span id='x%dv%d'>", channel, field);
  data.concat(temp);
  data.concat(reading);
  data.concat(F("</span>"));
  data.concat(unit);
  data.concat(F("</span></div>"));
  www.sendContentAndClear(data);
}

void handleAux() {
  if (status.auxiliaryFound == SD_TRUE && servePageShell(PAGE_AUXILIARY)) return;
  char temp[240] = "";
  char temp1[80] = "";

  state.updateAuxiliary(false, true);
  if (status.auxiliaryFound != SD_TRUE) { handleNotFound(); return; }

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  processAuxGet();

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  String data = FPSTR(html_head_begin);
  data.concat(FPSTR(html_main_css_begin));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_core));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_control));
  data.concat(FPSTR(html_main_css_buttons));
  data.concat(FPSTR(html_main_css_end));
  data.concat(FPSTR(html_head_end));
  www.sendContentAndClear(data);

  // show this page
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_AUXILIARY);
  data.concat(FPSTR(html_onstep_page_begin));

  // OnStep wasn't found, show warning and info.
  if (!status.onStepFound) {
    data.concat(FPSTR(html_bad_comms_message));
    data.concat(FPSTR(html_page_and_body_end));
    www.sendContentAndClear(data);
    www.sendContent("");
    return;
  }

  // scripts
  snprintf_P(temp, sizeof(temp), html_script_ajax_get, "auxiliary-ajax-get.txt");
  data.concat(temp);
  data.concat(FPSTR(html_script_ajax_shortcuts));

  // active ajax page is: auxAjax();
  data.concat(F("<script>var ajaxPage='auxiliary-ajax.txt';</script>\n"));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_script_ajax));
  www.sendContentAndClear(data);

  // 原始 SWS 的功能类型、文字来源与参数保持不变，只调整布局
  data.concat(F("<div class='sws-device-layout sws-auxiliary'><div class='sws-aux-grid'>"));
  if (status.auxiliaryFound == SD_TRUE) {
    for (int i = 0; i < 8; i++) {
      state.selectFeature(i);
      int purpose = state.featurePurpose(), channel = i + 1;
      if (purpose <= 0) continue;
      const char *type = "Unknown";
      switch (purpose) {
        case SWITCH: type = "Switch"; break;
        case ANALOG_OUTPUT: type = "Analog Out"; break;
        case DEW_HEATER: type = "Dew Heater"; break;
        case INTERVALOMETER: type = "Intervalometer"; break;
      }
      data.concat(F("<div class='b1 sws-aux-card'><header class='sws-aux-head'><div class='sws-aux-title'><span>"));
      auxiliaryName(data, state.featureName());
      snprintf(temp, sizeof(temp), "</span><h2 id='aux%d-title'>", channel);
      data.concat(temp);
      data.concat(type);
      data.concat(F("</h2></div><div class='sws-aux-power c'>"));
      float voltageV = state.featureVoltage(), currentI = state.featureCurrent();
      if (!isnan(voltageV)) {
        snprintf(temp, sizeof(temp), "<span><span id='vout%d'>?</span> V</span>", channel);
        data.concat(temp);
      }
      if (!isnan(currentI)) {
        snprintf(temp, sizeof(temp), "<span><span id='iout%d'>?</span> A</span>", channel);
        data.concat(temp);
      }
      data.concat(F("</div></header><div class='sws-aux-body'>"));
      www.sendContentAndClear(data);

      if (purpose == SWITCH || purpose == DEW_HEATER) {
        #if UNITS == METRIC
          const char *dewLabel = "DP " L_DP_MSG " (&deg;C)";
        #else
          const char *dewLabel = "DP " L_DP_MSG " (&deg;F)";
        #endif
        auxiliaryRowBegin(data, channel, 1, purpose == SWITCH ? "" : dewLabel);
        snprintf_P(temp, sizeof(temp), html_auxOnSwitch, channel, channel); data.concat(temp);
        snprintf_P(temp, sizeof(temp), html_auxOffSwitch, channel, channel); data.concat(temp);
        data.concat(F("</div><span class='sws-aux-value'>"));
        if (purpose == DEW_HEATER) {
          dtostrf(celsiusToNativeRelative(state.featureValue4()), 3, 1, temp1);
          snprintf(temp, sizeof(temp), "&Delta;<span id='x%dv4'>%s</span>&deg;" TEMPERATURE_UNITS_ABV, channel, temp1);
          data.concat(temp);
        }
        data.concat(F("</span></div>"));
        www.sendContentAndClear(data);
      }
      if (purpose == ANALOG_OUTPUT) {
        snprintf(temp1, sizeof(temp1), "%d", (int)lround((state.featureValue1()/255.0)*100.0));
        auxiliarySliderRow(data, channel, 1, "", html_auxAnalog, state.featureValue1(), temp1, "%");
      } else if (purpose == DEW_HEATER) {
        dtostrf(celsiusToNativeRelative(state.featureValue2()), 3, 1, temp1);
        auxiliarySliderRow(data, channel, 2, L_DP_ZERO, html_auxHeater, (int)lround(celsiusToNativeRelative(state.featureValue2())*DEW_HEATER_CONTROL_SCALE), temp1, "&deg;" TEMPERATURE_UNITS_ABV);
        dtostrf(celsiusToNativeRelative(state.featureValue3()), 3, 1, temp1);
        auxiliarySliderRow(data, channel, 3, L_DP_SPAN, html_auxHeater, (int)lround(celsiusToNativeRelative(state.featureValue3())*DEW_HEATER_CONTROL_SCALE), temp1, "&deg;" TEMPERATURE_UNITS_ABV);
      } else if (purpose == INTERVALOMETER) {
        auxiliaryRowBegin(data, channel, 1, "");
        data.concat(FPSTR(html_auxStartStop1));
        snprintf(temp, sizeof(temp), "x%dv1", channel); data.concat(temp);
        data.concat(FPSTR(html_auxStartStop2)); data.concat(temp);
        data.concat(FPSTR(html_auxStartStop3));
        snprintf(temp, sizeof(temp), "</div><span class='sws-aux-value' id='x%dv1'>-</span></div>", channel); data.concat(temp);
        dtostrf(state.featureValue4(), 0, 0, temp1);
        auxiliarySliderRow(data, channel, 4, L_CAMERA_COUNT, html_auxCount, (int)state.featureValue4(), temp1, " x");
        float value = state.featureValue2();
        int digits = value < 1.0 ? 3 : value < 10.0 ? 2 : value < 30.0 ? 1 : 0;
        dtostrf(value, 0, digits, temp1);
        auxiliarySliderRow(data, channel, 2, L_CAMERA_EXPOSURE, html_auxExposure, (int)timeToByte(value), temp1, " sec");
        value = state.featureValue3();
        digits = value < 10.0 ? 2 : value < 30.0 ? 1 : 0;
        dtostrf(value, 0, digits, temp1);
        auxiliarySliderRow(data, channel, 3, L_CAMERA_DELAY, html_auxDelay, (int)timeToByte(value), temp1, " sec");
      }
      data.concat(F("</div></div>"));
      www.sendContentAndClear(data);
    }
  }
  data.concat(F("</div></div>"));
  data.concat(FPSTR(html_page_and_body_end));
  www.sendContentAndClear(data);
  www.sendContent("");
}

void auxAjaxGet() {
  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  processAuxGet();

  www.sendContent("");
}

void auxAjax() {
  String data="";
  char temp[120]="";

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/plain", String());

  // update auxiliary feature values
  if (status.auxiliaryFound == SD_TRUE) {

    for (int i = 0; i < 8; i++) {
      state.selectFeature(i);

      if (state.featurePurpose() && state.featurePurpose() != INTERVALOMETER && state.featurePurpose() != COVER_SWITCH) {

        const float voltageV = state.featureVoltage();
        const float currentI = state.featureCurrent();
        if (!isnan(voltageV)) {
          if (state.featureValue1()) {
            snprintf(temp, sizeof(temp), "vout%d|", i + 1);
            data.concat(temp);
            sprintF(temp, "%3.1f\n", voltageV);
            data.concat(temp);
          } else {
            snprintf(temp, sizeof(temp), "vout%d|0.0\n", i + 1);
            data.concat(temp);
          }
        }

        if (!isnan(currentI)) {
          if (fabs(currentI) > 0.2499F) {
            snprintf(temp, sizeof(temp), "iout%d|", i + 1);
            data.concat(temp);
            sprintF(temp, "%3.1f\n", currentI);
            data.concat(temp);
          } else {
            snprintf(temp, sizeof(temp), "iout%d|0.0\n", i + 1);
            data.concat(temp);
          }
        }
      }

      if (state.featurePurpose() == SWITCH) {
        if (state.featureValue1() != 0) {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"selected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"unselected"); data.concat(temp);
        } else {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"unselected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"selected"); data.concat(temp);
        }
      } else

      if (state.featurePurpose() == ANALOG_OUTPUT) {
        snprintf(temp, sizeof(temp), "x%dv1|%d\n",i+1,(int)lround((state.featureValue1()/255.0)*100.0)); data.concat(temp);
      } else

      if (state.featurePurpose() == DEW_HEATER) {
        char s[40];
        if (state.featureValue1() != 0) {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"selected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"unselected"); data.concat(temp);
        } else {
          snprintf(temp, sizeof(temp), "sw%d_on|%s\n",i+1,"unselected"); data.concat(temp);
          snprintf(temp, sizeof(temp), "sw%d_off|%s\n",i+1,"selected"); data.concat(temp);
        }
        dtostrf(celsiusToNativeRelative(state.featureValue2()),3,1,s); snprintf(temp, sizeof(temp), "x%dv2|%s\n",i+1,s); data.concat(temp);
        dtostrf(celsiusToNativeRelative(state.featureValue3()),3,1,s); snprintf(temp, sizeof(temp), "x%dv3|%s\n",i+1,s); data.concat(temp);
        dtostrf(celsiusToNativeRelative(state.featureValue4()),3,1,s); snprintf(temp, sizeof(temp), "x%dv4|%s\n",i+1,s); data.concat(temp);
      } else

      if (state.featurePurpose() == INTERVALOMETER) {
        char s[40];
        float v; int d;
        
        v=state.featureValue1();
        if (fabs(v) < 0.001) snprintf(temp, sizeof(temp), "x%dv1|-\n",i+1); else snprintf(temp, sizeof(temp), "x%dv1|%d\n",i+1,(int)v); data.concat(temp);
        v=state.featureValue2(); if (v < 1.0) d=3; else if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); snprintf(temp, sizeof(temp), "x%dv2|%s\n",i+1,s); data.concat(temp);
        v=state.featureValue3(); if (v < 10.0) d=2; else if (v < 30.0) d=1; else d=0;
        dtostrf(v,0,d,s); snprintf(temp, sizeof(temp), "x%dv3|%s\n",i+1,s); data.concat(temp);
        snprintf(temp, sizeof(temp), "x%dv4|%d\n",i+1,(int)state.featureValue4()); data.concat(temp);
      }
    }
  }

  www.sendContentAndClear(data);
  www.sendContent("");

  state.lastAuxPageLoadTime = millis();
}

void processAuxGet() {
  String v;
  char temp[80] = "";
  char temp1[40] = "";

  // Auxiliary Feature set Value1 to Value4
  for (char c = '1'; c <= '8'; c++) {
    state.selectFeature(c - '1');

    snprintf(temp, sizeof(temp), "x%cv1", c);
    v = www.arg(temp);
    if (!v.equals(EmptyStr)) {
      snprintf(temp, sizeof(temp), ":SXX%c,V%s#", c, v.c_str());
      onStep.commandBool(temp);
    }

    if (state.featurePurpose() == DEW_HEATER) {
      snprintf(temp, sizeof(temp), "x%cv2", c);
      v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(nativeToCelsiusRelative(v.toFloat()/DEW_HEATER_CONTROL_SCALE), 0, 1, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,Z%s#", c, temp1);
        onStep.commandBool(temp);
      }
      snprintf(temp, sizeof(temp), "x%cv3", c);
      v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(nativeToCelsiusRelative(v.toFloat()/DEW_HEATER_CONTROL_SCALE), 0, 1, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,S%s#", c, temp1);
        onStep.commandBool(temp);
      }
    } else

    if (state.featurePurpose() == INTERVALOMETER) {
      snprintf(temp, sizeof(temp), "x%cv2", c);
      v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(byteToTime(v.toInt()), 0, 3, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,E%s#", c, temp1);
        onStep.commandBool(temp);
      }
      snprintf(temp, sizeof(temp), "x%cv3", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(byteToTime(v.toInt()), 0, 2, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,D%s#", c, temp1);
        onStep.commandBool(temp);
      }
      snprintf(temp, sizeof(temp), "x%cv4", c); v = www.arg(temp);
      if (!v.equals(EmptyStr)) {
        dtostrf(v.toFloat(), 0, 0, temp1);
        snprintf(temp, sizeof(temp), ":SXX%c,C%s#", c, temp1);
        onStep.commandBool(temp);
      }
    }
  }

  state.lastAuxPageLoadTime = millis();
}

int main(){handleAux();std::cout<<www.output<<"\nPREVIEW_AJAX\n";www.output.clear();auxAjax();std::cout<<www.output<<"PREVIEW_TIMES\n";for(int i=0;i<256;i++)std::cout<<byteToTime(i)<<"\n";}