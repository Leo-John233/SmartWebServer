
#include <string>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <iostream>
#define PROGMEM
#define F(x) x
#define FPSTR(x) x
#define snprintf_P snprintf
#define sstrcpy strcpy
#define strlen_P strlen
#define memcpy_P memcpy
#define ON 1
#define OFF 0
#define WIFI 2
#define OPERATIONAL_MODE WIFI
#define DISPLAY_SPECIAL_CHARS ON
#define DISPLAY_RESET_CONTROLS OFF
#define OTA_PRESENT
#define CONTENT_LENGTH_UNKNOWN -1
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
using std::min;
struct String:std::string {using std::string::string;void concat(const char*s){append(s);}};
struct Web {
  std::string output;bool ended=false;
  void setContentLength(int){}
  void sendHeader(const char*,const char*){}
  void send(int,const char*,const String&){}
  void sendContent(const char*s){if(ended)return;if(!*s)ended=true;else output+=s;}
  void sendContent(const String&s){sendContent(s.c_str());}
}www;
// 和 Wi-Fi WebServer.h 相同；空字符串在分块响应中表示结束。
#define sendContentAndClear(x) sendContent(x); x = "";
struct Serial {void setTimeout(int){}} serial;
#define SERIAL_ONSTEP serial
struct Device {void serialRecvFlush(){}} onStep;
struct Status {bool onStepFound=true;} status;
struct Kv {template<class T>void get(const char*,T&){}};
struct Nv {Kv store;Kv&kv(){return store;}} storage;
Nv&nv(){return storage;}
int webTimeout=200,cmdTimeout=200;
bool servePageShell(int){return false;}
void pageHeader(int){}
void processNetworkGet(){}
void renderWifiNetwork(String&);
void sendPageResource(const char*);
void swsNetworkActivity(){}
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

// -----------------------------------------------------------------------------------
// Setup Network





const char htmL_NETWORKSerial[] PROGMEM =
"<br/><hr><form method='post' action='/net.htm'>"
L_NETWORK_CMD_TO ": <input style='width:4em' name='ccto' value='%d' type='number' min='100' max='300'> ms<br/>"
L_NETWORK_WWW_TO ": <input style='width:4em' name='wcto' value='%d' type='number' min='100' max='300'> ms<br/>"
"<br/><button type='submit'>" L_UPLOAD "</button></form>\n";

const char htmL_NET_MAC[] PROGMEM =
"MAC: <input style='width:10em' name='%s' type='text' value='%s' maxlength='17' disabled><br/>";

const char htmL_NET_MAC_AP[] PROGMEM =
"MAC: <input style='width:10em' name='apmac' type='text' value='%s' maxlength='17' disabled>&nbsp;&nbsp;&nbsp;"
L_NETWORK_CHA ": <input style='width:3em' name='apch' value='%d' type='number' min='1' max='11'><br/>";

const char htmL_NET_IP[] PROGMEM =
"<table><tr><td>" L_IP_ADDRESS ": </td><td>"
"<input name='%sip1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sip4' value='%d' type='number' min='0' max='255'></td>";

const char htmL_NET_GW[] PROGMEM =
"<tr><td>" L_GATEWAY ": </td><td>"
"<input name='%sgw1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%sgw4' value='%d' type='number' min='0' max='255'></td>";

const char htmL_NET_SN[] PROGMEM =
"<tr><td>" L_SUBNET ": </td><td>"
"<input name='%ssn1' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn2' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn3' value='%d' type='number' min='0' max='255'>&nbsp;.&nbsp;"
"<input name='%ssn4' value='%d' type='number' min='0' max='255'></td></tr></table>";

#if OPERATIONAL_MODE == WIFI
  const char htmL_NETWORKSSID1[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>"
  "SSID: <input style='width:13em; text-align:left;' name='stssid' type='text' value='%s' maxlength='32'>&nbsp;&nbsp;&nbsp;"
  L_NETWORK_PWD ": <input style='width:8em; text-align:left;' name='stpwd' type='password' minlength='8' maxlength='63'> (" L_NETWORK_PWD_MSG ")<br/>";

  const char htmL_NETWORKSSID2[] PROGMEM =
  L_NETWORK_EN_DHCP ": <input type='checkbox' name='stadhcp' value='1' %s> (" L_NETWORK_EN_DHCP_MSG ")<br/>"
  L_NETWORK_EN_STA ": <input type='checkbox' name='staen' value='1' %s><br/>"
  "<br/><button type='submit'>" L_UPLOAD "</button></form>\n";

  const char htmL_NETWORKSSID3A[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>"
  "SSID: <input style='width:13em; text-align:left;' name='apssid' type='text' ";

  const char htmL_NETWORKSSID3B[] PROGMEM =
  "value='%s' maxlength='31'>&nbsp;&nbsp;&nbsp;"
  L_NETWORK_PWD ": <input style='width:8em; text-align:left;' name='appwd' type='password' minlength='8' maxlength='31'> (" L_NETWORK_PWD_MSG ")<br/>";

  const char htmL_NETWORKSSID7[] PROGMEM =
  L_NETWORK_EN_AP_MODE ": <input type='checkbox' name='apen' value='1' %s><br/>"
  "<br/><button type='submit'>" L_UPLOAD "</button></form>\n";
#else
  const char htmL_NETWORK_ETH_DHCP[] PROGMEM =
  L_NETWORK_EN_DHCP ": <input type='checkbox' name='ethdhcp' value='1' %s> (" L_NETWORK_EN_DHCP_MSG ")<br/>";

  const char htmL_NETWORK_ETH_BEG[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>\n";

  const char htmL_NETWORK_ETH_END[] PROGMEM =
  "<button type='submit'>" L_UPLOAD "</button></form>\n";
#endif

#ifdef OTA_PRESENT
  const char html_network_ota_begin[] PROGMEM =
  "<div class='sws-ota'><p class='sws-ota-intro'>" L_NETWORK_OTA_HELP "</p>"
  "<form method='post' action='/net.htm' class='sws-ota-form'>"
  "<label for='sws-ota-password'>" L_NETWORK_OTA_PASSWORD "</label>"
  "<input id='sws-ota-password' name='otapwd' type='password' minlength='8' maxlength='63' autocomplete='new-password' aria-describedby='sws-ota-password-help'>"
  "<p id='sws-ota-password-help' class='sws-ota-help'>" L_NETWORK_OTA_PASSWORD_HELP "</p>";

  #if DISPLAY_RESET_CONTROLS != OFF
    const char html_network_ota_action[] PROGMEM =
    "<br/><button " SWS_CONFIRM_ATTRIBUTE " onclick=\"return confirm(this.dataset.confirm)\" type='submit' name='otareboot' value='1'>" L_REBOOT_OTA "</button></form>\n"
    ;
  #else
    const char html_network_ota_mode[] PROGMEM =
    "<label class='sws-ota-mode'><input type='checkbox' name='otaen' value='1' %s><span>" L_NETWORK_EN_OTA "</span></label>";
    const char html_network_ota_action[] PROGMEM =
    "<button type='submit'>" L_SAVE_SETTINGS "</button></form>\n";
  #endif
  const char html_network_ota_end[] PROGMEM =
  "<div class='sws-ota-guide'><h3>" L_NETWORK_OTA_STEPS "</h3><ol><li>"
  #if DISPLAY_RESET_CONTROLS != OFF
    L_REBOOT_OTA
  #else
    L_NETWORK_OTA_STEP_RESTART
  #endif
  "</li><li>" L_NETWORK_OTA_STEP_PORT "</li><li>" L_NETWORK_OTA_STEP_UPLOAD "</li></ol></div>"
  "<p class='sws-ota-help'>" L_NETWORK_OTA_NOTE "</p></div>";
#endif

const char html_logout[] PROGMEM =
"<h2>" L_WEB_PASSWORD_TITLE "</h2><p class='sws-account-note'>" L_WEB_PASSWORD_HELP "</p>"
"<div class='sws-account-controls'><form method='post' action='/net.htm'>"
"<label for='sws-web-password'>" L_WEB_PASSWORD_NEW "</label><div class='sws-password-actions'>"
"<input id='sws-web-password' name='webpwd' type='password' minlength='3' maxlength='31' autocomplete='new-password' aria-describedby='sws-web-password-rule'>"
"<button type='submit'>" L_WEB_PASSWORD_SAVE "</button></div></form>"
"<form class='sws-logout-form' method='post' action='/net.htm'>"
"<button class='sws-secondary' type='submit' name='logout' value='1'>" L_WEB_LOGOUT "</button></form></div>"
"<p id='sws-web-password-rule' class='sws-account-note'>" L_WEB_PASSWORD_RULE "</p>\n";

const char html_reboot[] PROGMEM =
"<br/><br/><br/><br/><br/><form method='post' action='/net.htm'>"
"<b>" L_NETWORK_RESTART_MSG "</b><br/><br/>"
"<button type='submit'>" L_CONTINUE "</button>"
"</form><br/><br/><br/><br/>"
"\n";

const char html_login[] PROGMEM =
"<div class='nw-card sws-network-entry'><section class='sws-login-card'>"
"<h2>" L_WEB_PASSWORD_TITLE "</h2><p id='sws-login-help' class='sws-entry-note'>" L_WEB_PASSWORD_HELP "</p>"
"<form method='post' action='/net.htm'><label for='sws-login-password'>" L_NETWORK_PWD "</label>"
"<div class='sws-login-actions'><input id='sws-login-password' name='login' type='password' minlength='3' maxlength='31' required autocomplete='current-password' aria-describedby='sws-login-help'>"
"<button type='submit'>" L_OK "</button></div></form></section>"
#if OPERATIONAL_MODE == WIFI
"<section class='sws-network-help'><h2>" L_NETWORK_ADVICE1 "</h2>"
"<p>" L_NETWORK_ADVICE2 "</p><p>" L_NETWORK_ADVICE3 L_NETWORK_ADVICE4 L_NETWORK_ADVICE5 L_NETWORK_ADVICE6 "</p>"
"<p>" L_NETWORK_ADVICE7 L_NETWORK_ADVICE8 L_NETWORK_ADVICE9 "</p></section>"
#endif
"</div>\n";



// 网络页使用独立的响应式样式，不加载外部字体和脚本
// 新版通信与固件更新卡片限制在屏幕宽度内，避免手机页面横向滚动
const char html_network_css[] PROGMEM = R"NET(
<style>
.sws-network-entry.nw-card {
  grid-column: 1/-1;
  display: block;
  width: 100%;
  max-width: 960px;
  margin: 0 auto;
  padding: 24px;
}
.sws-network-entry h2 { font-size: 20px; line-height: 1.5; margin: 0; }
.sws-entry-note { margin: 10px 0 0; color: var(--muted); font-size: 14px; line-height: 1.8; }
.sws-login-card form { margin: 20px 0 0; max-width: 560px; }
.sws-login-card label { margin: 0 0 8px; font-size: 14px; }
.sws-login-actions { display: flex; align-items: stretch; gap: 10px; }
.sws-login-actions input:not([type='checkbox']) { flex: 1; min-width: 0; width: 0; min-height: 46px; text-align: left; }
.sws-login-actions button { flex: 0 0 auto; margin: 0; min-height: 46px; padding: 8px 20px; }
.sws-network-help { margin-top: 24px; padding-top: 24px; border-top: 1px solid var(--border); }
.sws-network-help p { margin: 12px 0 0; color: var(--muted); font-size: 14px; line-height: 1.85; overflow-wrap: anywhere; }
.sws-network-help br { display: none; }
.sws-network-help b { display: inline; }
@media(max-width:800px) {
  .sws-network-entry.nw-card { padding: 20px; }
}

.nw-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  margin: 22px 0;
}

.nw-card {
  border: 1px solid var(--border, #383838);
  background: var(--panel, #191919);
  border-radius: 14px;
  padding: 20px;
  min-width: 0;
}

.nw-card h2 {
  margin: 0 0 8px;
  font-size: 20px;
}
.nw-note {
  color: #a9a9a9;
  font-size: 13px;
  line-height: 1.7;
  margin: 8px 0 16px;
}

.nw-card label {
  display: block;
  margin: 14px 0 6px;
}
.nw-card input:not([type='checkbox']),
.nw-card select {
  display: block;
  width: 100%;
  margin: 0;
}

.nw-card input[type='checkbox'] {
  width: 20px;
  height: 20px;
  vertical-align: middle;
  accent-color: var(--accent);
  margin: 0 8px 0 0;
}

.nw-card .nw-toggle {
  display: flex;
  align-items: center;
  font-weight: 600;
}
.nw-card details {
  margin: 16px 0;
  padding: 12px;
  border: 1px solid #373737;
  border-radius: 9px;
}

.nw-card summary {
  cursor: pointer;
}
.nw-actions {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin-top: 16px;
}
.nw-secondary {
  background: #282828;
}

.nw-state {
  padding: 16px;
  background: #202020;
  border: 1px solid #353535;
  border-radius: 12px;
  overflow-wrap: anywhere;
}

.nw-state-top {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  color: #9e9e9e;
  font-size: 12px;
}
.nw-badge {
  padding: 4px 9px;
  border-radius: 6px;
  background: #2e2e2e;
  color: #bbbbbb;
  font-size: 12px;
  white-space: nowrap;
}
.nw-badge[data-connected='true'] {
  background: #343434;
  color: #bfbfbf;
}

.nw-state-name {
  display: block;
  font-size: 18px;
  font-weight: 600;
  color: #e5e5e5;
  margin: 14px 0 16px;
}
.nw-state-values {
  display: flex;
  gap: 28px;
  flex-wrap: wrap;
  margin: 0;
}
.nw-state-values div {
  min-width: 0;
}
.nw-state-values dt {
  color: #939393;
  font-size: 12px;
  margin-bottom: 5px;
}
.nw-state-values dd {
  margin: 0;
  color: #cfcfcf;
  font-size: 14px;
  font-variant-numeric: tabular-nums;
}

/* 网络列表将名称放在左侧，锁和信号放在右侧，勾选只表示已经连接 */
/* 访问地址直接显示在已连接网络下方，断开后隐藏旧地址 */
.nw-access {
  display: flex;
  align-items: baseline;
  flex-wrap: wrap;
  gap: 8px 16px;
  margin: 12px 0 0;
  padding: 0 16px;
}
.nw-access dt {
  color: #9d9d9d;
  font-size: 13px;
}
.nw-access dd {
  margin: 0;
  color: #e4e4e4;
  font-size: 15px;
  font-variant-numeric: tabular-nums;
  overflow-wrap: anywhere;
}

.nw-networks {
  overflow: hidden;
  margin-top: 12px;
  border: 1px solid #373737;
  border-radius: 14px;
  background: #1d1d1d;
}

#nw-results {
  max-height: 360px;
  overflow: auto;
}
.nw-networks:empty {
  display: none;
}

.nw-row {
  display: flex;
  align-items: center;
  gap: 12px;
  width: 100%;
  min-height: 72px;
  text-align: left;
  background: transparent;
  border: 0;
  border-bottom: 1px solid #2f2f2f;
  border-radius: 0;
  margin: 0;
  padding: 14px 16px;
  font-weight: 400;
}

.nw-row:last-child {
  border-bottom: 0;
}
.nw-row:hover {
  background: #282828;
}

.nw-network-copy {
  display: flex;
  flex: 1;
  min-width: 0;
  flex-direction: column;
  gap: 5px;
}
.nw-network-name {
  font-size: 16px;
  overflow-wrap: anywhere;
  color: #e4e4e4;
}
.nw-network-meta {
  font-size: 12px;
  color: #9d9d9d;
}

.nw-wifi {
  width: 25px;
  height: 25px;
  flex-shrink: 0;
  fill: none;
  stroke: currentColor;
  stroke-width: 1.8;
  stroke-linecap: round;
  stroke-linejoin: round;
}
.nw-lock {
  width: 14px;
  height: 18px;
  flex-shrink: 0;
  fill: none;
  stroke: currentColor;
  stroke-width: 1.8;
}

.nw-selected {
  width: 20px;
  color: #afafaf;
  font-size: 22px;
  flex-shrink: 0;
  text-align: center;
}
.nw-info {
  border: 1.5px solid #868686;
  border-radius: 50%;
  width: 19px;
  height: 19px;
  text-align: center;
  line-height: 19px;
  font-size: 13px;
  color: #acacac;
  flex-shrink: 0;
}

.nw-list-heading {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-top: 20px;
  gap: 12px;
}
.nw-list-heading strong {
  font-size: 14px;
  color: #aaaaaa;
}
.nw-list-heading button {
  margin: 0;
  padding: 7px 12px;
  min-height: 36px;
  font-size: 13px;
}

.nw-switch-line {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 16px;
  margin: 18px 0;
}
.nw-switch-line label {
  margin: 0;
}
.nw-switch {
  appearance: none;
  position: relative;
  width: 44px !important;
  height: 26px !important;
  border-radius: 18px;
  padding: 0;
  background: #494949;
  cursor: pointer;
  margin: 0 !important;
  flex-shrink: 0;
}
.nw-switch:before {
  content: '';
  position: absolute;
  top: 3px;
  left: 3px;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #fff;
  transition: transform 0.15s;
}
.nw-switch:checked {
  background: var(--accent);
  border-color: var(--accent);
}
.nw-switch:checked:before {
  transform: translateX(18px);
}

/* 点选网络后弹出独立连接窗口，密码和高级设置不再常驻列表 */
#nw-dialog {
  box-sizing: border-box;
  width: calc(100% - 24px);
  max-width: 440px;
  max-height: 85dvh;
  overflow: auto;
  color: #cdcdcd;
  background: #1d1d1d;
  border: 1px solid #4d4d4d;
  border-radius: 18px;
  padding: 22px;
}
#nw-dialog::backdrop {
  background: #0009;
}

.nw-dialog-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
}
.nw-dialog-head h2 {
  overflow-wrap: anywhere;
  font-size: 20px;
  margin: 0;
}
.nw-dialog-head button {
  background: transparent;
  border: 0;
  font-size: 22px;
  padding: 4px 10px;
}

.nw-password-line {
  display: flex;
  align-items: stretch;
  border: 1px solid #585858;
  border-radius: 9px;
  overflow: hidden;
  background: #1f1f1f;
}
.nw-card .nw-password-line input {
  min-width: 0;
  border: 0;
  border-radius: 0;
  flex: 1;
}
.nw-password-line button {
  background: transparent;
  border: 0;
  border-radius: 0;
  padding: 8px 12px;
  color: #afafaf;
  font-size: 13px;
  white-space: nowrap;
}

/* 使用统一的显示按钮，避免浏览器自带的密码图标重复出现 */
#nw-password::-ms-reveal {
  display: none;
}

#nw-dialog .nw-actions button {
  flex: 1;
}
#nw-dialog-error {
  color: #c1c1c1;
  overflow-wrap: anywhere;
}
#nw-dialog-error:empty {
  display: none;
}

[hidden] {
  display: none !important;
}

.nw-feedback {
  margin: 12px 0 0;
  padding: 10px 12px;
  border: 1px solid var(--border);
  border-left: 3px solid #777;
  border-radius: 8px;
  background: var(--field);
  color: var(--text);
  font-size: 13px;
  line-height: 1.7;
  overflow-wrap: anywhere;
}
.nw-feedback.nw-error {
  border-left-color: var(--accent);
}

@media (max-width: 650px) {
  .nw-grid {
    grid-template-columns: 1fr;
  }
  .nw-card {
    padding: 16px;
  }
  .nw-actions button {
    flex: 1;
  }
  input {
    max-width: 100%;
  }
}
</style>
)NET";



// 表单的 name 和 id 对应原有配网接口。
const char html_network_wifi[] PROGMEM = R"NET(
<div class="nw-grid" id="nw-panel">
<section class="nw-card" id="nw-sta-card"><h2>Wi-Fi <small>STA</small></h2>
<p class="nw-note">)NET"
 L_UI_STATION_NOTE
 R"NET(</p>
<div class="nw-switch-line"><label for="nw-enabled">)NET"
 L_UI_WIFI_CONNECT
 R"NET(</label><input id="nw-enabled" class="nw-switch" type="checkbox" role="switch"></div>
<div id="nw-sta-state" aria-live="polite"><span id="nw-sta-badge" class="nw-note">)NET"
 L_UI_LOADING
 R"NET(</span><p id="nw-message" class="nw-feedback" role="status" aria-live="polite" hidden></p><div id="nw-current" class="nw-networks"></div><dl id="nw-access" class="nw-access" hidden><dt>)NET"
 L_UI_ACCESS_ADDRESS
 R"NET(</dt><dd id="nw-access-ip">—</dd></dl></div>
<div id="nw-available"><div class="nw-list-heading"><strong>)NET"
 L_UI_OTHER_NETWORKS
 R"NET(</strong><button type="button" id="nw-scan" class="nw-secondary">)NET"
 L_UI_REFRESH
 R"NET(</button></div>
<p id="nw-scan-state" class="nw-note" role="status"></p><div id="nw-results" class="nw-networks"></div>
<div class="nw-networks"><button type="button" id="nw-manual" class="nw-row"><span class="nw-network-copy">)NET"
 L_UI_OTHER_NETWORK
 R"NET(<span class="nw-network-meta">)NET"
 L_UI_HIDDEN_NETWORK
 R"NET(</span></span><span aria-hidden="true">＋</span></button></div></div>
<dialog id="nw-dialog" class="nw-card" aria-labelledby="nw-dialog-title">
<div class="nw-dialog-head"><h2 id="nw-dialog-title">)NET"
 L_UI_WIFI_CONNECT
 R"NET(</h2><button type="button" id="nw-close" aria-label=")NET"
 L_UI_CLOSE
 R"NET(">×</button></div>
<p id="nw-dialog-note" class="nw-note"></p>
<form id="nw-sta"><input name="enabled" type="checkbox" hidden><input name="open" type="checkbox" hidden>
<div id="nw-manual-fields"><label for="nw-ssid">)NET"
 L_UI_SSID
 R"NET(</label><input id="nw-ssid" name="ssid" maxlength="31" autocomplete="off" required>
<label for="nw-security">)NET"
 L_UI_SECURITY
 R"NET(</label><select id="nw-security"><option value="password">)NET"
 L_UI_SECURED
 R"NET(</option><option value="open">)NET"
 L_UI_OPEN
 R"NET(</option></select></div>
<div id="nw-password-fields"><label for="nw-password">)NET"
 L_UI_PASSWORD
 R"NET(</label><div class="nw-password-line"><input id="nw-password" name="password" type="password" maxlength="31" autocomplete="new-password"><button type="button" id="nw-show" aria-pressed="false">)NET"
 L_UI_SHOW
 R"NET(</button></div></div>
<p id="nw-open-note" class="nw-note" hidden>)NET"
 L_UI_OPEN_NOTE
 R"NET(</p>
<details id="nw-advanced"><summary>)NET"
 L_UI_NETWORK_SETTINGS
 R"NET(</summary><dl id="nw-connection-details" class="nw-state-values" hidden><div><dt>)NET"
 L_UI_IP_ADDRESS
 R"NET(</dt><dd id="nw-sta-ip">—</dd></div><div><dt>)NET"
 L_UI_SIGNAL
 R"NET(</dt><dd id="nw-sta-signal">—</dd></div></dl>
<div class="nw-switch-line"><label for="nw-dhcp">)NET"
 L_UI_DHCP
 R"NET(</label><input id="nw-dhcp" class="nw-switch" name="dhcp" type="checkbox" role="switch"></div><div id="nw-static">
<label>)NET"
 L_UI_STATIC_IP
 R"NET(<input name="ip" inputmode="decimal" placeholder="192.168.1.55"></label>
<label>)NET"
 L_UI_GATEWAY
 R"NET(<input name="gw" inputmode="decimal" placeholder="192.168.1.1"></label>
<label>)NET"
 L_UI_SUBNET
 R"NET(<input name="sn" inputmode="decimal" placeholder="255.255.255.0"></label></div></details>
<p id="nw-dialog-error" role="alert"></p><div class="nw-actions"><button type="button" id="nw-cancel" class="nw-secondary">)NET"
 L_UI_CANCEL
 R"NET(</button><button id="nw-connect" type="submit">)NET"
 L_UI_CONNECT
 R"NET(</button></div></form></dialog></section>
<section class="nw-card"><h2>)NET"
 L_UI_AP_TITLE
 R"NET(<small>AP</small></h2>
<p class="nw-note">)NET"
 L_UI_AP_NOTE
 R"NET(</p>
<div class="nw-state" id="nw-ap-state" aria-live="polite">
<div class="nw-state-top"><span>)NET"
 L_UI_DEVICE_AP
 R"NET(</span><span class="nw-badge" id="nw-ap-badge">)NET"
 L_UI_LOADING
 R"NET(</span></div>
<strong class="nw-state-name" id="nw-ap-name">—</strong>
<dl class="nw-state-values"><div><dt>)NET"
 L_UI_ACCESS_ADDRESS
 R"NET(</dt><dd id="nw-ap-ip">—</dd></div></dl></div>
<form id="nw-ap"><label class="nw-toggle"><input name="enabled" type="checkbox">)NET"
 L_UI_AP_ENABLE
 R"NET(</label>
<label>)NET"
 L_UI_AP_NAME
 R"NET(<input name="ssid" maxlength="31" required autocomplete="off"></label>
<label>)NET"
 L_UI_AP_PASSWORD
 R"NET(<input name="password" type="password" minlength="8" maxlength="31" autocomplete="new-password" placeholder=")NET"
 L_UI_KEEP_PASSWORD
 R"NET("></label>
<details><summary>)NET"
 L_UI_AP_ADVANCED
 R"NET(</summary><label>)NET"
 L_UI_CHANNEL
 R"NET(<input name="channel" type="number" min="1" max="11" required></label>
<label>)NET"
 L_UI_AP_IP
 R"NET(<input name="ip" inputmode="decimal" required></label>
<label>)NET"
 L_UI_GATEWAY
 R"NET(<input name="gw" inputmode="decimal" required></label>
<label>)NET"
 L_UI_SUBNET
 R"NET(<input name="sn" inputmode="decimal" required></label></details>
<p class="nw-note">)NET"
 L_UI_AP_RECOVERY_NOTE
 R"NET(<br>)NET"
 L_UI_AP_SCAN_NOTE
 R"NET(</p>
<div class="nw-actions"><button type="submit">)NET"
 L_UI_AP_SAVE
 R"NET(</button></div><p id="nw-ap-message" class="nw-feedback" role="status" aria-live="polite" hidden></p></form></section>
</div>
)NET";




// 浏览器扫描、连接与状态显示；只调用 NetworkWifi.cpp 提供的接口。
const char html_network_wifi_script[] PROGMEM = R"NET(
<script>
(() => {
  const $ = (id) => document.getElementById(id),
    sta = $('nw-sta'),
    ap = $('nw-ap'),
    dialog = $('nw-dialog');
  let loaded = false,
    scanning = false,
    working = false,
    submitting = false,
    scanStarted = 0,
    scanBusy = false,
    paused = false,
    statusTimer = null,
    scanTimer = null,
    revision = 0,
    lastNotice = '',
    messageTarget = 'sta',
    statusBusy = false,
    current = null,
    networks = [],
    autoScanned = false,
    listKey = '',
    enableDraft = false;
  const reads = new Set();
  function scheduleStatus(delay = 2500) {
    clearTimeout(statusTimer);
    if (!paused && !document.hidden) statusTimer = setTimeout(status, delay);
  }
  function message(text, error = false, target = 'sta') {
    messageTarget = target;
    const active = $(target === 'ap' ? 'nw-ap-message' : 'nw-message');
    ['nw-message','nw-ap-message'].forEach(id => {
      const node = $(id), value = node === active ? text : '';
      if (node.textContent !== value) node.textContent = value;
      node.className = 'nw-feedback' + (node === active && error ? ' nw-error' : '');
      node.hidden = !value;
    });
  }
  function sync() {
    $('nw-static').hidden = sta.elements.dhcp.checked;
    $('nw-password').disabled = sta.elements.open.checked;
    $('nw-password-fields').hidden = sta.elements.open.checked;
    $('nw-open-note').hidden = !sta.elements.open.checked;
    $('nw-available').hidden = !$('nw-enabled').checked;
    $('nw-scan').disabled = !loaded || scanning || working || !$('nw-enabled').checked;
    $('nw-enabled').disabled = !loaded || scanning || working;
    $('nw-manual').disabled = !loaded || scanning || working;
    $('nw-connect').disabled = !loaded || scanning || working;
    document
      .querySelectorAll('.nw-row')
      .forEach((b) => (b.disabled = !loaded || working || scanning));
    ap.querySelector('[type=submit]').disabled = !loaded || scanning || working;
  }
  function lock(value) {
    working = value || submitting;
    sync();
  }
  // 请求设置超时，断网时停止等待，避免不断堆积连接
  async function api(action, params) {
    const controller = new AbortController(),
      timeout = action === 'scan' ? Math.max(1, Math.min(8000, 30000 - (Date.now() - scanStarted))) : 8000,
      timer = setTimeout(() => controller.abort(), timeout);
    const read = action === 'status' || action === 'scan';
    if (read) reads.add(controller);
    if (params) revision++;
    try {
      const options = { cache: 'no-store', signal: controller.signal };
      if (params) {
        options.method = 'POST';
        options.headers = { 'Content-Type': 'application/x-www-form-urlencoded' };
        options.body = new URLSearchParams({ ...params, action });
      }
      const response = await fetch('/net-wifi?' + new URLSearchParams({ action }), options),
        data = await response.json();
      if (!response.ok) throw Error(data.error || )NET" SWS_JS_TEXT(L_UI_FAILED) R"NET();
      return data;
    } catch (error) {
      if (['AbortError','TypeError','SyntaxError'].includes(error.name)) throw Error()NET" SWS_JS_TEXT(L_UI_CONNECTION_LOST) R"NET();
      throw error;
    } finally {
      clearTimeout(timer);
      reads.delete(controller);
    }
  }
  function fields(form, data) {
    Object.keys(data).forEach((k) => {
      const e = form.elements[k];
      if (!e) return;
      if (e.type === 'checkbox') e.checked = !!data[k];
      else e.value = data[k];
    });
  }
  // 三条弧按 RSSI 分档，弱信号仍保留圆点；无有效读数时全部变暗。
  function signalIcon(rssi) {
    const valid = Number.isFinite(rssi) && rssi < 0;
    const level = !valid ? 0 : rssi >= -55 ? 3 : rssi >= -70 ? 2 : rssi >= -85 ? 1 : 0;
    const arcs = [
      'M11 20a7 7 0 0 1 10 0',
      'M7 15a13 13 0 0 1 18 0',
      'M3 10a19 19 0 0 1 26 0',
    ];
    return '<svg class="nw-wifi" viewBox="0 0 32 32" aria-hidden="true">' +
      arcs.map((d, i) => '<path d="' + d + '" opacity="' + (i < level ? 1 : 0.18) + '"/>').join('') +
      '<circle cx="16" cy="25" r="1.6" fill="currentColor" stroke="none" opacity="' + (valid ? 1 : 0.18) + '"/></svg>';
  }
  // 网络名称只通过纯文本写入，图标依据每个网络的真实信号读数生成。
  function row(n, connected) {
    const b = document.createElement('button');
    b.type = 'button';
    b.className = 'nw-row';
    b.setAttribute(
      'aria-label',
      n.ssid +
        (connected
          ? )NET" SWS_JS_TEXT(L_UI_CONNECTED_DETAILS) R"NET(
          : ' ' + n.rssi + ' dBm ' + (n.open ? )NET" SWS_JS_TEXT(L_UI_OPEN) R"NET( : )NET" SWS_JS_TEXT(L_UI_SECURED) R"NET()),
    );
    b.innerHTML =
      '<span class="nw-network-copy"><span class="nw-network-name"></span><span class="nw-network-meta"></span></span>' +
      (n.open
        ? ''
        : '<svg class="nw-lock" viewBox="0 0 16 20" aria-hidden="true"><rect x="3" y="8" width="10" height="9" rx="2"/><path d="M5 8V5a3 3 0 0 1 6 0v3"/></svg>') +
      signalIcon(n.rssi) +
      (connected
        ? '<span class="nw-selected" aria-hidden="true">✓</span>'
        : '<span class="nw-info" aria-hidden="true">i</span>');
    b.querySelector('.nw-network-name').textContent = n.ssid;
    b.querySelector('.nw-network-meta').textContent = n.rssi + ' dBm';
    b.onclick = () => openNetwork(n, false);
    return b;
  }
  // 列表不变时保留节点，避免轮询反复打断用户选择
  function renderNetworks() {
    if (!current) return;
    const key = JSON.stringify([
      current.connected,
      current.ssid,
      current.rssi,
      current.sta.open,
      networks,
    ]);
    if (key === listKey) return;
    listKey = key;
    $('nw-current').replaceChildren();
    if (current.connected)
      $('nw-current').appendChild(
        row({ ssid: current.ssid, rssi: current.rssi, open: current.sta.open }, true),
      );
    $('nw-results').replaceChildren();
    networks
      .filter((n) => !current.connected || n.ssid !== current.ssid)
      .forEach((n) => $('nw-results').appendChild(row(n, false)));
  }
  // 弹窗按所选网络初始化，取消不会改变连接，密码也不会留到下次打开
  function openNetwork(n, manual) {
    if (!loaded || working || scanning) return;
    fields(sta, current.sta);
    sta.elements.enabled.checked = true;
    const same = !manual && n.ssid === current.sta.ssid,
      connected = !manual && current.connected && n.ssid === current.ssid;
    sta.elements.ssid.value = manual ? '' : n.ssid;
    sta.elements.open.checked = manual ? false : n.open;
    if (!same) sta.elements.dhcp.checked = true;
    $('nw-manual-fields').hidden = !manual;
    $('nw-ssid').readOnly = !manual;
    $('nw-security').value = sta.elements.open.checked ? 'open' : 'password';
    $('nw-dialog-title').textContent = manual ? )NET" SWS_JS_TEXT(L_UI_ADD_NETWORK) R"NET( : n.ssid;
    $('nw-dialog-note').textContent = connected
      ? )NET" SWS_JS_TEXT(L_UI_CONNECTED_NOTE) R"NET(
      : manual
        ? )NET" SWS_JS_TEXT(L_UI_MANUAL_NOTE) R"NET(
        : n.rssi + ' dBm';
    $('nw-password').value = '';
    $('nw-password').type = 'password';
    $('nw-password').placeholder = same ? )NET" SWS_JS_TEXT(L_UI_SAVED_PASSWORD) R"NET( : )NET" SWS_JS_TEXT(L_UI_PASSWORD_PROMPT) R"NET(;
    $('nw-password').required = !(same && !current.sta.open) && !sta.elements.open.checked;
    $('nw-password').minLength = 8;
    $('nw-show').textContent = )NET" SWS_JS_TEXT(L_UI_SHOW) R"NET(;
    $('nw-show').setAttribute('aria-pressed', 'false');
    $('nw-dialog-error').textContent = '';
    $('nw-advanced').open = connected;
    $('nw-connection-details').hidden = !connected;
    $('nw-connect').textContent = connected ? )NET" SWS_JS_TEXT(L_UI_SAVE) R"NET( : )NET" SWS_JS_TEXT(L_UI_CONNECT) R"NET(;
    sync();
    dialog.showModal();
    if (manual) $('nw-ssid').focus();
    else if (!sta.elements.open.checked && !connected) $('nw-password').focus();
  }
  dialog.addEventListener('close', () => {
    $('nw-password').value = '';
    $('nw-password').type = 'password';
  });
  $('nw-close').onclick = $('nw-cancel').onclick = () => dialog.close();
  // 轮询只更新设备状态，不覆盖弹窗中正在编辑的内容
  async function status() {
    if (statusBusy || scanning || submitting || paused || document.hidden || document.body.inert) { scheduleStatus(); return; }
    statusBusy = true;
    const ticket = revision;
    try {
      const s = await api('status');
      if (ticket !== revision || paused) return;
      current = s;
      if (!loaded) {
        fields(ap, s.ap);
        loaded = true;
      }
      if (!submitting) $('nw-enabled').checked = s.sta.enabled || enableDraft;
      $('nw-sta-badge').textContent = s.connected
        ? )NET" SWS_JS_TEXT(L_UI_CONNECTED) R"NET(
        : s.connecting
          ? )NET" SWS_JS_TEXT(L_UI_CONNECTING) R"NET( + s.sta.ssid
          : s.sta.enabled || enableDraft
            ? )NET" SWS_JS_TEXT(L_UI_NOT_CONNECTED) R"NET(
            : )NET" SWS_JS_TEXT(L_UI_WIFI_OFF) R"NET(;
      // 使用实时连接地址，避免把配置中的静态地址当作实际访问地址
      $('nw-access').hidden = !s.connected;
      $('nw-access-ip').textContent = s.connected ? s.ip : '—';
      $('nw-sta-ip').textContent = s.connected ? s.ip : )NET" SWS_JS_TEXT(L_UI_NO_IP) R"NET(;
      $('nw-sta-signal').textContent = s.connected ? s.rssi + ' dBm' : '—';
      $('nw-ap-badge').textContent = s.apActive ? )NET" SWS_JS_TEXT(L_UI_ENABLED) R"NET( : )NET" SWS_JS_TEXT(L_UI_DISABLED) R"NET(;
      $('nw-ap-badge').dataset.connected = String(s.apActive);
      $('nw-ap-name').textContent = s.apSsid;
      $('nw-ap-ip').textContent = s.apActive ? s.apIp : '—';
      renderNetworks();
      // 已连接及访问地址已在网络卡片内显示，不在卡片外重复提示。
      const noticeKey = JSON.stringify([s.message, s.messageTarget, s.failed, s.connected, s.connecting, s.pending]);
      if (noticeKey !== lastNotice) {
        lastNotice = noticeKey;
        const connectionShown = s.connected && !s.failed && !s.connecting && !s.pending &&
          (s.message === )NET" SWS_JS_TEXT(L_UI_CONNECTED) R"NET( ||
           (s.message || '').startsWith()NET" SWS_JS_TEXT(L_UI_CONNECTED_ADDRESS) R"NET());
        message(connectionShown ? '' : (s.message || ''), s.failed, s.messageTarget === 'ap' ? 'ap' : 'sta');
      }
      lock(s.connecting || s.pending);
      if (!autoScanned && s.sta.enabled && !working) {
        autoScanned = true;
        startScan();
      }
    } catch (e) {
      if (paused || ticket !== revision) return;
      lastNotice = '';
      message()NET" SWS_JS_TEXT(L_UI_CONNECTION_LOST) R"NET(, true, messageTarget);
      lock(false);
    } finally {
      statusBusy = false;
      scheduleStatus();
    }
  }
  async function scan() {
    clearTimeout(scanTimer);
    if (scanBusy || paused || document.hidden) return;
    scanBusy = true;
    try {
      if (Date.now() - scanStarted >= 30000) throw Error()NET" SWS_JS_TEXT(L_UI_SCAN_TIMEOUT) R"NET();
      const r = await api('scan', {});
      if (paused) return;
      if (r.running) {
        return;
      }
      scanning = false;
      const seen = new Set();
      networks = r.networks
        .sort((a, b) => b.rssi - a.rssi)
        .filter((n) => {
          if (!n.ssid || seen.has(n.ssid)) return false;
          seen.add(n.ssid);
          return true;
        });
      renderNetworks();
      $('nw-scan-state').textContent = networks.length ? )NET" SWS_JS_TEXT(L_UI_PICK_NETWORK) R"NET( : )NET" SWS_JS_TEXT(L_UI_NO_NETWORKS) R"NET(;
    } catch (e) {
      if (paused) return;
      scanning = false;
      $('nw-scan-state').textContent = Date.now() - scanStarted >= 30000 ? )NET" SWS_JS_TEXT(L_UI_SCAN_TIMEOUT) R"NET( : e.message;
    } finally {
      scanBusy = false;
      sync();
      if (scanning) {
        if (!paused && !document.hidden) scanTimer = setTimeout(scan, 1000);
      } else scheduleStatus(0);
    }
  }
  // 进入已开启 Wi-Fi 的页面和打开开关时自动搜索，也保留手动刷新入口
  function startScan() {
    if (scanning || working || !loaded) return;
    scanning = true;
    scanStarted = Date.now();
    clearTimeout(statusTimer);
    $('nw-scan-state').textContent = )NET" SWS_JS_TEXT(L_UI_SEARCHING) R"NET(;
    sync();
    scan();
  }
  $('nw-scan').onclick = startScan;
  $('nw-manual').onclick = () => openNetwork(null, true);
  $('nw-security').onchange = () => {
    sta.elements.open.checked = $('nw-security').value === 'open';
    $('nw-password').required = !sta.elements.open.checked;
    $('nw-password').value = '';
    sync();
  };
  sta.addEventListener('change', sync);
  $('nw-show').onclick = () => {
    const show = $('nw-password').type === 'password';
    $('nw-password').type = show ? 'text' : 'password';
    $('nw-show').textContent = show ? )NET" SWS_JS_TEXT(L_UI_HIDE) R"NET( : )NET" SWS_JS_TEXT(L_UI_SHOW) R"NET(;
    $('nw-show').setAttribute('aria-pressed', String(show));
  };
  // 关闭连接要确认，开启时先展示列表，选好网络后才发送连接请求
  $('nw-enabled').onchange = async () => {
    enableDraft = $('nw-enabled').checked;
    if (enableDraft) {
      startScan();
      return;
    }
    if (!current.sta.enabled) {
      sync();
      return;
    }
    if (!confirm()NET" SWS_JS_TEXT(L_UI_DISCONNECT_CONFIRM) R"NET()) {
      $('nw-enabled').checked = true;
      return;
    }
    submitting = true;
    lock(true);
    try {
      const p = {
        ...current.sta,
        enabled: '0',
        open: current.sta.open ? '1' : '0',
        dhcp: current.sta.dhcp ? '1' : '0',
        password: '',
      };
      const r = await api('connect', p);
      message(r.message);
    } catch (e) {
      $('nw-enabled').checked = true;
      message(e.message, true);
    } finally {
      submitting = false;
      await status();
    }
  };
  for (const [form, action] of [
    [sta, 'connect'],
    [ap, 'ap'],
  ])
    form.onsubmit = async (e) => {
      e.preventDefault();
      if (working || scanning) return;
      if (action === 'ap' && !confirm()NET" SWS_JS_TEXT(L_UI_AP_CONFIRM) R"NET()) return;
      const p = Object.fromEntries(new FormData(form));
      ['enabled', 'dhcp', 'open'].forEach((k) => {
        if (form.elements[k]) p[k] = form.elements[k].checked ? '1' : '0';
      });
      submitting = true;
      lock(true);
      $('nw-dialog-error').textContent = '';
      try {
        const r = await api(action, p);
        message(r.message, false, action === 'ap' ? 'ap' : 'sta');
        form.elements.password.value = '';
        if (action === 'connect') {
          enableDraft = false;
          dialog.close();
        }
      } catch (err) {
        if (action === 'connect') $('nw-dialog-error').textContent = err.message;
        else message(err.message, true, 'ap');
      } finally {
        submitting = false;
        lock(false);
        await status();
      }
    };
  // 后台页面停止读取；回来后重新读取状态，不重发任何设置或连接命令。
  const pause = window.swsPausePolling, resume = window.swsResumePolling;
  window.swsPausePolling = () => {
    paused = true;
    revision++;
    clearTimeout(statusTimer); clearTimeout(scanTimer);
    reads.forEach(controller => controller.abort());
    if (pause) pause();
  };
  window.swsResumePolling = () => {
    paused = false;
    if (resume) resume();
    if (scanning) scan(); else status();
  };
  document.addEventListener('visibilitychange', () => {
    if (document.hidden) window.swsPausePolling(); else window.swsResumePolling();
  });
  window.addEventListener('pagehide', window.swsPausePolling);
  window.addEventListener('pageshow', () => { if (!paused) return; window.swsResumePolling(); });
  window.addEventListener('online', () => { if (!paused) status(); });
  sync(); status();
})();
</script>
)NET";

void sendPageResource(const char *content, size_t length) {
  char part[1025];
  for (size_t offset = 0; offset < length; offset += sizeof(part) - 1) {
    const size_t count = min(sizeof(part) - 1, length - offset);
    memcpy_P(part, content + offset, count);
    part[count] = 0;
    www.sendContent(part);
    swsNetworkActivity();
  }
}

void sendPageResource(const char *content) {
  sendPageResource(content, strlen_P(content));
}




#if OPERATIONAL_MODE == WIFI



// 界面资源与配网处理分开；按小块发送，避免整页复制到 RAM。
void renderWifiNetwork(String &data) {
  www.sendContentAndClear(data);
  sendPageResource(html_network_wifi);
  sendPageResource(html_network_wifi_script);
}
#endif

// -----------------------------------------------------------------------------------
// Setup Network











extern int webTimeout;
extern int cmdTimeout;

void processNetworkGet();

bool restartRequired = false;
bool loginRequired = true;

#ifdef OTA_PRESENT

  typedef struct OtaSettings {
    char password[64];
    bool enabled;
  } OtaSettings;
#endif

void handleNetwork() {
  if (servePageShell(PAGE_NETWORK)) return;
  char temp[420]  = "";
  char temp1[140] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();
  
  processNetworkGet();

  www.setContentLength(CONTENT_LENGTH_UNKNOWN);
  www.sendHeader("Cache-Control", "no-cache");
  www.send(200, "text/html", String());

  // send a standard http response header
  String data = FPSTR(html_head_begin);
  data.concat(FPSTR(html_main_css_begin));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_core));
  data.concat(FPSTR(html_main_css_control));
  www.sendContentAndClear(data);
  data.concat(FPSTR(html_main_css_end));
  // 保留开发分支的手机端网络页面样式
  data.concat(FPSTR(html_network_css));
  data.concat(FPSTR(html_head_end));
  www.sendContentAndClear(data);

  // show this page
  data.concat(FPSTR(html_body_begin));
  www.sendContentAndClear(data);
  pageHeader(PAGE_NETWORK);
  data.concat(FPSTR(html_onstep_page_begin));

  // OnStep wasn't found, show warning and info.
  if (!status.onStepFound) {
    data.concat(FPSTR(html_bad_comms_message));
    data.concat(FPSTR(html_page_and_body_end));
    www.sendContentAndClear(data);
    www.sendContent("");
    return;
  }

  // page contents
  data.concat(FPSTR(html_layout_network_begin));

  if (restartRequired) {
    restartRequired = false;
    data.concat(FPSTR(html_reboot));
  } else
  if (loginRequired) {
    restartRequired = false;
    data.concat(FPSTR(html_login));
  } else {

    #if OPERATIONAL_MODE == WIFI
      // 登录与新版设置存储沿用上游，路由器和热点设置保留开发版界面
      renderWifiNetwork(data);
    #else
      snprintf_P(temp, sizeof(temp), html_tile_text_beg, "33em", "19em", "Ethernet"); data.concat(temp);
      data.concat(FPSTR(htmL_NETWORK_ETH_BEG));

      temp1[0] = 0;
      for (int i = 0; i < 6; i++) {
        char temp2[200];
        snprintf(temp2, sizeof(temp2), "%s%02x:", temp1, ethernetManager.settings.mac[i]);
        sstrcpy(temp1, temp2);
      }
      temp1[strlen(temp1) - 1] = 0;

      snprintf_P(temp, sizeof(temp), htmL_NET_MAC,"eth", temp1); data.concat(temp);
      www.sendContentAndClear(data);

      snprintf_P(temp, sizeof(temp), htmL_NET_IP, "eth", (int)ethernetManager.settings.ip[0], "eth", (int)ethernetManager.settings.ip[1], "eth", (int)ethernetManager.settings.ip[2], "eth", (int)ethernetManager.settings.ip[3]); data.concat(temp);
      snprintf_P(temp, sizeof(temp), htmL_NET_GW, "eth", (int)ethernetManager.settings.gw[0], "eth", (int)ethernetManager.settings.gw[1], "eth", (int)ethernetManager.settings.gw[2], "eth", (int)ethernetManager.settings.gw[3]); data.concat(temp);
      snprintf_P(temp, sizeof(temp), htmL_NET_SN, "eth", (int)ethernetManager.settings.sn[0], "eth", (int)ethernetManager.settings.sn[1], "eth", (int)ethernetManager.settings.sn[2], "eth", (int)ethernetManager.settings.sn[3]); data.concat(temp);
      www.sendContentAndClear(data);

      snprintf_P(temp, sizeof(temp), htmL_NETWORK_ETH_DHCP, ethernetManager.settings.dhcpEnabled ? "checked" : ""); data.concat(temp);

      data.concat(FPSTR(htmL_NETWORK_ETH_END));
      data.concat(FPSTR(html_tile_end));
    #endif
    // Wi-Fi 界面已分段发送并清空 data；空分块会提前结束 HTTP 响应。
    if (data.length()) { www.sendContentAndClear(data); }

    #ifdef OTA_PRESENT
      OtaSettings otaSettings = {"", false};
      nv().kv().get("OTA_SETTINGS", otaSettings);
      
      snprintf_P(temp, sizeof(temp), html_tile_text_beg, "33em", "10em", L_NETWORK_OTA_TITLE); data.concat(temp);
      data.concat(FPSTR(html_network_ota_begin));
      www.sendContentAndClear(data);
      #if DISPLAY_RESET_CONTROLS == OFF
        snprintf_P(temp, sizeof(temp), html_network_ota_mode, otaSettings.enabled ? "checked" : ""); data.concat(temp);
      #endif
      data.concat(FPSTR(html_network_ota_action));
      www.sendContentAndClear(data);
      data.concat(FPSTR(html_network_ota_end));
      data.concat(FPSTR(html_tile_end));
      www.sendContentAndClear(data);
    #endif

    snprintf_P(temp, sizeof(temp), html_tile_text_beg, "33em", "10em", L_NETWORK_PERFORMANCE); data.concat(temp);
    snprintf_P(temp, sizeof(temp), htmL_NETWORKSerial, cmdTimeout, webTimeout); data.concat(temp);
    data.concat(FPSTR(html_tile_end));
    www.sendContentAndClear(data);

    // 账号设置独占一行，通信与固件更新按屏幕宽度排列
    data.concat(FPSTR(html_layout_network_account));
    data.concat(FPSTR(html_logout));
    data.concat(FPSTR(html_layout_network_account_end));
  }

  sstrcpy(temp, "</div></div></body></html>");
  data.concat(temp);

  www.sendContentAndClear(data);
  www.sendContent("");
}

int main(){loginRequired=true;handleNetwork();
  if(!www.ended){std::cerr<<"missing response terminator";return 1;}
  std::cout<<www.output;
}