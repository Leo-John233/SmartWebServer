// 模拟设备响应，实际页面生成与提交代码保持原样
#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define strcpy_P strcpy
#define strncpy_P strncpy
#define ON 1
#define OFF 0
#define DRIVE_CONFIGURATION ON
#define DISPLAY_RESET_CONTROLS OFF
#define HOME_OFFSET_RANGE_AXIS1 3600
#define HOME_OFFSET_RANGE_AXIS2 3600
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define SD_TRUE 1
struct String : std::string {
  using std::string::string;
  String(const std::string &s): std::string(s) {}
  void concat(const char *s) { append(s); }
  bool equals(const char *s) const { return *this == s; }
  int toInt() const { return atoi(c_str()); }
};
const char *EmptyStr="";
struct Web {
  std::string output;
  std::map<std::string,String> args;
  void sendContentAndClear(String &s) { output += s; s.clear(); }
  String arg(const char *key) { return args[key]; }
} www;
struct Status {
  int mountFound=1,rotatorFound=1;
  bool focuserPresent[6]={true,true,true,true,true,true};
  int getVersionMajor() {return 10;}
  int getVersionMinor() {return 28;}
} status;
struct State {char driverStatusStr[9][16]={{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"},{"OK"}};} state;
struct Device {
  std::vector<std::string> sent;
  bool command(const char *s,char *out) {
    int axis=0,n=0;
    if(strstr(s,",M#")){strcpy(out,"TMC2209");return true;}
    sscanf(s,":GXA%d,%d#",&axis,&n);
    if(n==0){strcpy(out,"26");return true;}
    int type=n==7?1:n==8||n==9?9:n==10||n==11?10:n>=16?5:3;
    snprintf(out,80,"1,0,256,%d,$%d",type,n);return true;
  }
  bool commandBool(const char *s){sent.push_back(s);return true;}
} onStep;
struct Convert {void stripNumericStr(char*,bool){}} convert;
void dtostrf(double value,int,int precision,char *out){sprintf(out,"%.*f",precision,value);}
void sstrcpyex(char *out,const char *in,size_t n){strncpy(out,in,n);out[n]=0;}
const char *keyValueString(const char*,const char*){return "";}

#define DISPLAY_LANGUAGE L_us
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

// -----------------------------------------------------------------------------------
// Ambient tile






extern bool axisTile(int axis, String &data);
extern void axisTileAjax(int axis, String &data);
extern void axisTileGet();

const char html_indexDriverStatus[] PROGMEM = L_DRIVER " " L_STATUS ": <span id='dvr_stat%d' class='c'>%s</span><br />";

const char html_configAdvanced[] PROGMEM =
"<hr>" L_ADV_SET_TITLE " (<i>" L_ADV_SET_HEADER_MSG "</i>)<br /><br />";

const char html_configMountType[] PROGMEM =
" <input style='width: 7em;' value='%d' type='number' name='mountt' min='1' max='3' step='1'>&nbsp; " L_ADV_MOUNT_TYPE "<br /><br />\n";

#if DRIVE_CONFIGURATION == ON
  // 参数名称与输入框同排展示，标签沿用语言文件中的原文
  const char html_configAxisFloat[] PROGMEM =
  "<label class='sws-parameter'><input style='width: 10em;' value='%s' type='number' name='a%dp%dvalue' min='%ld' max='%ld' step='any'><span>%s</span></label>\n";
  const char html_configAxisInt[] PROGMEM =
  "<label class='sws-parameter'><input style='width: 10em;' value='%ld' type='number' name='a%dp%dvalue' min='%ld' max='%ld' step='1'><span>%s</span></label>\n";
  const char html_configAxisSelectStart[] PROGMEM =
  "<label class='sws-parameter'><select size='1' style='width: 10.6em; margin: 3px; text-align-last: center;' name='a%dp%dvalue' >";
  const char html_configAxisSelectOption[] PROGMEM =
  "<option value='%d'>%s</option>";
  const char html_configAxisSelectOptionSelected[] PROGMEM =
  "<option value='%d' selected>%s</option>";
  const char html_configAxisSelectEnd[] PROGMEM =
  "</select><span>%s</span></label>\n";

  const char html_configAxisRevert[] PROGMEM =
  "<button name='revert' value='%d' type='submit'>" L_REVERT "</button>";

  #define AXPN_COUNT 26
  const char html_axpn_1[] PROGMEM = L_AXPN_STEPS_PER_DEG;
  const char html_axpn_2[] PROGMEM = L_AXPN_LIMIT_DEGS_MIN;
  const char html_axpn_3[] PROGMEM = L_AXPN_LIMIT_DEGS_MAX;
  const char html_axpn_4[] PROGMEM = L_AXPN_STEPS_PER_UM;
  const char html_axpn_5[] PROGMEM = L_AXPN_LIMIT_UM_MIN;
  const char html_axpn_6[] PROGMEM = L_AXPN_LIMIT_UM_MAX;
  const char html_axpn_7[] PROGMEM = L_AXPN_REVERSE;
  const char html_axpn_8[] PROGMEM = L_AXPN_MICROSTEPS;
  const char html_axpn_9[] PROGMEM = L_AXPN_MICROSTEPS_GOTO;
  const char html_axpn_10[] PROGMEM = L_AXPN_DECAY_MODE;
  const char html_axpn_11[] PROGMEM = L_AXPN_DECAY_MODE_GOTO;
  const char html_axpn_12[] PROGMEM = L_AXPN_CURRENT_HOLD;
  const char html_axpn_13[] PROGMEM = L_AXPN_CURRENT_RUN;
  const char html_axpn_14[] PROGMEM = L_AXPN_CURRENT_GOTO;
  const char html_axpn_15[] PROGMEM = L_AXPN_INTERPOLATE;
  const char html_axpn_16[] PROGMEM = L_AXPN_PID_P;
  const char html_axpn_17[] PROGMEM = L_AXPN_PID_I;
  const char html_axpn_18[] PROGMEM = L_AXPN_PID_D;
  const char html_axpn_19[] PROGMEM = L_AXPN_PID_SLEWING_P;
  const char html_axpn_20[] PROGMEM = L_AXPN_PID_SLEWING_I;
  const char html_axpn_21[] PROGMEM = L_AXPN_PID_SLEWING_D;
  const char html_axpn_22[] PROGMEM = L_AXPN_RADS_PER_COUNT;
  const char html_axpn_23[] PROGMEM = L_AXPN_STEPS_TO_COUNTS;
  const char html_axpn_24[] PROGMEM = L_AXPN_MAX_ACCEL;
  const char html_axpn_25[] PROGMEM = L_AXPN_MIN_PWR;
  const char html_axpn_26[] PROGMEM = L_AXPN_MAX_PWR;
#endif

// -----------------------------------------------------------------------------------
// Axis tile





bool decodeParameter(char* s, double *value, long *min, long *max, int *type, char *name);

// create the related webpage tile
bool axisTile(int axis, String &data)
{
  bool success = false;
  char temp[256] = "";
  char temp1[80] = "";
  double value;
  long min, max;
  int type;
  // 欧洲语言的 PID 标签包含 HTML 标记，保留后缀空间并限制复制长度
  char name[80];

  if (axis <= 1 && status.mountFound != SD_TRUE) return success;
  if (axis == 2 && status.rotatorFound != SD_TRUE) return success;
  if (axis >= 3 && !status.focuserPresent[axis - 3]) return success;

  switch (axis) {
    case 0: snprintf(temp1, sizeof(temp1), "Axis%d RA/Azm", 1); break;
    case 1: snprintf(temp1, sizeof(temp1), "Axis%d Dec/Alt", 2); break;
    case 2: snprintf(temp1, sizeof(temp1), "Axis%d " L_ROTATOR, 3); break;
    case 3: case 4: case 5: case 6: case 7: case 8:
      snprintf(temp1, sizeof(temp1), "Axis%d " L_FOCUSER "%d", axis + 1, axis - 2);
    break;
  }
  snprintf_P(temp, sizeof(temp), html_tile_text_beg, "22em", "11em", temp1);
  data.concat(temp);
  data.concat(F("<br /><hr>"));
  if (state.driverStatusStr[axis][0] == '?') strcpy(temp1, L_UNKNOWN); else strcpy(temp1, state.driverStatusStr[axis]);
  snprintf_P(temp, sizeof(temp), html_indexDriverStatus, axis, temp1);
  data.concat(temp);

  data.concat(F("<hr>"));
  www.sendContentAndClear(data);

  #if DRIVE_CONFIGURATION == ON
    if (status.getVersionMajor() > 10 || (status.getVersionMajor() == 10 && status.getVersionMinor() >= 26)) {

      snprintf_P(temp, sizeof(temp), html_collapsable_beg, L_SETTINGS "...");
      data.concat(temp);

      snprintf_P(temp, sizeof(temp), html_form_begin, "index.htm");
      data.concat(temp);
      www.sendContentAndClear(data);

      // get axis parameter count
      snprintf(temp, sizeof(temp), ":GXA%d,0#", axis + 1);
      if (onStep.command(temp, temp1)) {
        int parameterCount = atoi(temp1);

        // show controls
        for (int parameterNumber = 1; parameterNumber <= parameterCount; parameterNumber++) {
          snprintf(temp, sizeof(temp), ":GXA%d,%d#", axis + 1, parameterNumber);
          if (onStep.command(temp, temp1)) {
            decodeParameter(temp1, &value, &min, &max, &type, name);

            // name lookup, for locale specific strings
            if (name[0] == '$') {
              int i = atoi(&name[1]);
              if (i >= 1 && i <= AXPN_COUNT) {
                switch (i) {
                  case 1: strncpy_P(name, html_axpn_1, sizeof(name) - 11); break;
                  case 2: strncpy_P(name, html_axpn_2, sizeof(name) - 11); break;
                  case 3: strncpy_P(name, html_axpn_3, sizeof(name) - 11); break;
                  case 4: strncpy_P(name, html_axpn_4, sizeof(name) - 11); break;
                  case 5: strncpy_P(name, html_axpn_5, sizeof(name) - 11); break;
                  case 6: strncpy_P(name, html_axpn_6, sizeof(name) - 11); break;
                  case 7: strncpy_P(name, html_axpn_7, sizeof(name) - 11); break;
                  case 8: strncpy_P(name, html_axpn_8, sizeof(name) - 11); break;
                  case 9: strncpy_P(name, html_axpn_9, sizeof(name) - 11); break;
                  case 10: strncpy_P(name, html_axpn_10, sizeof(name) - 11); break;
                  case 11: strncpy_P(name, html_axpn_11, sizeof(name) - 11); break;
                  case 12: strncpy_P(name, html_axpn_12, sizeof(name) - 11); break;
                  case 13: strncpy_P(name, html_axpn_13, sizeof(name) - 11); break;
                  case 14: strncpy_P(name, html_axpn_14, sizeof(name) - 11); break;
                  case 15: strncpy_P(name, html_axpn_15, sizeof(name) - 11); break;
                  case 16: strncpy_P(name, html_axpn_16, sizeof(name) - 11); break;
                  case 17: strncpy_P(name, html_axpn_17, sizeof(name) - 11); break;
                  case 18: strncpy_P(name, html_axpn_18, sizeof(name) - 11); break;
                  case 19: strncpy_P(name, html_axpn_19, sizeof(name) - 11); break;
                  case 20: strncpy_P(name, html_axpn_20, sizeof(name) - 11); break;
                  case 21: strncpy_P(name, html_axpn_21, sizeof(name) - 11); break;
                  case 22: strncpy_P(name, html_axpn_22, sizeof(name) - 11); break;
                  case 23: strncpy_P(name, html_axpn_23, sizeof(name) - 11); break;
                  case 24: strncpy_P(name, html_axpn_24, sizeof(name) - 11); break;
                  case 25: strncpy_P(name, html_axpn_25, sizeof(name) - 11); break;
                  case 26: strncpy_P(name, html_axpn_26, sizeof(name) - 11); break;
                }
              }

              name[sizeof(name) - 11] = 0;

              // if element 7 (reverse) is present, which it always is, show the Motor/Driver identification string
              if (i == 7) {
                data.concat("<br />" L_ADV_MOTOR ": ");
                snprintf(temp, sizeof(temp), ":GXA%d,M#", axis + 1);
                if (onStep.command(temp, temp1)) {
                  data.concat(temp1);
                } else {
                  data.concat("?");
                }
                data.concat("<br />");
              }
            }

            if (type == 2 || type == 4 || type == 6) strcat(name, " (<i>i</i>)");

            long valueInt = lround(value);
            char valueStr[24];
            dtostrf(value, 1, 3, valueStr);
            convert.stripNumericStr(valueStr, true);

            switch (type) {
              // AXP_BOOLEAN, AXP_BOOLEAN_IMMEDIATE
              case 1: case 2: {
                snprintf_P(temp, sizeof(temp), html_configAxisSelectStart, axis + 1, parameterNumber);
                data.concat(temp);
                int selection[4] = {-1, -2, 0, 1};
                const char *selectionName[5] = {L_OFF, L_ON, L_OFF, L_ON};
                for (int i = 0; i < 4; i++) {
                  if (selection[i] >= min && selection[i] <= max) {
                    snprintf_P(temp, sizeof(temp), valueInt == selection[i] ? html_configAxisSelectOptionSelected : html_configAxisSelectOption, selection[i], selectionName[i]);
                    data.concat(temp);
                    www.sendContentAndClear(data);
                  }
                }
                snprintf_P(temp, sizeof(temp), html_configAxisSelectEnd, name);
                data.concat(temp);
              } break;

              // AXP_INTEGER, AXP_INTEGER_IMMEDIATE
              case 3: case 4:
                snprintf_P(temp, sizeof(temp), html_configAxisInt, valueInt, axis + 1, parameterNumber, min, max, name);
                data.concat(temp);
              break;

              // AXP_FLOAT, AXP_FLOAT_IMMEDIATE
              case 5: case 6:
                snprintf_P(temp, sizeof(temp), html_configAxisFloat, valueStr, axis + 1, parameterNumber, min, max, name);
                data.concat(temp);
              break;

              // AXP_POW2
              case 9: {
                snprintf_P(temp, sizeof(temp), html_configAxisSelectStart, axis + 1, parameterNumber);
                data.concat(temp);
                int selection[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
                const char *selectionName[9] = {"1", "2", "4", "8", "16", "32", "64", "128", "256"};
                for (int i = 0; i < 9; i++) {
                  if (selection[i] >= min && selection[i] <= max) {
                    snprintf_P(temp, sizeof(temp), valueInt == selection[i] ? html_configAxisSelectOptionSelected : html_configAxisSelectOption, selection[i], selectionName[i]);
                    data.concat(temp);
                    www.sendContentAndClear(data);
                  }
                }
                snprintf_P(temp, sizeof(temp), html_configAxisSelectEnd, name);
                data.concat(temp);
              } break;

              // AXP_DECAY
              case 10: {
                snprintf_P(temp, sizeof(temp), html_configAxisSelectStart, axis + 1, parameterNumber);
                data.concat(temp);
                int selection[5] = {1, 2, 3, 4, 5};
                const char *selectionName[5] = {L_ADV_DECAY_MIXED, L_ADV_DECAY_FAST, L_ADV_DECAY_SLOW, L_ADV_DECAY_SPREADCYCLE, L_ADV_DECAY_STEALTHCHOP};
                for (int i = 0; i < 5; i++) {
                  if (selection[i] >= min && selection[i] <= max) {
                    snprintf_P(temp, sizeof(temp), valueInt == selection[i] ? html_configAxisSelectOptionSelected : html_configAxisSelectOption, selection[i], selectionName[i]);
                    data.concat(temp);
                    www.sendContentAndClear(data);
                  }
                }
                snprintf_P(temp, sizeof(temp), html_configAxisSelectEnd, name);
                data.concat(temp);
              } break;
              
            }
          }
        }

        data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button> "));
        www.sendContentAndClear(data);
        snprintf_P(temp, sizeof(temp), html_configAxisRevert, axis + 1);
        data.concat(temp);
        www.sendContentAndClear(data);

        success = true;
      }

      if (!success) {
        data.concat(L_ADV_SET_AXIS_NO_EDIT "<br />");
      }

      data.concat(FPSTR(html_form_end));

      data.concat(FPSTR(html_collapsable_end));

      www.sendContentAndClear(data);
    }
  #endif

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);

  return success;
}

// use Ajax key/value pairs to pass related data to the web client in the background
void axisTileAjax(int axis, String &data)
{
  char temp[80], temp1[80];

  snprintf(temp, sizeof(temp), "dvr_stat%d", axis);
  if (state.driverStatusStr[axis][0] == '?') strcpy(temp1, L_UNKNOWN); else strcpy(temp1, state.driverStatusStr[axis]);
  data.concat(keyValueString(temp, temp1));

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void axisTileGet()
{
  if (status.getVersionMajor() < 10 || (status.getVersionMajor() == 10 && status.getVersionMinor() < 26)) return;

  char command[80] = "";
  char response[80] = "";
  char argStr[20];

  #if DRIVE_CONFIGURATION == ON
 
    String ssr = www.arg("revert");
    if (!ssr.equals(EmptyStr)) {
      int axis = ssr.toInt();
      if (axis == 0) {
        strcpy(command, ":SXEM,0#");
        onStep.commandBool(command);
      } else
      if (axis >= 1 && axis <= 9) {
        snprintf(command, sizeof(command), ":SXA%d,R#",axis);
        onStep.commandBool(command);
      }
      return;
    }

    // determine what axis is being set
    int axisNumber = 0;
    // 包含第九轴，确保最后一个调焦器的参数也能提交
    for (int i = 1; i <= 9; i++) {
      snprintf(argStr, sizeof(argStr), "a%dp%dvalue", i, 1);
      if (!www.arg(argStr).equals(EmptyStr)) {
        axisNumber = i;
        break;
      }
    }
    if (axisNumber == 0) return;

    // get axis parameter count
    snprintf(command, sizeof(command), ":GXA%d,0#", axisNumber);
    if (!onStep.command(command, response)) return;
    int parameterCount = atoi(response);

    // send axis parameters to OnStepX
    for (int parameterNumber = 1; parameterNumber <= parameterCount; parameterNumber++) {
      snprintf(argStr, sizeof(argStr), "a%dp%dvalue", axisNumber, parameterNumber);
      String parameterValue = www.arg(argStr);
      snprintf(command, sizeof(command), ":SXA%d,%d,%s#", axisNumber, parameterNumber, parameterValue.c_str());
      onStep.commandBool(command);
    }
  #endif
}

bool decodeParameter(char* s, double *value, long *min, long *max, int *type, char *name) {
  char *ws = s;
  char *conv_end;
  double f = strtod(ws, &conv_end); 
  if (&s[0] != conv_end) *value = f; else return false;
  ws = strchr(ws, ',');
  if (ws != NULL) {
    ws++;
    *min = atol(ws);
    ws = strchr(ws, ',');
    if (ws != NULL) {
      ws++;
      *max = atol(ws);
      ws = strchr(ws, ',');
      if (ws != NULL) {
        ws++;
        *type = atol(ws);
        ws = strchr(ws, ',');
        if (ws != NULL) {
          ws++;
          sstrcpyex(name, ws, 18);
          return true;
        }
      }
    }
  }
  return false;
}

// 逐项检查设备返回的参数没有被预览或新布局省略
int main() {
  String data;
  assert(axisTile(0,data));
  for(int p=1;p<=26;p++) {
    const std::string key="name='a1p"+std::to_string(p)+"value'";
    assert(www.output.find(key)!=std::string::npos);
  }
  assert(www.output.find(L_AXPN_STEPS_PER_DEG)!=std::string::npos);
  assert(www.output.find(L_AXPN_CURRENT_RUN)!=std::string::npos);
  assert(www.output.find(L_REVERT)!=std::string::npos);
  assert(www.output.find(L_UPLOAD)!=std::string::npos);
  for(int p=1;p<=26;p++)www.args["a9p"+std::to_string(p)+"value"]="1";
  axisTileGet();
  assert(onStep.sent.size()==26);
  assert(onStep.sent.front()==":SXA9,1,1#");
  assert(onStep.sent.back()==":SXA9,26,1#");
  std::cout<<www.output;
}
