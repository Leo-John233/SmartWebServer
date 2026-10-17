#define ON 1
#define DISPLAY_SPECIAL_CHARS ON
#define OFF 0
#define PROGMEM
#define DISPLAY_WEATHER ON
#define DISPLAY_INTERNAL_TEMPERATURE ON
#define WIFI 2
#define OPERATIONAL_MODE WIFI
#define DRIVE_CONFIGURATION ON
#define DISPLAY_COORDINATE_ORIGIN ON
#define DISPLAY_RESET_CONTROLS ON
#define HOME_OFFSET_RANGE_AXIS1 3600
#define HOME_OFFSET_RANGE_AXIS2 3600
#define STR_INNER(x) #x
#define STR(x) STR_INNER(x)
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



PREVIEW_FILE_0


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


PREVIEW_FILE_1


// 编辑卡片共用内嵌图标，不依赖字体或外部资源。
#define SWS_EDIT_ICON "<svg viewBox='0 0 24 24' aria-hidden='true'><path d='m16 3 5 5-12 12-6 1 1-6Zm-2 2 5 5' fill='currentColor'/></svg>"

// 语言仍由 DISPLAY_LANGUAGE / Locale.h 在编译时选择。
// 将 C++ 字符串转为已转义的 JavaScript 字符串，避免引号破坏脚本。
#define SWS_JS_TEXT_INNER(text) #text
#define SWS_JS_TEXT(text) SWS_JS_TEXT_INNER(text)

// 文案放进 HTML 属性，由事件读取；语言里的单引号不会进入 JavaScript。
#define SWS_CONFIRM_ATTRIBUTE " data-confirm=\"" L_ARE_YOU_SURE "?\" "


PREVIEW_FILE_2


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


PREVIEW_FILE_3






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


PREVIEW_FILE_4
// -----------------------------------------------------------------------------------
// Ambient tile






#if DISPLAY_WEATHER == ON

extern void ambientTile(String &data);
extern void ambientTileAjax(String &data);
extern void ambientTileGet();

const char html_indexTPHD[] PROGMEM = "<div class='sws-metric'><span>%s</span><strong id='tphd_%c' class='c'>%s</strong></div>";

#endif


PREVIEW_FILE_5
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


PREVIEW_FILE_6
// -----------------------------------------------------------------------------------
// The home page, status information









#if DISPLAY_SERVO_MONITOR == ON

#endif
#if DISPLAY_STALLGUARD_MONITOR == ON

#endif

#if DRIVE_CONFIGURATION == ON
  const char html_configAxesNotes[] PROGMEM =
  "<br />Notes:<ul>"
  "<li>" L_ADV_SET_FOOTER_MSG0 "</li>"
  "<li>" L_ADV_SET_FOOTER_MSG1 "</li>"
  "<li>" L_ADV_SET_FOOTER_MSG2 "</li>";

  const char html_configAxesNotesOnStep[] PROGMEM =
  "<li>" L_ADV_SET_FOOTER_MSG4 "</li>"
  "</ul>";
#endif


PREVIEW_FILE_7
// -----------------------------------------------------------------------------------
// Controller Status tile






#if DISPLAY_SERVO_MONITOR == ON

extern void servoTile(String &data);
extern void servoTileAjax(String &data);
extern void servoTileGet();

const char html_servoSelect[] PROGMEM = "<button id='svax%d' onpointerdown=\"s('svax','%d')\" type='button' class='bb'>%c</button>";

const char html_servoGraph[] PROGMEM = "<canvas id='servoCanvas' width='420' height='300' style='border:1px solid #000000;'></canvas> \n";

const char html_servoScript1[] PROGMEM =
  "<script>\n"

  "var svu=setInterval(updateServo,500);"
  "var svr=false;" // has run?
  "var svt=0;"     // time (x axis, 0.5s ticks)
  "var svd=[];"    // array of servo deltas
  "var svhs=[4,10,20,50,100,500,1000,5000,10000,50000,100000,500000,1000000,500000,10000000,50000000,100000000,500000000];"
  "var svs=10;"    // scale (y axis)
  "var svw=210;"   // half width
  "var svh=300;"   // height
  "var svx=0;"
  "var svy=0;\n"

  "function updateServo() {\n"
    "var canvas=document.getElementById('servoCanvas');"
    "var ctx=canvas.getContext('2d');\n"

    "var deltaText = document.getElementById('svoD').innerText;"
    "if (deltaText=='?') { inactiveServo(ctx); svr=false; return; }"
    "var delta=Number(deltaText);"

    "if (!svr) { for (i=0;i<svw;i++) svd[i]=0; svr=true; svt=0; svx=0; svy=0; clearServo(ctx); }\n"

    "svt++;"
    "var x=svt;"

    "if (x>=svw-1) { x=svw-1; for (i=1;i<=x;i++) { svd[i-1]=svd[i]; } clearServo(ctx); }\n"
    "svd[x]=delta;\n";

  const char html_servoScript2[] PROGMEM =
    "var max=0;"
    "for (i=0;i<=x;i++) { if (Math.abs(svd[i])>max) max=Math.abs(svd[i]); }"

    "var osvs=svs;"
    "for (i=0;i<=18;i++) { if (max<svhs[i]*0.9) { svs=svhs[i]; break; } }"
    "if (osvs!=svs) { clearServo(ctx); }\n"

    "y=svh/2-delta*((svh/2)/svs);"
    "ctx.beginPath(); ctx.moveTo(svx*2,svy); ctx.lineTo(x*2,y); ctx.strokeStyle='" COLOR_SERVO_PEN_4 "'; ctx.stroke();"
    "svx=x; svy=y;"
  "}\n"

  "function inactiveServo(ctx) {\n"
    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_1 "';"
    "ctx.fillRect(0,0,svw*2,svh);"

    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_2 "';"
    "ctx.font = 'bold 36px Arial';"
    "ctx.fillText('Inactive',svw-70,svh/2-9);"
  "}"

  "function clearServo(ctx) {\n"
    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_1 "';"
    "ctx.fillRect(0,0,svw*2,svh);"

    "ctx.strokeStyle='" COLOR_SERVO_PEN_3 "';"
    "ctx.beginPath(); ctx.moveTo(0, svh/2); ctx.lineTo(svw*2, svh/2); ctx.stroke();";

  const char html_servoScript3[] PROGMEM =

    "ctx.strokeStyle='" COLOR_SERVO_PEN_2 "';"
    "ctx.beginPath();"
    "ctx.moveTo(0,svh*0.25); ctx.lineTo(svw*2,svh*0.25);"
    "ctx.moveTo(0,svh*0.75); ctx.lineTo(svw*2,svh*0.75);"
    "ctx.stroke();"

    "ctx.strokeStyle='" COLOR_SERVO_PEN_1 "';"
    "ctx.beginPath();"
    "for (i=60; i<svw*2; i+=60) { ctx.moveTo(i,0); ctx.lineTo(i,svh); }"
    "ctx.stroke();"

    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_3 "';"
    "ctx.font = 'bold 12px Arial';"
    "ctx.textBaseline = 'top';"
    "ctx.fillText(svs.toString(),2,2);"
    "ctx.fillText(svs.toString()/2,2,svh*0.25+2);"
    "svs=-svs; ctx.fillText(svs.toString()/2,2,svh*0.75+2); svs=-svs;"
    "ctx.fillText('0',2,svh/2+2);"

    "ctx.strokeStyle='" COLOR_SERVO_PEN_4 "';"
    "ctx.beginPath(); svx=0; svy=svh/2+svd[0]/svs;"
    "for (i=0;i<=svt;i++) { ctx.moveTo(svx*2,svy); svx=i; svy=svh/2-svd[i]*((svh/2)/svs); ctx.lineTo(svx*2,svy); }"
    "ctx.stroke();"

  "}\n"

  "</script>\n";
#endif


PREVIEW_FILE_8
// -----------------------------------------------------------------------------------
// StallGuard monitor tile






#if DISPLAY_STALLGUARD_MONITOR == ON

extern void stallGuardTile(String &data);
extern void stallGuardTileAjax(String &data);
extern void stallGuardTileGet();

const char html_stallGuardSelect[] PROGMEM = "<button id='sgax%d' onpointerdown=\"s('sgax','%d')\" type='button' class='bb'>%c</button>";

const char html_stallGuardGraph[] PROGMEM = "<canvas id='stallGuardCanvas' width='420' height='220' style='border:1px solid #000000;'></canvas> \n";

const char html_stallGuardScript_1[] PROGMEM =
  "<script>\n"
  "var sgu=setInterval(updateStallGuard,500);"
  "var sgr=false;"
  "var sgtm=0;"
  "var sgData=[];"
  "var sgTrip=[];"
  "var sgw=210;"
  "var sgh=220;"
  "var sgx=0;"
  "var sgy=0;"
  "var sgty=0;"
  "var sglastTrip=-1;\n"

  "function updateStallGuard(){"
    "var canvas=document.getElementById('stallGuardCanvas');"
    "var ctx=canvas.getContext('2d');"
    "var sgText=document.getElementById('sgV').innerText;"
    "var tripText=document.getElementById('sgT').innerText;"
    "var sg=Number(sgText);"
    "var trip=Number(tripText);"
    "var sgValid=(sgText!='?'&&!isNaN(sg)&&sg>=0);"
    "var tripValid=(tripText!='?'&&!isNaN(trip)&&trip>=0);"
    "if(!sgr){for(i=0;i<sgw;i++){sgData[i]=-1;sgTrip[i]=-1;}sgr=true;sgtm=0;sgx=0;sgy=sgh;sgty=sgh;sglastTrip=-1;clearStallGuard(ctx);}"
    "if(tripValid){sglastTrip=trip;}else{trip=sglastTrip;tripValid=(trip>=0);}"
    "sgtm++;"
    "var x=sgtm;";

const char html_stallGuardScript_2[] PROGMEM =
    "if(x>=sgw-1){x=sgw-1;for(i=1;i<=x;i++){sgData[i-1]=sgData[i];sgTrip[i-1]=sgTrip[i];}sgtm=x;clearStallGuard(ctx);sgx=x-1;if(sgData[sgx]>=0){sgy=sgh-sgData[sgx]*(sgh/1023);}if(sgTrip[sgx]>=0){sgty=sgh-sgTrip[sgx]*(sgh/1023);}}"
    "sgData[x]=sgValid?sg:-1;"
    "sgTrip[x]=tripValid?trip:-1;"
    "if(sgValid){var y=sgh-sg*(sgh/1023);if(sgData[sgx]>=0){ctx.beginPath();ctx.moveTo(sgx*2,sgy);ctx.lineTo(x*2,y);ctx.strokeStyle='yellow';ctx.lineWidth=1;ctx.stroke();}sgy=y;}"
    "if(tripValid){var ty=sgh-trip*(sgh/1023);if(sgTrip[sgx]>=0){ctx.beginPath();ctx.moveTo(sgx*2,sgty);ctx.lineTo(x*2,ty);ctx.strokeStyle='orange';ctx.lineWidth=1;ctx.stroke();}sgty=ty;}"
    "sgx=x;"
  "}\n"

  "function clearStallGuard(ctx){"
    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_1 "';"
    "ctx.fillRect(0,0,sgw*2,sgh);"
    "ctx.strokeStyle='" COLOR_SERVO_PEN_1 "';"
    "ctx.beginPath();"
    "for(i=60;i<sgw*2;i+=60){ctx.moveTo(i,0);ctx.lineTo(i,sgh);}"
    "ctx.stroke();"
    "ctx.strokeStyle='" COLOR_SERVO_PEN_3 "';"
    "ctx.beginPath();ctx.moveTo(0,sgh/2);ctx.lineTo(sgw*2,sgh/2);ctx.stroke();"
    "ctx.strokeStyle='" COLOR_SERVO_PEN_2 "';";

const char html_stallGuardScript_3[] PROGMEM =
    "ctx.beginPath();ctx.moveTo(0,sgh*0.25);ctx.lineTo(sgw*2,sgh*0.25);ctx.moveTo(0,sgh*0.75);ctx.lineTo(sgw*2,sgh*0.75);ctx.stroke();"
    "ctx.fillStyle='" COLOR_SERVO_BACKGROUND_3 "';"
    "ctx.font='bold 12px Arial';"
    "ctx.textBaseline='top';"
    "ctx.fillText('1023',2,2);"
    "ctx.fillText('768',2,sgh*0.25+2);"
    "ctx.fillText('512',2,sgh/2+2);"
    "ctx.fillText('256',2,sgh*0.75+2);"
    "ctx.fillText('SG',sgw*2-52,2);"
    "ctx.fillText('Trip',sgw*2-52,18);"
    "ctx.fillStyle='yellow';ctx.fillRect(sgw*2-72,6,14,2);"
    "ctx.fillStyle='orange';ctx.fillRect(sgw*2-72,21,14,2);"
    "ctx.strokeStyle='yellow';ctx.lineWidth=1;"
    "ctx.beginPath();"
    "for(i=1;i<=sgtm;i++){if(sgData[i-1]>=0&&sgData[i]>=0){ctx.moveTo((i-1)*2,sgh-sgData[i-1]*(sgh/1023));ctx.lineTo(i*2,sgh-sgData[i]*(sgh/1023));}}ctx.stroke();"
    "ctx.strokeStyle='orange';ctx.lineWidth=1;"
    "ctx.beginPath();"
    "for(i=1;i<=sgtm;i++){if(sgTrip[i-1]>=0&&sgTrip[i]>=0){ctx.moveTo((i-1)*2,sgh-sgTrip[i-1]*(sgh/1023));ctx.lineTo(i*2,sgh-sgTrip[i]*(sgh/1023));}}ctx.stroke();ctx.lineWidth=1;"
  "}\n"
  "</script>\n";

#endif


PREVIEW_FILE_9
// -----------------------------------------------------------------------------------
// Controller Status tile






extern void statusTile(String &data);
extern void statusTileAjax(String &data);
extern void statusTileGet();

const char html_indexGeneralError[] PROGMEM = "<div class='sws-metric'><span>" L_LAST_GENERAL_ERROR "</span><strong id='last_err' class='c'>%s</strong></div>";

const char html_indexWorkload[] PROGMEM = "<div class='sws-metric'><span>" L_WORKLOAD "</span><strong id='work_load' class='c'>%s</strong></div>";

#if DISPLAY_INTERNAL_TEMPERATURE == ON
  const char html_indexTemp[] PROGMEM = "<div class='sws-metric'><span>%s</span><strong id='tphd_%c' class='c'>%s</strong></div>";
#endif

#if OPERATIONAL_MODE == WIFI
  const char html_indexSignalStrength[] PROGMEM = "<div class='sws-metric'><span>" L_WIRELESS_SIGNAL_STRENGTH "</span><strong id='signal' class='c'>%s</strong></div>";
#endif

const char html_indexMountType[] PROGMEM =
  L_PAGE_MOUNT L_ADV_MOUNT_TYPE ":<br /><input style='width: 7em;' value='%d' type='number' name='mountt' min='1' max='3' step='1'>"
  "<br /><br />\n";

const char html_resetNotes[] PROGMEM =
  "Notes:<ul>"
  "<li>" L_RESET_MSG1 "</li>"
  "<li>" L_DOWN_MESSAGE1 "</li>"
  "</ul>\n";


PREVIEW_FILE_10
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


PREVIEW_FILE_11




// 严格检查整数坐标，禁止空值、尾随字符和越界值进入设备协议
inline bool coordinateInteger(const char *value, int maximum, int &number) {
  if (!value || !*value) return false;
  number = 0;
  for (const char *p = value; *p; ++p) {
    if (*p < '0' || *p > '9') return false;
    number = number * 10 + (*p - '0');
    if (number > maximum) return false;
  }
  return true;
}
inline bool coordinateCommands(const char *const values[7], char *ra, char *dec) {
  int h, m, s, d, dm, ds;
  if (!coordinateInteger(values[0], 23, h) || !coordinateInteger(values[1], 59, m) ||
      !coordinateInteger(values[2], 59, s) || !coordinateInteger(values[4], 90, d) ||
      !coordinateInteger(values[5], 59, dm) || !coordinateInteger(values[6], 59, ds)) return false;
  if ((strcmp(values[3], "+") && strcmp(values[3], "-")) || (d == 90 && (dm || ds))) return false;
  snprintf(ra, 24, ":Sr%02d:%02d:%02d#", h, m, s);
  snprintf(dec, 24, ":Sd%c%02d*%02d:%02d#", values[3][0], d, dm, ds);
  return true;
}


PREVIEW_FILE_12
// -----------------------------------------------------------------------------------
// Coordinate basis tile






#if DISPLAY_COORDINATE_ORIGIN == ON
  extern void coordinateTile(String &data);
  extern void coordinateTileGet();

  const char html_coordinateBasis[] PROGMEM =
    L_COORD_ORIGIN_MSG
    "<br /><br />"
    "<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('ob','set')\" type='button'>" L_COORD_ORIGIN "</button>"
    "<br />\n";
#endif


PREVIEW_FILE_13
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
"<form id='sws-coordinate-form' class='sws-coordinate-form' data-send='" L_COORD_SEND "' data-ok='" L_COORD_OK "' data-invalid='" L_COORD_INVALID "' data-reject='" L_COORD_REJECT "' data-network='" L_COORD_NETWORK "'>\n<div class='sws-coordinate-grid'><label class='sws-coordinate-system'><span>" L_COORD_SYSTEM "</span><input value='" L_COORD_JNOW "' readonly></label><div class='sws-coordinate-actions'><button type='submit' name='coord_action' value='go'>" L_GOTO_ACTION "</button><button class='sws-secondary' type='submit' name='coord_action' value='sync'>" L_COORD_SYNC "</button></div><fieldset><legend>" L_COORD_RA "</legend><div class='sws-coordinate-units'><label class='sws-coordinate-unit'><input name='ra_h' type='number' min='0' max='23' step='1' inputmode='numeric' required aria-label='" L_COORD_H "'><span>" L_COORD_H "</span></label><label class='sws-coordinate-unit'><input name='ra_m' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_M "'><span>" L_COORD_M "</span></label><label class='sws-coordinate-unit'><input name='ra_s' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_S "'><span>" L_COORD_S "</span></label></div></fieldset><fieldset><legend>" L_COORD_DEC "</legend><div class='sws-coordinate-units'><select name='dec_sign' aria-label='" L_COORD_SIGN "'><option value='+'>+</option><option value='-'>−</option></select><label class='sws-coordinate-unit'><input name='dec_d' type='number' min='0' max='90' step='1' inputmode='numeric' required aria-label='" L_COORD_D "'><span>" L_COORD_D "</span></label><label class='sws-coordinate-unit'><input name='dec_m' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_M "'><span>" L_COORD_M "</span></label><label class='sws-coordinate-unit'><input name='dec_s' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_S "'><span>" L_COORD_S "</span></label></div></fieldset></div><p role='status' class='sws-coordinate-message'></p></form>";

extern void gotoCoordinatesGet();


PREVIEW_FILE_14
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
"<div class='sws-choice-source sws-guide-choice' data-label='" L_GUIDE " " L_RATE "'>"
"<button id='guide_r0' class='btns_right' onpointerdown=\"g('R0')\" type='button'>0.25</button>"
"<button id='guide_r1' class='btns_mid' onpointerdown=\"g('R1')\" type='button'>0.5</button>"
"<button id='guide_r2' class='btns_left' onpointerdown=\"g('R2')\" type='button'>1x</button>"
"</div>";

const char html_guideRates[] PROGMEM =
"<div class='sws-choice-source sws-guide-choice' data-label='" L_CONTROLS " " L_RATE "'>"
"<button id='guide_r3' class='btns_right' onpointerdown=\"g('R3')\" type='button'>2x</button>"
"<button id='guide_r4' class='btns_mid' onpointerdown=\"g('R4')\" type='button'>4x</button>"
"<button id='guide_r5' class='btns_mid' onpointerdown=\"g('R5')\" type='button'>8x</button>"
"<button id='guide_r6' class='btns_mid' onpointerdown=\"g('R6')\" type='button'>20x</button>"
"<button id='guide_r7' class='btns_mid' onpointerdown=\"g('R7')\" type='button'>48x</button>"
"<button id='guide_r8' class='btns_mid' onpointerdown=\"g('R8')\" type='button'>VF</button>"
"<button id='guide_r9' class='btns_left' onpointerdown=\"g('R9')\" type='button'>VVF</button>"
"</div>";


PREVIEW_FILE_15
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


PREVIEW_FILE_16
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


PREVIEW_FILE_17
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



PREVIEW_FILE_18
// -----------------------------------------------------------------------------------
// Control








#if DISPLAY_COORDINATE_ORIGIN == ON

#endif









PREVIEW_FILE_19
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


PREVIEW_FILE_20
// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Date/Time/Location tile






extern void siteTile(String &data);
extern void siteTileAjax(String &data);
extern void siteTileGet();

const char html_browserTime[] PROGMEM =
  "<p class='sws-browser-clock'><span>" L_TIME_BROWSER_UT "</span><strong id='datetime'></strong><script>update_date_time();</script></p>";
const char html_date[] PROGMEM = "<div class='sws-site-clock'><div class='sws-clock-value'><span class='sws-field-label'>" L_UT "</span><strong><span id='date_ut' class='c'>%s</span>";
const char html_time[] PROGMEM = " <span id='time_ut' class='c'>%s</span></strong></div>";
const char html_sidereal[] PROGMEM = "<div class='sws-sidereal-clock'><span>" L_LST "</span><strong id='time_lst' class='c'>%s</strong></div></div>";
const char html_site[] PROGMEM =
  "<div hidden><span id='site_lat'>%s</span><span id='site_long'>%s</span></div>";
const char html_setDateTime[] PROGMEM =
  "<form style='display: inline' method='get' action='/mount.htm'>\n"
  "<div class='sws-clock-action'>"
  "<small><button name='qb' class='bb' value='st' onpointerdown='SetDateTime();' type='submit'>" L_SYNC_TIME "</button></small>"
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


PREVIEW_FILE_21
// -----------------------------------------------------------------------------------
// Tracking tile






extern void trackingTile(String &data);
extern void trackingTileAjax(String &data);
extern void trackingTileGet();

const char html_trackingEnable[] PROGMEM = 
"<button id='trk_on' data-label='" L_DEVICE_TRACKING "' class='btns_right' onpointerdown=\"s('dr','T1')\" type='button'>" L_ON "</button>"
"<button id='trk_off' class='btns_left' onpointerdown=\"s('dr','T0')\" type='button'>" L_OFF "</button>";

const char html_trackingPreset[] PROGMEM =
"<div class='sws-choice-source' data-label='" L_TRACKING " " L_RATE "'>"
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


PREVIEW_FILE_22
// -----------------------------------------------------------------------------------
// Focuser Backlash tile






extern void focuserBacklashTcfTile(String &data);
extern void focuserBacklashTcfTileAjax(String &data);
extern void focuserBacklashTcfTileGet();

const char html_backlashValue[] PROGMEM =
"<div><span>" L_BACKLASH "</span><strong id='f_bl' class='c'>%s " L_UNIT_STEPS "</strong></div>";

const char html_tcfDeadbandValue[] PROGMEM =
"<div><span>" L_DEADBAND "</span><strong id='f_tcf_db' class='c'>%s " L_UNIT_STEPS "</strong></div>";

const char html_tcfEnableValue[] PROGMEM =
"<div><span>" L_TCF_COEF_EN "</span><strong id='f_tcf_en' class='c'>%s</strong></div>";

const char html_tcfCoefValue[] PROGMEM =
"<div><span>" L_TCF_COEF "</span><strong id='f_tcf_coef' class='c'>%s</strong></div>";

const char html_backlash[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_BACKLASH_RANGE_AXIS3UP "</span><input value='%s' type='number' name='foc_bl' min='0' max='32767' step='1' required></label>";

const char html_tcfDeadband[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_DEADBAND_RANGE_AXIS4UP "</span><input value='%s' type='number' name='foc_tcf_db' min='1' max='32767' step='1' required></label>";

const char html_tcfEnable[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_TCF_COEF_EN "</span><select name='foc_tcf_en'><option value='0' %s>" L_OFF "</option><option value='1' %s>" L_ON "</option></select></label>";

const char html_tcfCoef[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_TCF_COEF_RANGE_AXIS4UP "</span><input value='%s' type='number' name='foc_tcf_coef' min='-999.0' max='999.0' step='any' required></label>";


PREVIEW_FILE_23
// -----------------------------------------------------------------------------------
// Settings












PREVIEW_FILE_24
// -----------------------------------------------------------------------------------
// Focuser Home tile






extern void focuserHomeTile(String &data);
extern void focuserHomeTileAjax(String &data);
extern void focuserHomeTileGet();

const char html_focuserHome[] PROGMEM =
  "<button onpointerdown=\"gf('Fh')\" type='button'>"
  L_RETURN_HOME "</button>"
  "<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) gf('FH')\" type='button'>"
  L_RESET_HOME "</button>";


PREVIEW_FILE_25
// -----------------------------------------------------------------------------------
// Focuser Select tile






extern void focuserSelectTile(String &data);
extern void focuserSelectTileAjax(String &data);
extern void focuserSelectTileGet();

const char html_selectFocuser1[] PROGMEM =
"<button id='foc1_sel' class='bbh' onpointerdown=\"gf('F1')\" title='" L_HINT_FOC_SEL " 1' type='button'>1</button>";
const char html_selectFocuser2[] PROGMEM =
"<button id='foc2_sel' class='bbh' onpointerdown=\"gf('F2')\" title='" L_HINT_FOC_SEL " 2' type='button'>2</button>";
const char html_selectFocuser3[] PROGMEM =
"<button id='foc3_sel' class='bbh' onpointerdown=\"gf('F3')\" title='" L_HINT_FOC_SEL " 3' type='button'>3</button>";
const char html_selectFocuser4[] PROGMEM =
"<button id='foc4_sel' class='bbh' onpointerdown=\"gf('F4')\" title='" L_HINT_FOC_SEL " 4' type='button'>4</button>";
const char html_selectFocuser5[] PROGMEM =
"<button id='foc5_sel' class='bbh' onpointerdown=\"gf('F5')\" title='" L_HINT_FOC_SEL " 5' type='button'>5</button>";
const char html_selectFocuser6[] PROGMEM =
"<button id='foc6_sel' class='bbh' onpointerdown=\"gf('F6')\" title='" L_HINT_FOC_SEL " 6' type='button'>6</button>";


PREVIEW_FILE_26
// -----------------------------------------------------------------------------------
// Focuser Slewing tile






extern void focuserSlewingTile(String &data);
extern void focuserSlewingTileAjax(String &data);
extern void focuserSlewingTileGet();

const char html_focPosition[] PROGMEM =
"<label class='sws-accessory-target'><span>" L_TARGET "</span><span class='sws-accessory-input'><input id='foc_set' value='0' type='number' min='-500000' max='500000' step='1' required><span>µm</span></span></label>"
"<button class='bbh' onpointerdown=\"{const e=document.getElementById('foc_set');if(e.reportValidity())s('fs',e.value);}\" type='button'>" L_SET_LABEL "</button>";

const char html_focStop[] PROGMEM =
"<button class='bbh sws-accessory-stop' onpointerdown=\"if(window.fmc)fmc.halt();else s('foc','stop')\" type='button' title='" L_HINT_EMERGENCY_STOP "'>" L_STOP "</button>";

const char html_focuserIn[] PROGMEM =
  "<script>"
  "var fmc=motionControls({p:'focuser-ajax-get.txt',k:'foc',s:'stop',g:1});"
  "function ft(e,v){fmc.t(e,v);}"
  "function fu(e,v){fmc.u(e,v);}"
  "function fp(e,v){fmc.p(e,v);}"
  "function fr(e,v){fmc.r(e,v);}"
  "function fc(e){return fmc.c(e);}"
  "</script>"
  "<button class='bbh' ontouchstart=\"ft(event,'inf')\" ontouchend=\"fu(event,'inf')\" ontouchcancel=\"fu(event,'inf')\" ontouchmove=\"return fc(event)\" onpointerdown=\"fp(event,'inf')\" onpointerup=\"fr(event,'inf')\" onpointercancel=\"fr(event,'inf')\" oncontextmenu=\"return fc(event)\" type='button'><span>" ARROW_DD "</span><small>" L_FOC_IN "</small></button>"
  "<button class='bbh' ontouchstart=\"ft(event,'in')\" ontouchend=\"fu(event,'in')\" ontouchcancel=\"fu(event,'in')\" ontouchmove=\"return fc(event)\" onpointerdown=\"fp(event,'in')\" onpointerup=\"fr(event,'in')\" onpointercancel=\"fr(event,'in')\" oncontextmenu=\"return fc(event)\" type='button'><span>" ARROW_D "</span><small>" L_FOC_IN "</small></button>";

const char html_focuserOut[] PROGMEM =
  "<button class='bbh' ontouchstart=\"ft(event,'out')\" ontouchend=\"fu(event,'out')\" ontouchcancel=\"fu(event,'out')\" ontouchmove=\"return fc(event)\" onpointerdown=\"fp(event,'out')\" onpointerup=\"fr(event,'out')\" onpointercancel=\"fr(event,'out')\" oncontextmenu=\"return fc(event)\" type='button'><span>" ARROW_U "</span><small>" L_FOC_OUT "</small></button>"
  "<button class='bbh' ontouchstart=\"ft(event,'outf')\" ontouchend=\"fu(event,'outf')\" ontouchcancel=\"fu(event,'outf')\" ontouchmove=\"return fc(event)\" onpointerdown=\"fp(event,'outf')\" onpointerup=\"fr(event,'outf')\" onpointercancel=\"fr(event,'outf')\" oncontextmenu=\"return fc(event)\" type='button'><span>" ARROW_UU "</span><small>" L_FOC_OUT "</small></button>";

const char html_focuserSlewSpeed[] PROGMEM =
"<div class='sws-accessory-live-rate'><span>" L_POSITIONING_SPEED "</span><strong id='foc_rate' class='c'>%s</strong></div>";

const char html_focuserGotoSelect[] PROGMEM =
"<div class='sws-choice-source' data-label='" L_POSITIONING_RATE "'>"
  "<button id='foc_rate_vf' class='btns_right' onpointerdown=\"s('foc','vf')\" type='button'>" L_VFAST "</button>"
  "<button id='foc_rate_f' class='btns_mid' onpointerdown=\"s('foc','f')\"  type='button'>" L_FAST "</button>"
  "<button id='foc_rate_n' class='btns_mid' onpointerdown=\"s('foc','n')\"  type='button'>" L_NORMAL "</button>"
  "<button id='foc_rate_s' class='btns_mid' onpointerdown=\"s('foc','s')\"  type='button'>" L_SLOW "</button>"
  "<button id='foc_rate_vs' class='btns_left' onpointerdown=\"s('foc','vs')\" type='button'>" L_VSLOW "</button>"
  "</div>\n";


PREVIEW_FILE_27
// -----------------------------------------------------------------------------------
// Rotator Backlash tile






extern void rotatorBacklashTile(String &data);
extern void rotatorBacklashTileAjax(String &data);
extern void rotatorBacklashTileGet();

const char html_backlashValue[] PROGMEM =
"<div class='sws-accessory-metrics'><div><span>" L_BACKLASH "</span><strong class='c'>%d " L_UNIT_STEPS "</strong></div></div>";

const char html_configBlAxis3[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_BACKLASH_RANGE_AXIS3UP "</span><input value='%d' type='number' name='b3' min='0' max='32767' required></label>";


PREVIEW_FILE_28
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


PREVIEW_FILE_29
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


PREVIEW_FILE_30
// -----------------------------------------------------------------------------------
// Rotator












PREVIEW_FILE_31
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


PREVIEW_FILE_32
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


PREVIEW_FILE_33
// -----------------------------------------------------------------------------------
// Encoders






#if ENCODERS == ON


#endif


PREVIEW_FILE_34
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


PREVIEW_FILE_35
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


PREVIEW_FILE_36
// -----------------------------------------------------------------------------------
// Library help






const char html_libCatalogHelp1[] PROGMEM =
"Data format:<br /><pre>"
"Object Name Cat    RA       Dec   <br />"
"ccccccccccc,ccc,HH:MM:SS,sDD*MM:SS<br /></pre>"
"Sample data:<br /><pre>"
"$PlaNeb1<br />"
"M97        ,PN ,11:15:53,+54*55:24<br />"
"M27        ,PN ,20:00:26,+22*46:29<br />"
"</pre>"
L_CAT_EXAMPLE1 L_CAT_EXAMPLE2 L_CAT_EXAMPLE3 L_CAT_EXAMPLE4 L_CAT_EXAMPLE5
"  UNK = Unknown<br />"
"  OC  = Open Cluster<br />"
"  GC  = Globular Cluster<br />";
const char html_libCatalogHelp2[] PROGMEM =
"  PN  = Planetary Nebula<br />"
"  DN  = Dark Nebula<br />"
"  SG  = Spiral Galaxy<br />"
"  EG  = Elliptical Galaxy<br />"
"  IG  = Irregular Galaxy<br />"
"  KNT = Knot<br />"
"  SNR = Supernova Remnant<br />"
"  GAL = Galaxy<br />"
"  CN  = Cluster w/Nebula<br />"
"  STR = Star<br />"
"  PLA = Planet<br />"
"  CMT = Comet<br />"
"  AST = Asteroid<br /><br />"
L_CAT_EXAMPLE6 L_CAT_EXAMPLE7
"<br /></pre>\n";


PREVIEW_FILE_37




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


PREVIEW_FILE_38



// 只负责排版与外观，不读取或发送设备命令。
const char html_shared_styles[] PROGMEM = R"UI(
<style>
:root {
  color-scheme: dark;
  --bg: )UI" COLOR_BODY_BACKGROUND R"UI(;
  --panel: )UI" SWS_COLOR_PANEL R"UI(;
  --field: )UI" COLOR_LIGHT_BACKGROUND R"UI(;
  --border: )UI" COLOR_BORDER R"UI(;
  --accent: )UI" COLOR_CONTROL R"UI(;
  --sidebar-width: 156px;
  --nav-active: )UI" COLOR_LINK_SELECTED_BACKGROUND R"UI(;
  --text: )UI" SWS_COLOR_TEXT R"UI(;
  --muted: )UI" SWS_COLOR_MUTED R"UI(;
  --readout: )UI" SWS_COLOR_READOUT R"UI(;
}

* {
  box-sizing: border-box;
}
body {
  margin: 0;
  padding: 16px;
  background: var(--bg);
  color: var(--text);
  font:
    15px/1.6 system-ui,
    -apple-system,
    'Segoe UI',
    sans-serif;
}

.t,
.b {
  width: 100%;
  min-width: 0;
  max-width: none;
  margin: 0;
  padding: 20px;
  border: 1px solid var(--border);
  background: #131313;
  color: var(--text);
}

.t {
  border-radius: 16px 16px 0 0;
}
.b {
  border-top: 0;
  border-radius: 0 0 16px 16px;
  display: flex;
  flex-wrap: wrap;
  align-items: flex-start;
  gap: 16px;
}

.t table {
  width: 100%;
}
.t td {
  overflow-wrap: anywhere;
}
.t font {
  font-size: 24px;
}
.t a:link,
.t a:visited {
  border: 1px solid var(--border);
  background: #1f1f1f;
  color: var(--muted);
  border-radius: 9px;
  padding: 7px 14px;
  margin: 10px 6px 0 0;
  text-decoration: none;
}

.t a:hover {
  background: var(--field) !important;
  color: var(--text);
}
.t a:focus-visible {
  outline: 2px solid #9b9b9b;
  outline-offset: 3px;
}

.b1 {
  float: none;
  flex: 1 1 310px;
  width: auto !important;
  max-width: 100%;
  min-width: 0;
  min-height: 0 !important;
  margin: 0;
  padding: 18px;
  border: 1px solid var(--border);
  border-radius: 14px;
  background: var(--panel);
  overflow-x: auto;
}

.b1 > div:first-child {
  font-weight: 600;
}
.b1 table {
  max-width: 100%;
}
.b1 canvas {
  display: block;
  max-width: 100%;
  height: auto;
}
.clear {
  flex-basis: 100%;
  height: 0;
}
.b > hr {
  width: 100%;
}

input,
select,
textarea,
button {
  font: inherit;
  max-width: 100%;
  border: 1px solid #4c4c4c;
  border-radius: 8px;
  color: var(--text);
  box-shadow: none;
}

input,
select,
textarea {
  text-align: left;
  background: var(--field);
  padding: 8px 10px;
  margin: 3px;
  font-weight: 400;
}
input[type='number'] {
  min-width: 5.5em;
  text-align: left;
}
input[type='checkbox'],
input[type='radio'] {
  width: 18px;
  height: 18px;
  accent-color: #8b2020;
  vertical-align: middle;
  padding: 0;
}

button {
  min-height: 40px;
  background: var(--accent);
  padding: 8px 14px;
  margin: 3px;
  cursor: pointer;
  font-weight: 500;
}
button:hover:not(:disabled) {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
button[data-selected='true']:not(:disabled) {
  background-image: linear-gradient(rgb(185 185 185 / 22%), rgb(185 185 185 / 22%)) !important;
  box-shadow: none;
}
button:not(:disabled):active {
  background-image: linear-gradient(rgb(185 185 185 / 32%), rgb(185 185 185 / 32%)) !important;
  box-shadow: none;
}
button:disabled {
  background: #292929;
  color: #7e7e7e;
  border-color: #3b3b3b;
  cursor: default;
}

input:focus-visible,
select:focus-visible,
textarea:focus-visible,
button:focus-visible,
summary:focus-visible {
  outline: 2px solid #9b9b9b;
  outline-offset: 2px;
}
hr {
  border: 0;
  border-top: 1px solid var(--border);
  margin: 16px 0;
}

a:link,
a:visited {
  color: #a8a8a8;
  background: transparent;
  border-color: var(--border);
}
.c {
  color: var(--readout);
}
.y {
  color: #d6b786;
}
.gb {
  font-family: inherit;
  border-radius: 10px;
}
.collapsible {
  width: 100%;
  text-align: left;
  background: #252525;
  border: 1px solid var(--border);
  border-radius: 9px;
  padding: 10px;
  color: #b5b5b5;
}
.active,
.collapsible:hover {
  background: #303030;
}
.content {
  background: #1c1c1c;
  border-radius: 0 0 9px 9px;
  padding: 12px;
}

.sws-network-layout {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  width: 100%;
  min-width: 0;
}
.sws-network-layout > .nw-grid {
  grid-column: 1/-1;
  margin: 0;
}
.sws-network-layout > .b1 {
  width: 100% !important;
}
.sws-network-layout > #nw-message {
  grid-column: 1/-1;
  margin: 0;
}
.sws-account {
  grid-column: 1/-1;
  border-top: 1px solid var(--border);
  padding-top: 12px;
}

html.sws-embedded body {
  padding: 0;
}
html.sws-embedded .t {
  display: none;
}
html.sws-embedded .b {
  border: 0;
  border-radius: 0;
  padding: 20px;
}

#sws-frame {
  display: block;
  width: 100%;
  border: 1px solid var(--border);
  border-top: 0;
  border-radius: 0 0 16px 16px;
  background: #131313;
}
#sws-load {
  margin: 0;
  color: var(--muted);
  font-size: 13px;
}
#sws-load:empty {
  display: none;
}
html.sws-shell body {
  overflow: hidden;
}

@media (min-width: 1500px) {
  .b1 {
    flex-basis: 340px;
  }
}

@media (max-width: 700px) {
  body {
    padding: 8px;
  }
  .t,
  .b,
  html.sws-embedded .b {
    padding: 14px;
  }
  .t td {
    display: block;
    text-align: left;
  }
  .t td:last-child {
    font-size: 12px;
  }
  .t a:link,
  .t a:visited {
    padding: 7px 10px;
    margin-right: 4px;
  }
  .b1 {
    flex-basis: 100%;
    padding: 14px;
  }
  .sws-network-layout {
    grid-template-columns: 1fr;
  }
  .sws-network-layout > .b1 {
    grid-column: 1;
  }
  .nw-grid {
    grid-template-columns: 1fr !important;
  }
}

/* 桌面使用侧栏与内容区，手机使用底部导航并为触控留出空间 */
html.sws-shell body {
  padding: 0;
  height: 100dvh;
  overflow: hidden;
}
.sws-shell .t {
  position: fixed;
  inset: 0 auto 0 0;
  width: var(--sidebar-width);
  border: 0;
  border-right: 1px solid #292929;
  border-radius: 0;
  background: #141414;
  padding: 30px 16px;
  display: flex;
  flex-direction: column;
  gap: 30px;
}
.sws-shell .t table,
.sws-shell .t tbody,
.sws-shell .t tr,
.sws-shell .t td {
  display: block;
  width: 100%;
  text-align: center;
}
.sws-shell .t td:last-child {
  font-size: 11px;
  font-weight: 400;
  color: #808080;
  margin-top: 9px;
}
.sws-shell .t font {
  font-size: 24px;
}
.sws-nav {
  display: flex;
  flex-direction: column;
  gap: 10px;
}
.sws-shell .t a:link,
.sws-shell .t a:visited {
  display: flex;
  align-items: center;
  gap: 12px;
  margin: 0;
  padding: 14px;
  border: 0;
  border-radius: 10px;
  background: transparent;
  color: #9f9f9f;
  font-size: 14px;
}
.sws-shell .t a[aria-current='page'] {
  background: var(--nav-active) !important;
  color: #fff;
  box-shadow: none;
}
@media (min-width: 701px) {
  .sws-nav {
    flex: 1;
    min-height: 0;
    align-items: center;
    overflow-y: auto;
  }
  .sws-shell .t a:link,
  .sws-shell .t a:visited {
    flex-shrink: 0;
    flex-direction: column;
    justify-content: center;
    width: 112px;
    min-height: 84px;
    gap: 6px;
    padding: 12px 8px;
    border-radius: 12px;
    text-align: center;
    line-height: 1.4;
    font-size: 13px;
  }
}
.sws-nav svg {
  width: 28px;
  height: 28px;
  fill: currentColor;
  stroke: none;
  flex-shrink: 0;
}
.sws-sidebar-note {
  width: 100%;
  text-align: center;
  margin-top: auto;
  color: var(--muted);
  font-size: 12px;
  line-height: 1.8;
}

.sws-topbar {
  margin-left: var(--sidebar-width);
  padding: 25px 30px 20px;
  background: #101010;
  border-bottom: 1px solid #242424;
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 20px;
}
.sws-topbar h1 {
  margin: 0;
  color: #e2e2e2;
  font-size: 25px;
  font-weight: 600;
  text-align: left;
}
.sws-topbar p {
  margin: 5px 0 0;
  color: #8c8c8c;
  font-size: 13px;
}
.sws-brand-small {
  font-size: 12px;
  color: #959595;
  letter-spacing: 0.05em;
}
.sws-shell #sws-frame {
  margin-left: var(--sidebar-width);
  width: calc(100% - var(--sidebar-width));
  border: 0;
  border-radius: 0;
  background: #101010;
}
.sws-shell #sws-load {
  position: fixed;
  right: 28px;
  top: 8px;
  font-size: 11px;
  color: #8e8e8e;
  z-index: 3;
}

html.sws-embedded body {
  background: #101010;
}
html.sws-embedded .b {
  background: transparent;
  padding: 26px 30px;
  gap: 24px;
}
.b1 {
  border-color: #303030;
  background: #191919;
  border-radius: 14px;
}
.b1 > div:first-child {
  font-size: 16px;
  color: #d8d8d8;
}
h2.sws-section-title {
  margin: 0;
  font-size: 17px;
  font-weight: 600;
  color: #c7c7c7;
}
.sws-section-head {
  width: 100%;
  display: flex;
  align-items: baseline;
  justify-content: space-between;
  gap: 12px;
  margin: 0 0 14px;
}
.sws-section-head p {
  font-size: 12px;
  color: #8a8a8a;
  margin: 0;
}

.sws-controller {
  width: 100%;
  display: flex;
  flex-direction: column;
  gap: 26px;
  min-width: 0;
}
.sws-overview-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 350px), 1fr));
  gap: 18px;
}
.sws-overview-grid > .b1 {
  padding: 22px 24px;
  background: linear-gradient(120deg, #1c1c1c, #191919);
  border-color: #3a3a3a;
}
.sws-axis-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 300px), 1fr));
  gap: 18px;
}
.sws-axis-grid > .b1 {
  padding: 22px;
  line-height: 1.9;
}
.sws-axis-grid .content {
  margin-top: 10px;
}
.sws-axis-grid .b1 > hr:first-of-type {
  margin-top: 10px;
}

.sws-more {
  width: 100%;
  border: 1px solid #303030;
  border-radius: 14px;
  padding: 18px 22px;
  background: #151515;
}
.sws-more > summary {
  cursor: pointer;
  color: #bababa;
  font-size: 14px;
}
.sws-more[open] > summary {
  margin-bottom: 18px;
}
.sws-settings-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 310px), 1fr));
  gap: 18px;
  min-width: 0;
}

.sws-mount {
  width: 100%;
  display: flex;
  flex-direction: column;
  gap: 24px;
  min-width: 0;
}
.sws-control-grid {
  display: grid;
  grid-template-columns: minmax(290px, 0.9fr) minmax(340px, 1.4fr);
  grid-template-areas: 'guide target' 'guide tracking';
  gap: 18px;
  align-items: stretch;
}
.sws-guide {
  grid-area: guide;
}
.sws-target {
  grid-area: target;
}
.sws-tracking {
  grid-area: tracking;
}
.sws-guide > .b1,
.sws-target > .b1,
.sws-tracking > .b1 {
  height: 100%;
  width: 100% !important;
  padding: 24px;
}
.sws-guide > .b1 {
  background: radial-gradient(ellipse at center, #202020, #191919 75%);
  text-align: center !important;
}
.sws-guide .gb {
  width: 64px;
  height: 58px;
  border-radius: 12px;
  background: #282828;
  border-color: #494949;
  color: #cfcfcf;
  font-size: 23px;
}
.sws-guide .gb:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-guide .collapsible {
  margin-top: 20px;
}
.sws-guide .sws-pad {
  display: grid;
  grid-template-columns: repeat(3, 64px);
  gap: 8px;
  justify-content: center;
  margin: 32px auto 26px;
}
.sws-pad .gb {
  margin: 0;
}
.sws-pad .gb:first-of-type {
  grid-column: 2;
}
.sws-pad .gb:nth-of-type(2) {
  grid-column: 1;
}
.sws-pad .gb:last-of-type {
  grid-column: 2;
}

.sws-target .b1 {
  line-height: 1.9;
}
.sws-tracking .b1 {
  line-height: 1.9;
}
.sws-account form {
  margin-top: 10px;
}

@media (min-width: 1700px) {
  .sws-control-grid {
    grid-template-columns: minmax(320px, 0.7fr) minmax(440px, 1.3fr);
  }
}

@media (max-width: 900px) {
  .sws-control-grid {
    grid-template-columns: 1fr;
    grid-template-areas: 'guide' 'target' 'tracking';
  }
  .sws-guide > .b1 {
    padding: 20px;
  }
  .sws-axis-grid {
    grid-template-columns: 1fr;
  }
}

@media (max-width: 700px) {
  .sws-shell .t {
    inset: auto 0 0;
    width: 100%;
    height: calc(74px + env(safe-area-inset-bottom));
    padding: 5px 8px calc(5px + env(safe-area-inset-bottom));
    border: 0;
    border-top: 1px solid #303030;
    z-index: 10;
    background: #161616;
    display: block;
  }
  .sws-shell .t table,
  .sws-sidebar-note {
    display: none !important;
  }
  .sws-nav {
    display: flex;
    flex-direction: row;
    gap: 4px;
    overflow-x: auto;
    height: 64px;
  }
  .sws-shell .t a:link,
  .sws-shell .t a:visited {
    flex: 1 0 64px;
    flex-direction: column;
    justify-content: center;
    gap: 3px;
    padding: 5px 8px;
    font-size: 11px;
    border-radius: 10px;
    min-width: 64px;
  }
  .sws-shell .t a[aria-current='page'] {
    box-shadow: none;
  }
  .sws-nav svg {
    width: 21px;
    height: 21px;
  }
  .sws-topbar {
    margin-left: 0;
    padding: 14px 18px;
    gap: 8px;
  }
  .sws-topbar h1 {
    font-size: 20px;
  }
  .sws-topbar p {
    font-size: 11px;
    max-width: 230px;
  }
  .sws-brand-small {
    font-size: 11px;
  }
  .sws-shell #sws-frame {
    margin-left: 0;
    width: 100%;
  }
  .sws-shell #sws-load {
    right: 12px;
    top: 2px;
    font-size: 10px;
  }
  html.sws-embedded .b {
    padding: 18px 14px;
  }
  .sws-overview-grid,
  .sws-settings-grid {
    grid-template-columns: 1fr;
  }
  .sws-section-head {
    align-items: flex-start;
    flex-direction: column;
    gap: 4px;
  }
  .sws-more {
    padding: 16px;
  }
  .sws-control-grid {
    gap: 14px;
  }
  .sws-guide .sws-pad {
    margin: 22px auto;
  }
  .sws-target > .b1,
  .sws-tracking > .b1 {
    padding: 18px;
  }
  .sws-overview-grid > .b1,
  .sws-axis-grid > .b1 {
    padding: 18px;
  }
}

/* 轴参数使用原有描述，标签与输入框对齐且允许长名称换行 */
.sws-parameter {
  display: grid;
  grid-template-columns: minmax(0, 1fr) minmax(90px, 1fr);
  gap: 12px;
  align-items: center;
  margin: 10px 0;
  text-align: left;
}
.sws-parameter > span {
  order: -1;
  overflow-wrap: anywhere;
}
.sws-parameter > input,
.sws-parameter > select {
  width: 100% !important;
  min-width: 0;
  margin: 0 !important;
}
.sws-document pre {
  white-space: pre-wrap;
  overflow-wrap: anywhere;
}

/* 其余设备页采用主操作与次要设置分区，帮助与错误页保持同一视觉语言 */
.sws-device-layout {
  display: flex;
  flex-direction: column;
  gap: 24px;
  width: 100%;
  min-width: 0;
}
.sws-device-controls {
  display: grid;
  grid-template-columns: minmax(300px, 1.3fr) minmax(280px, 1fr);
  gap: 18px;
}
.sws-device-controls > section {
  min-width: 0;
}
.sws-device-controls > section > .b1 {
  height: 100%;
  padding: 24px;
}
.sws-device-primary .b1 {
  background: radial-gradient(ellipse at top, #202020, #191919 80%);
}
.sws-device-selector > .b1 {
  background: #1c1c1c;
  padding: 22px;
}
.sws-device-layout .gb {
  min-width: 64px;
  min-height: 58px;
}
.sws-device-primary button {
  min-width: 44px;
  min-height: 44px;
}
.sws-device-layout .content {
  padding: 14px;
}

.sws-aux-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  align-items: start;
}
.sws-aux-grid > .sws-aux-card {
  display: block;
  text-align: left;
  padding: 20px;
  overflow-wrap: anywhere;
}
.sws-aux-head {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  padding-bottom: 14px;
  margin-bottom: 6px;
  border-bottom: 1px solid #2c2c2c;
}
.sws-aux-title {
  min-width: 0;
}
.sws-aux-title > span {
  display: block;
  color: var(--muted);
  font-size: 12px;
  font-weight: 400;
  line-height: 1.6;
}
.sws-aux-title h2 {
  font-size: 16px;
  font-weight: 600;
  line-height: 1.6;
  margin: 2px 0 0;
  color: #d8d8d8;
}
.sws-aux-power {
  display: flex;
  flex-wrap: wrap;
  justify-content: flex-end;
  gap: 6px;
  max-width: 45%;
  font-size: 12px;
  font-weight: 500;
  line-height: 1.5;
  font-variant-numeric: tabular-nums;
}
.sws-aux-power:empty {
  display: none;
}
.sws-aux-power > span {
  white-space: nowrap;
  padding: 4px 8px;
  border-radius: 6px;
  background: var(--field);
}
.sws-aux-row {
  display: grid;
  grid-template-columns: minmax(110px, 1fr) minmax(0, 1.7fr) 66px;
  align-items: center;
  gap: 12px;
  min-height: 54px;
  padding: 8px 0;
}
.sws-aux-row + .sws-aux-row {
  border-top: 1px solid #252525;
}
.sws-aux-label {
  font-size: 13px;
  line-height: 1.7;
  color: var(--muted);
  font-weight: 400;
}
.sws-aux-controls {
  display: flex;
  align-items: center;
  gap: 6px;
  min-width: 0;
}
.sws-aux-value {
  font-size: 13px;
  line-height: 1.6;
  text-align: right;
  color: var(--readout);
  font-variant-numeric: tabular-nums;
  white-space: nowrap;
}
.sws-aux-controls button {
  flex: 1;
  min-width: 0;
  min-height: 38px;
  margin: 0;
  padding: 7px 10px;
  font-size: 13px;
  border-radius: 7px;
}
.sws-aux-controls input[type='range'] {
  width: 100%;
  max-width: 100%;
  min-width: 0;
  margin: 0;
  padding: 0;
  height: 38px;
  accent-color: #962525;
  background: transparent;
  border: 0;
  cursor: pointer;
}
@media (max-width: 800px) {
  .sws-aux-grid {
    grid-template-columns: 1fr;
  }
}
@media (max-width: 500px) {
  .sws-aux-grid > .sws-aux-card {
    padding: 16px;
  }
  .sws-aux-head {
    gap: 10px;
    padding-bottom: 12px;
  }
  .sws-aux-power {
    gap: 4px;
    max-width: 46%;
  }
  .sws-aux-power > span {
    padding: 3px 6px;
  }
  .sws-aux-row {
    grid-template-columns: 90px minmax(0, 1fr) 54px;
    gap: 8px;
  }
  .sws-aux-label {
    font-size: 12px;
  }
  .sws-aux-controls button {
    font-size: 12px;
    padding: 6px 4px;
  }
  .sws-aux-value {
    font-size: 12px;
  }
}

.sws-document {
  max-width: 960px;
  margin: 30px auto;
  padding: 32px;
  background: #191919;
  border: 1px solid #373737;
  border-radius: 18px;
}
.sws-document header {
  border-bottom: 1px solid #303030;
  padding-bottom: 22px;
  margin-bottom: 26px;
}
.sws-document h1 {
  text-align: left;
  font-size: 28px;
  color: #d1d1d1;
}
.sws-document h2 {
  font-size: 18px;
  color: #c4c4c4;
  margin-top: 28px;
}
.sws-document p {
  color: #9e9e9e;
  line-height: 1.8;
}
.sws-document pre {
  padding: 20px;
  background: #0f0f0f;
  border: 1px solid #303030;
  border-radius: 12px;
  overflow: auto;
  color: #c4c4c4;
}
.sws-document table {
  width: 100%;
  border-collapse: collapse;
}
.sws-document td,
.sws-document th {
  text-align: left;
  padding: 10px;
  border-bottom: 1px solid #303030;
}
.sws-document a {
  display: inline-block;
  padding: 10px 16px;
  border: 1px solid #4c4c4c;
  border-radius: 9px;
  background: #2f2f2f;
  text-decoration: none;
}
.sws-empty {
  max-width: 620px;
  margin: 12vh auto;
  text-align: center;
}
.sws-empty h1 {
  text-align: center;
}
.sws-notice {
  width: 100%;
  padding: 24px;
  background: #1d1d1d;
  border: 1px solid #494949;
  border-radius: 14px;
}
.sws-notice h2 {
  font-size: 20px;
}
.sws-notice li {
  margin: 12px 0;
}

@media (max-width: 800px) {
  .sws-device-controls {
    grid-template-columns: 1fr;
  }
  .sws-device-controls > section > .b1 {
    padding: 18px;
  }
  .sws-aux-grid {
    grid-template-columns: 1fr;
  }
}

@media (max-width: 700px) {
  .sws-document {
    margin: 10px 0;
    padding: 18px;
  }
  .sws-document h1 {
    font-size: 23px;
  }
  .sws-document pre {
    font-size: 12px;
  }
  .sws-notice {
    padding: 18px;
  }
}

.sws-extra-nav {
  display: contents;
}
.sws-nav-more {
  display: none;
}

@media (max-width: 700px) {
  .sws-nav {
    overflow: visible;
  }
  .sws-nav-more {
    display: flex;
    flex: 1 0 64px;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    gap: 3px;
    padding: 5px 8px;
    margin: 0;
    border: 0;
    border-radius: 10px;
    background: transparent;
    color: #9f9f9f;
    font-size: 11px;
  }
  .sws-nav-more[data-active='true'] {
    background: var(--nav-active);
    color: #fff;
    box-shadow: none;
  }
  .sws-nav-more svg {
    fill: currentColor;
    stroke: none;
  }
  .sws-extra-nav {
    display: none;
    position: fixed;
    bottom: calc(84px + env(safe-area-inset-bottom));
    left: 12px;
    right: 12px;
    padding: 10px;
    background: #1f1f1f;
    border: 1px solid #424242;
    border-radius: 14px;
    box-shadow: 0 -12px 36px #0008;
  }
  .sws-nav[data-expanded='true'] .sws-extra-nav {
    display: grid;
    grid-template-columns: repeat(2, minmax(0, 1fr));
    gap: 8px;
  }
  .sws-shell .t .sws-extra-nav a {
    min-height: 64px;
    flex-direction: row;
    justify-content: flex-start;
    font-size: 13px;
    gap: 10px;
  }
}

/* 状态指标合并展示，常用控制采用左右分区并随窄屏改为单列 */
.sws-overview-grid {
  display: block;
}
.sws-metrics {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(155px, 1fr));
  gap: 12px;
}
.sws-metric {
  display: flex;
  flex-direction: column;
  gap: 8px;
  background: #1d1d1d;
  border-radius: 10px;
  padding: 16px;
}
.sws-metric > span {
  font-size: 12px;
  color: var(--muted);
}
.sws-metric > strong {
  font-size: 20px;
  font-weight: 600;
}
.sws-overview-grid > .b1 {
  padding: 20px;
}
.sws-overview-grid .content {
  margin-top: 12px;
}
.sws-overview-grid .content ul {
  font-size: 13px;
  color: var(--muted);
}

.sws-control-grid {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  grid-template-areas: 'tracking guide' 'target target';
  align-items: start;
}
.sws-guide > .b1,
.sws-tracking > .b1 {
  min-height: 390px !important;
  height: auto;
}
.sws-target > .b1 {
  height: auto;
}
.sws-home > .b1 {
  width: 100% !important;
  padding: 22px;
}
.sws-home button {
  min-width: 120px;
}
.sws-home > .b1 > .collapsible {
  margin-top: 12px;
}
.sws-guide > .b1 {
  background: var(--panel);
}

.sws-choice {
  display: flex;
  flex-direction: column;
  gap: 8px;
  margin: 18px 0;
  text-align: left;
}
.sws-choice > span {
  font-size: 14px;
  font-weight: 600;
  color: var(--text);
}
.sws-choice select {
  margin: 0;
  width: 100%;
  min-height: 46px;
  padding: 10px 12px;
}
.sws-choice-originals {
  display: none !important;
}
.sws-tracking .content {
  margin-top: 12px;
}
.sws-track-switch {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin: 8px 0 20px;
}
.sws-switch {
  width: 52px;
  min-height: 28px;
  height: 28px;
  padding: 3px;
  border-radius: 18px;
  background: #333333;
  border: 0;
  position: relative;
}
.sws-switch::after {
  content: '';
  display: block;
  width: 22px;
  height: 22px;
  border-radius: 50%;
  background: #acacac;
  transition: transform 0.15s;
}
.sws-switch[aria-checked='true'] {
  background: var(--accent);
}
.sws-switch[aria-checked='true']::after {
  transform: translateX(24px);
  background: #d4d4d4;
}

/* 四个方向保持原来的按住移动语义，中间停止与定位区同步分别保留 */
.sws-guide .sws-pad {
  position: relative;
  display: block;
  width: 272px;
  height: 272px;
  margin: 20px auto;
  border-radius: 50%;
  overflow: hidden;
  background: #0f0f0f;
}
.sws-guide .sws-pad .gb {
  position: absolute;
  inset: 0;
  width: 100%;
  height: 100%;
  border: 0;
  border-radius: 0;
  margin: 0;
  background: #2a2a2a;
  font-size: 20px;
  padding: 0;
  display: block;
  min-width: 0;
}
.sws-pad .gb:nth-of-type(1) {
  clip-path: polygon(0 0, 100% 0, 50% 50%);
  padding-bottom: 190px !important;
}
.sws-pad .gb:nth-of-type(2) {
  clip-path: polygon(0 0, 50% 50%, 0 100%);
  padding-right: 190px !important;
}
.sws-pad .gb:nth-of-type(4) {
  clip-path: polygon(100% 0, 100% 100%, 50% 50%);
  padding-left: 190px !important;
}
.sws-pad .gb:nth-of-type(5) {
  clip-path: polygon(0 100%, 50% 50%, 100% 100%);
  padding-top: 190px !important;
}
.sws-guide .sws-pad .gb:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-guide .sws-pad .sws-pad-stop {
  inset: calc(50% - 49px);
  width: 98px;
  height: 98px;
  border: 10px solid #191919;
  border-radius: 50%;
  z-index: 2;
  background: var(--accent);
  font-size: 17px;
}
.sws-sync {
  min-width: 90px;
}
.sws-guide > .b1 > .collapsible,
.sws-guide > .b1 > .content {
  display: none;
}
.sws-guide .collapsible,
.sws-guide .content {
  display: none !important;
}
.sws-home-actions {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 18px;
}
.sws-home-actions > div {
  display: flex;
  gap: 8px;
  padding: 14px;
  background: #1d1d1d;
  border-radius: 12px;
}
.sws-home-actions button {
  flex: 1;
  min-width: 0;
  margin: 0;
}
.sws-actions > br {
  display: none;
}
.sws-actions {
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
  margin: 18px 0;
}
.sws-actions > button {
  margin: 0;
}
.sws-guide .sws-pad::after {
  content: '';
  position: absolute;
  inset: 0;
  pointer-events: none;
  background:
    linear-gradient(45deg, transparent 49%, #191919 49%, #191919 51%, transparent 51%),
    linear-gradient(-45deg, transparent 49%, #191919 49%, #191919 51%, transparent 51%);
}
.sws-pad-stop {
  z-index: 3 !important;
}

.sws-mount .sws-settings-grid {
  align-items: start;
}
.sws-mount .b1 .content {
  line-height: 1.8;
}
.sws-mount .b1 .content button {
  margin-top: 5px;
  margin-bottom: 5px;
}
.sws-mount .b1 .content br + br {
  display: none;
}

@media (max-width: 900px) {
  .sws-control-grid {
    grid-template-columns: 1fr;
    grid-template-areas: 'guide' 'tracking' 'target';
  }
  .sws-guide > .b1,
  .sws-tracking > .b1 {
    min-height: 0 !important;
  }
}

@media (max-width: 700px) {
  .sws-home-actions {
    grid-template-columns: 1fr;
  }
  .sws-metric {
    min-width: 0;
    overflow-wrap: anywhere;
  }
}

@media (max-width: 400px) {
  .sws-metrics {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }
  .sws-metric {
    padding: 12px;
  }
  .sws-metric > strong {
    font-size: 17px;
  }
  .sws-guide .sws-pad {
    width: 240px;
    height: 240px;
  }
  .sws-pad .gb:nth-of-type(1) {
    padding-bottom: 165px !important;
  }
  .sws-pad .gb:nth-of-type(2) {
    padding-right: 165px !important;
  }
  .sws-pad .gb:nth-of-type(4) {
    padding-left: 165px !important;
  }
  .sws-pad .gb:nth-of-type(5) {
    padding-top: 165px !important;
  }
}

/* 按任务分组设置卡片，桌面最多两列，手机保持单列与足够的触控间距 */
.sws-card-marker {
  display: none;
}
.sws-mount .sws-settings-grid {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 22px;
}
.sws-mount .sws-more {
  padding: 0;
  border: 0;
  background: transparent;
}
.sws-mount .sws-more > summary {
  padding: 8px 0;
}
.sws-mount .b1 {
  text-align: left !important;
  padding: 24px;
  overflow-wrap: anywhere;
}
.sws-mount .sws-guide > .b1 {
  text-align: center !important;
}
.sws-mount .content {
  padding: 18px;
  margin-top: 8px;
  background: #1b1b1b;
}
.sws-mount .content > br:first-child,
.sws-mount .content > br:last-child {
  display: none;
}
.sws-mount .collapsible {
  margin: 0;
  background: #202020;
  border-color: #3a3a3a;
  min-height: 48px;
}
.sws-mount .collapsible::after {
  content: '+';
  float: right;
}
.sws-mount .collapsible.active::after {
  content: '−';
}
.sws-mount .b1 > hr {
  margin: 18px 0;
}
.sws-mount button {
  min-height: 44px;
}
.sws-mount button:not(.gb) {
  font-size: 14px;
}
.sws-mount .content p {
  margin: 10px 0;
  font-size: 13px;
  line-height: 1.8;
  color: var(--muted);
}

.sws-field-group {
  border: 0;
  padding: 0;
  margin: 0 0 22px;
  min-width: 0;
}
.sws-field-group legend {
  padding: 0;
  margin-bottom: 10px;
  color: #c1c1c1;
  font-size: 13px;
  line-height: 1.7;
}
.sws-unit-inputs {
  display: flex;
  align-items: center;
  gap: 4px;
}
.sws-unit-inputs input {
  flex: 1;
  width: 0 !important;
  min-width: 0;
  margin: 0;
  padding: 10px 6px;
  text-align: center;
}
.sws-unit-inputs select {
  min-width: 76px;
  margin: 0;
}
.sws-help {
  font-size: 12px;
  color: var(--muted);
}
.sws-number-field {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 112px;
  gap: 14px;
  align-items: center;
  margin: 14px 0;
  font-size: 13px;
}
.sws-number-field input {
  min-width: 0;
  width: 100% !important;
  margin: 0;
  padding: 10px;
}
.sws-clock-action {
  margin: 18px 0;
}
.sws-clock-action button {
  margin: 0;
}
.sws-browser-time {
  font-size: 12px;
  color: var(--muted);
  padding: 12px 14px;
  border: 1px solid #2f2f2f;
  border-radius: 8px;
  margin-bottom: 14px;
}
.sws-site #date_ut,
.sws-site #time_ut {
  font-size: 18px;
  font-variant-numeric: tabular-nums;
}
.sws-site #site_lat,
.sws-site #site_long {
  font-variant-numeric: tabular-nums;
}
.sws-site .sws-help {
  margin-bottom: 0;
}

.sws-catalog-select {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 14px;
}
.sws-catalog-select select {
  width: 60%;
  margin: 0;
}
.sws-library #lib_message,
.sws-library #cat_message {
  width: 100% !important;
  margin: 14px 0 !important;
  padding: 12px 14px !important;
  border: 1px solid #2f2f2f !important;
  border-radius: 8px;
  background: #1d1d1d !important;
  font-size: 13px;
}
.sws-library #lib_free {
  font-size: 12px;
  font-weight: 400;
  float: none !important;
  text-align: left !important;
  margin: 6px 0;
  color: var(--muted);
}
.sws-editor-label {
  display: block;
  font-size: 13px;
  margin-bottom: 10px;
}
.sws-library textarea {
  display: block;
  width: 100%;
  min-height: 200px;
  margin: 0;
  padding: 14px;
  resize: vertical;
  background: #131313 !important;
  color: var(--text) !important;
  border-color: #4c4c4c !important;
  line-height: 1.65;
}
.sws-library a {
  display: inline-block;
  padding: 8px 12px;
  border: 1px solid #3a3a3a;
  border-radius: 8px;
  text-decoration: none;
  font-size: 13px;
}
.sws-library .content > br {
  display: none;
}
.sws-library #lib_clear {
  background: transparent;
  color: #a4a4a4;
  border-color: #4a4a4a;
}
.sws-library .sws-actions {
  margin-bottom: 10px;
}

.sws-option-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 14px;
}
.sws-option-row {
  min-width: 0;
  padding: 16px;
  border: 1px solid #313131;
  border-radius: 10px;
  font-size: 13px;
  background: #1c1c1c;
  line-height: 1.8;
}
.sws-option-row > br:last-child {
  display: none;
}
.sws-option-row button {
  margin: 8px 4px 0 0 !important;
  min-width: 48px;
}
.sws-target .sws-option-row {
  min-height: 104px;
}
.sws-homepark .sws-option-row button {
  min-width: 72px;
}
.sws-pec .content {
  font-size: 13px;
}
.sws-align .content {
  font-size: 13px;
}
.sws-align #align_lr,
.sws-align #align_ud {
  font-size: 20px;
}
.sws-advanced-grid .b1 {
  height: auto;
}
.sws-mount .sws-actions {
  gap: 10px;
}
.sws-target .sws-actions button {
  height: 44px !important;
  min-width: 90px;
}
.sws-target [style='display: table; text-align: left;'] {
  width: 100%;
  font-size: 14px;
  font-variant-numeric: tabular-nums;
}

@media (max-width: 1100px) {
  .sws-mount .sws-settings-grid {
    grid-template-columns: 1fr;
  }
}

@media (max-width: 700px) {
  .sws-mount .b1 {
    padding: 18px;
  }
  .sws-mount .content {
    padding: 14px;
  }
  .sws-option-grid {
    grid-template-columns: 1fr;
  }
  .sws-number-field {
    grid-template-columns: minmax(0, 1fr) 90px;
    gap: 10px;
  }
  .sws-target [style='display: table; text-align: left;'] {
    font-size: 12px;
  }
  .sws-target .sws-actions button {
    flex: 1 1 40%;
    min-width: 0;
  }
  .sws-home-actions > div {
    padding: 10px;
  }
  .sws-mount .content button {
    min-height: 48px;
  }
  .sws-library .sws-actions button {
    flex: 1 1 auto;
  }
  .sws-target > .b1 {
    overflow-x: hidden;
  }
}

/* 时间与位置独立排列，长说明与操作保持清楚的间距 */
.sws-location-values {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 12px;
  margin: 16px 0;
}
.sws-location-values > div {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 12px;
  background: #1d1d1d;
  border-radius: 8px;
  font-size: 12px;
  color: var(--muted);
}
.sws-location-values strong {
  font-size: 15px;
  font-weight: 500;
}
.sws-device-time {
  margin-bottom: 6px;
}
.sws-sidereal-time {
  font-size: 13px;
  color: var(--muted);
}
.sws-homepark form {
  max-width: 640px;
}
.sws-homepark .sws-number-field {
  grid-template-columns: 1fr 150px;
}
.sws-pec-write {
  margin-top: 18px;
  padding-top: 16px;
  border-top: 1px solid #303030;
}
.sws-library #lib_free {
  display: block;
  clear: both;
  padding-top: 8px;
}
.sws-mount .b1 > .sws-card-marker {
  display: none;
}
.sws-mount .b1 > .sws-card-marker + br {
  display: none;
}

@media (max-width: 400px) {
  .sws-location-values {
    grid-template-columns: 1fr;
  }
  .sws-homepark .sws-number-field {
    grid-template-columns: 1fr 100px;
  }
}

.sws-option-grid > br {
  display: none;
}
.sws-limit-values {
  display: grid;
  grid-template-columns: minmax(0, 1fr);
  gap: 10px;
  clear: both;
  min-width: 0;
}
.sws-limit-values > div > span {
  min-width: 0;
}
.sws-mount .b1 > hr {
  clear: both;
}
.sws-limit-values > div {
  display: flex;
  justify-content: space-between;
  gap: 16px;
  padding: 10px 12px;
  background: #1d1d1d;
  border-radius: 8px;
  font-size: 12px;
}
.sws-limit-values strong {
  font-size: 16px;
}
.sws-site .sws-field-group:last-of-type {
  margin-bottom: 8px;
}

/* 同行卡片自动等高，紧凑显示状态与表单，手机恢复自然高度 */
.sws-mount {
  gap: 18px;
}
.sws-control-grid {
  align-items: stretch;
  gap: 18px;
}
.sws-guide,
.sws-tracking {
  display: flex;
  min-width: 0;
}
.sws-guide > .b1,
.sws-tracking > .b1 {
  height: 100%;
  flex: 1;
  min-height: 0 !important;
}
.sws-guide > .b1 {
  display: flex;
  flex-direction: column;
}
.sws-guide .sws-pad {
  margin: auto;
  flex-shrink: 0;
}
.sws-guide > .b1 > hr {
  width: 100%;
}

/* 方位控制与跟踪标题下方仅保留间距 */
.sws-guide > .b1 > hr:first-of-type,
.sws-tracking > .b1 > hr:first-of-type {
  border: 0;
  height: 0;
}
.sws-choice {
  margin: 12px 0;
  gap: 6px;
}
.sws-choice select {
  min-height: 42px;
  padding: 8px 12px;
}
.sws-track-switch {
  margin: 4px 0 12px;
}
.sws-mount button.sws-switch {
  width: 44px !important;
  min-width: 44px;
  min-height: 26px !important;
  height: 26px !important;
  padding: 0;
  border: 1px solid #4c4c4c;
  border-radius: 18px;
  background: #494949;
  flex-shrink: 0;
}
.sws-mount button.sws-switch::after {
  position: absolute;
  top: 3px;
  left: 3px;
  width: 18px;
  height: 18px;
  background: #fff;
}
.sws-mount button.sws-switch[aria-checked='true'] {
  background: #8b2020;
  border-color: #8b2020;
}
.sws-mount button.sws-switch[aria-checked='true']::after {
  transform: translateX(18px);
  background: #fff;
}

.sws-mount .b1 {
  padding: 18px;
}
.sws-mount .b1 > hr {
  margin: 12px 0;
}
.sws-mount .content {
  padding: 12px;
  margin-top: 6px;
}
.sws-mount .collapsible {
  min-height: 40px;
  padding: 8px 12px;
}
.sws-mount .sws-settings-grid {
  gap: 18px;
  align-items: stretch;
}
.sws-site-library > .b1 {
  height: 100%;
}
.sws-location-values {
  margin: 10px 0;
  gap: 8px;
}
.sws-location-values > div {
  padding: 10px;
}
.sws-clock-action {
  margin: 12px 0;
}
.sws-library #lib_free {
  padding-top: 2px;
  margin: 0;
}
.sws-library #lib_message,
.sws-library #cat_message {
  margin: 10px 0 !important;
  padding: 9px 12px !important;
}
.sws-limit-values {
  gap: 4px;
}
.sws-limit-values > div {
  padding: 6px 10px;
  background: transparent;
  border-bottom: 1px solid #292929;
  border-radius: 0;
}
.sws-option-grid {
  gap: 10px;
}
.sws-option-row {
  padding: 12px;
  min-height: 0 !important;
}
.sws-home-actions {
  gap: 18px;
}
.sws-home-actions > div {
  display: block;
  padding: 16px;
  background: #1d1d1d;
}
.sws-home-actions h3,
.sws-coordinate-form h3 {
  font-size: 15px;
  font-weight: 600;
  margin: 0 0 12px;
}
.sws-action-pair {
  display: flex;
  gap: 10px;
}
.sws-action-pair > br {
  display: none;
}
.sws-action-pair button {
  flex: 1;
  min-width: 0;
}
.sws-secondary {
  background: #2b2b2b !important;
  border-color: #383838 !important;
}
.sws-secondary:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-unpark-row {
  margin-top: 8px;
  text-align: right;
}
.sws-unpark-row button {
  min-height: 32px !important;
  min-width: 0;
  padding: 4px 10px;
  background: transparent;
  color: var(--muted);
}

.sws-pier-status {
  clear: both;
  text-align: right;
  font-size: 12px;
  color: var(--muted);
  padding: 4px 0;
}
/* 卡片状态统一为柔和红色标签，空状态不显示底色 */
:is(#gto_status, #hp_sta, #align_progress, #guide_sta, #track, #foc_sta, #rot_sta) {
  display: inline-block;
  max-width: 100%;
  box-sizing: border-box;
  padding: 2px 10px;
  border-radius: 6px;
  background: #272727;
  color: var(--readout);
  font-size: 12px;
  font-weight: 600;
  line-height: 1.6;
  overflow-wrap: anywhere;
}

:is(#gto_status, #hp_sta, #align_progress, #guide_sta, #track, #foc_sta, #rot_sta):empty {
  display: none;
}

.sws-position-grid {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 8px;
  margin: 12px 0;
}
.sws-position-grid > div {
  container-type: inline-size;
  min-height: 86px;
  padding: 8px 12px;
  background: #1d1d1d;
  border-radius: 8px;
  display: flex;
  flex-direction: column;
  gap: 4px;
}
.sws-position-grid span {
  font-size: 12px;
  color: var(--muted);
}
.sws-position-grid strong {
  font-size: clamp(15px, 12cqi, 24px);
  line-height: 1.3;
  font-weight: 600;
  color: var(--readout);
  font-variant-numeric: tabular-nums;
}
.sws-coordinate-form {
  padding: 16px;
  background: #1d1d1d;
  border-radius: 10px;
  margin: 14px 0;
}
.sws-coordinate-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 14px 18px;
  align-items: end;
}
.sws-coordinate-system {
  display: flex;
  flex-direction: column;
  gap: 8px;
}
.sws-coordinate-grid input {
  margin: 0;
  min-width: 0;
  width: 100% !important;
}
.sws-coordinate-grid fieldset {
  margin: 0;
  padding: 0;
  border: 0;
  min-width: 0;
}
.sws-coordinate-grid legend {
  font-size: 14px;
  font-weight: 600;
  padding: 0;
  margin-bottom: 8px;
}
.sws-coordinate-actions {
  display: flex;
  gap: 10px;
}
.sws-coordinate-actions button {
  flex: 1;
  margin: 0;
  min-height: 44px;
}
.sws-coordinate-units {
  display: flex;
  gap: 6px;
  align-items: center;
  min-width: 0;
}
.sws-coordinate-unit {
  flex: 1;
  min-width: 0;
  position: relative;
}
.sws-coordinate-unit input {
  padding: 10px 28px 10px 8px;
  text-align: center;
  appearance: textfield;
  -moz-appearance: textfield;
}
.sws-coordinate-unit input::-webkit-inner-spin-button {
  appearance: none;
}
.sws-coordinate-unit span {
  position: absolute;
  right: 9px;
  top: 50%;
  transform: translateY(-50%);
  pointer-events: none;
  color: var(--muted);
  font-size: 12px;
}
.sws-coordinate-units select {
  width: 56px;
  min-width: 56px;
  padding: 10px 5px;
  margin: 0;
}
.sws-coordinate-message {
  font-size: 13px;
  margin: 10px 0 0 !important;
  color: #b1b1b1;
}
.sws-coordinate-message:empty {
  display: none;
}
.sws-target .sws-actions {
  margin: 12px 0;
}

@media (max-width: 900px) {
  .sws-guide,
  .sws-tracking {
    display: block;
  }
  .sws-guide > .b1,
  .sws-tracking > .b1 {
    height: auto;
  }
  .sws-guide .sws-pad {
    margin: 14px auto;
  }
  .sws-site-library > .b1 {
    height: auto;
  }
}

@media (max-width: 700px) {
  .sws-coordinate-grid {
    grid-template-columns: 1fr;
  }
  .sws-coordinate-actions {
    grid-row: 4;
  }
  .sws-coordinate-form {
    padding: 12px;
  }
  .sws-position-grid {
    grid-template-columns: repeat(2, minmax(0, 1fr));
  }
  .sws-position-grid > div:nth-child(2) {
    order: 2;
  }
  .sws-position-grid > div:nth-child(3) {
    order: 4;
  }
  .sws-position-grid > div:nth-child(4) {
    order: 1;
  }
  .sws-position-grid > div:nth-child(5) {
    order: 3;
  }
  .sws-position-grid > div:nth-child(6) {
    order: 5;
  }
  .sws-home-actions > div {
    padding: 12px;
  }
  .sws-mount .b1 {
    padding: 16px;
  }
  .sws-coordinate-unit input {
    padding-left: 4px;
    padding-right: 22px;
  }
  .sws-coordinate-unit span {
    right: 5px;
  }
  .sws-coordinate-units select {
    width: 44px;
    min-width: 44px;
  }
  .sws-mount button.sws-switch {
    min-height: 26px !important;
  }
}

/* 跟踪高级设置按功能分组，按钮保持等宽并适应窄屏 */
.sws-tracking-setting {
  border: 0;
  border-bottom: 1px solid #303030;
  padding: 0 0 14px;
  margin: 0 0 14px;
  min-width: 0;
}
.sws-tracking-setting:last-child {
  border-bottom: 0;
  padding-bottom: 0;
  margin-bottom: 0;
}
.sws-tracking-setting legend {
  float: none;
  width: 100%;
  padding: 0;
  margin: 0 0 8px;
  color: var(--muted);
  font-size: 13px;
  line-height: 1.6;
}
.sws-tracking-options {
  display: flex;
  gap: 8px;
}
.sws-mount .content .sws-tracking-options button {
  flex: 1;
  min-width: 0;
  margin: 0;
  padding: 9px 8px;
  border-radius: 8px;
  line-height: 1.5;
  white-space: normal;
}
.sws-frequency-options button {
  background: #292929;
  border-color: #3c3c3c;
}
.sws-frequency-options button:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
@media (max-width: 420px) {
  .sws-frequency-options {
    flex-wrap: wrap;
  }
  .sws-frequency-options button {
    flex-basis: calc(50% - 8px) !important;
  }
  .sws-frequency-options button:last-child {
    flex-basis: 100% !important;
  }
}

/* 时间概览使用统一的信息格，地点输入按实际内容限制宽度 */
.sws-time-summary {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
  margin: 12px 0 8px;
}
.sws-time-summary > div {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 12px;
  background: #1d1d1d;
  border: 0;
  border-radius: 8px;
  margin: 0;
  min-width: 0;
}
.sws-time-summary .sws-browser-time {
  grid-column: 1/-1;
  flex-direction: row;
  align-items: center;
  justify-content: space-between;
  flex-wrap: wrap;
  background: transparent;
  padding: 0 0 8px;
  border-bottom: 1px solid #2f2f2f;
  border-radius: 0;
}
.sws-time-label {
  font-size: 12px;
  color: var(--muted);
  font-weight: 400;
}
.sws-time-summary strong,
.sws-site #date_ut,
.sws-site #time_ut {
  font-size: 15px;
  font-weight: 500;
  line-height: 1.6;
  font-variant-numeric: tabular-nums;
}
.sws-time-summary #datetime {
  font-size: 13px;
  color: var(--text);
}
.sws-site .sws-location-values {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  margin: 8px 0;
}
.sws-site .sws-clock-action {
  display: flex;
  justify-content: flex-end;
  margin: 12px 0;
}
.sws-site .sws-clock-action button {
  font-weight: 500;
  font-size: 13px;
}
.sws-site-fields {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 250px), 1fr));
  gap: 16px;
}
.sws-site-fields .sws-field-group {
  margin: 0 !important;
}
.sws-site-fields .sws-field-group:last-child {
  grid-column: 1/-1;
}
.sws-site-fields legend {
  font-size: 12px;
  margin-bottom: 8px;
}
.sws-site-fields .sws-unit-inputs {
  max-width: 360px;
  gap: 4px;
}
.sws-site-fields .sws-unit-inputs input {
  height: 42px;
  padding: 8px 4px;
  font-size: 14px;
}
.sws-site-fields .sws-field-group:last-child .sws-unit-inputs {
  max-width: 220px;
}
.sws-site-fields .sws-unit-inputs select {
  height: 42px;
  min-width: 68px;
  padding: 8px;
  font-size: 14px;
}
.sws-site .sws-site-fields .sws-help {
  margin: 8px 0 0;
  font-size: 12px;
  line-height: 1.6;
}
.sws-site-submit {
  display: flex;
  justify-content: flex-end;
  margin-top: 16px;
  padding-top: 12px;
  border-top: 1px solid #2f2f2f;
}
.sws-site-submit button {
  margin: 0 !important;
  min-width: 100px;
}
@media (max-width: 420px) {
  .sws-time-summary,
  .sws-site .sws-location-values {
    grid-template-columns: 1fr;
  }
  .sws-site-fields {
    gap: 14px;
  }
  .sws-site .sws-clock-action button {
    width: 100%;
  }
  .sws-site .sws-clock-action small {
    width: 100%;
  }
  .sws-site-submit button {
    width: 100%;
  }
}

/* 时间与位置的信息框等宽等高，数值统一居中 */
.sws-time-summary > .sws-device-time,
.sws-time-summary > .sws-sidereal-time,
.sws-site .sws-location-values > div {
  box-sizing: border-box;
  min-height: 88px;
  padding: 12px;
  gap: 6px;
  display: flex;
  flex-direction: column;
  align-items: stretch;
  justify-content: flex-start;
  line-height: 1.6;
}
.sws-time-summary .sws-device-time > div,
.sws-time-summary #time_lst,
.sws-site .sws-location-values strong {
  display: flex;
  flex: 1;
  align-items: center;
  justify-content: center;
  text-align: center;
  width: 100%;
  gap: 5px;
  flex-wrap: wrap;
  font-size: 15px;
  line-height: 1.6;
}

/* 定位概览采用左侧读数与右侧操作的双栏布局 */
.sws-goto-overview {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 260px;
  gap: 18px;
  align-items: stretch;
  margin: 12px 0;
}
.sws-goto-overview .sws-position-grid {
  margin: 0;
  min-width: 0;
}
.sws-goto-overview .sws-position-grid strong {
  text-align: center;
  margin: auto 0;
}
.sws-target .sws-goto-overview .sws-actions {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  grid-template-rows: repeat(2, minmax(44px, 1fr));
  gap: 8px;
  margin: 0;
  align-items: stretch;
}
.sws-target .sws-goto-overview .sws-actions button {
  width: 100% !important;
  height: 100% !important;
  min-width: 0;
  margin: 0;
  padding: 10px 8px;
  white-space: normal;
  line-height: 1.5;
}
.sws-target .sws-goto-overview .sws-actions button:nth-of-type(n + 3) {
  background: #292929;
  border-color: #3c3c3c;
}
@media (max-width: 1000px) {
  .sws-goto-overview {
    grid-template-columns: 1fr;
  }
  .sws-target .sws-goto-overview .sws-actions {
    grid-template-columns: repeat(4, minmax(0, 1fr));
    grid-template-rows: 1fr;
  }
}
@media (max-width: 700px) {
  .sws-goto-overview {
    gap: 12px;
  }
  .sws-target .sws-goto-overview .sws-actions {
    grid-template-columns: repeat(2, minmax(0, 1fr));
    grid-template-rows: repeat(2, minmax(44px, 1fr));
  }
}

/* 方位控制标题与其他卡片一致靠左，方向盘继续居中 */
.sws-mount .sws-guide > .b1 > div:first-child {
  text-align: left;
  align-self: stretch;
}

/* 跟踪卡片从开关开始显示，移除重复标题及标题占位 */
.sws-tracking > .b1 > div:first-child,
.sws-tracking > .b1 > br:first-of-type,
.sws-tracking > .b1 > hr,
.sws-guide #guide_rate,
.sws-guide #guide_sta,
.sws-tracking #track {
  display: none;
}

/* 设备跟踪开关行与方位控制标题采用统一层级 */
.sws-track-switch {
  min-height: 28px;
  margin: 0 0 16px;
  clear: both;
}
.sws-track-switch > span,
.sws-mount .sws-guide > .b1 > div:first-child {
  font-size: 16px;
  font-weight: 600;
  line-height: 28px;
}

/* 隐藏旧页头的名称区域，版本信息由原始服务端数据移至页头右侧 */
.sws-shell .t td:last-child {
  display: none;
}
.sws-version {
  font-size: 11px;
  line-height: 1.5;
  color: var(--muted);
  text-align: right;
  overflow-wrap: anywhere;
  max-width: 50%;
}
@media (max-width: 700px) {
  .sws-version {
    font-size: 10px;
    max-width: 45%;
  }
  .sws-topbar {
    align-items: flex-start;
  }
}

/* 调焦器与旋转器按位置、移动、参考位置和补偿组织 */
.sws-accessory-layout {
  gap: 18px;
}
.sws-accessory-grid {
  display: grid;
  grid-template-columns: minmax(0, 1.15fr) minmax(0, 1fr);
  gap: 18px;
  align-items: start;
}
.sws-accessory-main {
  display: flex;
  flex-direction: column;
  gap: 18px;
  min-width: 0;
}
.sws-accessory-motion {
  min-width: 0;
}
.sws-accessory-home {
  min-width: 0;
}
.sws-accessory-settings > .b1 {
  flex: 0 0 auto;
}
.sws-accessory-metrics > div:only-child {
  grid-column: 1/-1;
}
.sws-accessory-settings {
  display: flex;
  flex-direction: column;
  gap: 18px;
  min-width: 0;
}
.sws-accessory-layout .b1 {
  width: 100% !important;
  padding: 20px;
  text-align: left;
  min-width: 0;
  overflow-wrap: anywhere;
  background: var(--panel);
}
.sws-device-card-head {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 12px;
  margin-bottom: 16px;
}
.sws-device-card-head h2 {
  font-size: 16px;
  line-height: 1.5;
  margin: 0;
  color: #d8d8d8;
}
.sws-accessory-status {
  font-size: 12px;
  font-weight: 500;
  white-space: nowrap;
}
.sws-accessory-position {
  container-type: inline-size;
  min-height: 96px;
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 14px;
  background: #1d1d1d;
  border-radius: 10px;
  margin-bottom: 16px;
}
.sws-accessory-position > span {
  font-size: 12px;
  color: var(--muted);
}
.sws-accessory-position strong {
  font-size: clamp(22px, 8cqi, 28px);
  line-height: 1.3;
  font-weight: 600;
  margin: auto 0;
  text-align: center;
  font-variant-numeric: tabular-nums;
}
.sws-accessory-goto {
  display: grid;
  grid-template-columns: minmax(0, 1fr) auto auto;
  gap: 10px;
  align-items: end;
}
.sws-accessory-target {
  min-width: 0;
  display: flex;
  flex-direction: column;
  gap: 6px;
  font-size: 12px;
  color: var(--muted);
}
.sws-accessory-input {
  position: relative;
  min-width: 0;
}
.sws-accessory-input input {
  width: 100%;
  min-width: 0 !important;
  margin: 0;
  padding: 10px 40px 10px 12px;
  height: 44px;
  font-size: 15px;
}
.sws-accessory-input > span {
  position: absolute;
  right: 12px;
  top: 50%;
  transform: translateY(-50%);
}
.sws-accessory-goto button {
  height: 44px;
  min-height: 44px;
  margin: 0;
  padding: 8px 14px;
  min-width: 0;
}
.sws-accessory-stop {
  background: #3e3e3e;
  border-color: #626262;
}
.sws-accessory-hint {
  font-size: 12px;
  color: var(--muted);
  line-height: 1.7;
  margin: 14px 0 10px;
}
.sws-accessory-jog {
  display: grid;
  grid-template-columns: repeat(4, minmax(0, 1fr));
  gap: 8px;
}
.sws-accessory-jog > br {
  display: none;
}
.sws-accessory-jog button {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 2px;
  width: 100% !important;
  min-width: 0;
  min-height: 68px;
  height: auto !important;
  padding: 8px 4px;
  margin: 0;
  background: #292929;
  border-color: #3c3c3c;
  touch-action: none;
  user-select: none;
}
.sws-accessory-jog button:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-accessory-jog span {
  font-size: 22px;
  line-height: 1.3;
}
.sws-accessory-jog small {
  font-size: 12px;
  white-space: normal;
  overflow-wrap: anywhere;
}
.sws-accessory-motion-card .sws-choice {
  margin: 16px 0 8px;
}
.sws-accessory-motion-card .sws-choice > span {
  font-size: 12px;
  color: var(--muted);
  font-weight: 400;
}
.sws-accessory-live-rate {
  display: flex;
  justify-content: space-between;
  gap: 12px;
  color: var(--muted);
  font-size: 12px;
}
.sws-accessory-live-rate strong {
  font-weight: 500;
}
.sws-accessory-home-actions {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
}
.sws-accessory-home-actions button {
  width: 100%;
  min-height: 44px;
  margin: 0;
  padding: 10px 8px;
  white-space: normal;
}
.sws-accessory-home-actions button:last-child {
  background: #292929;
  border-color: #3c3c3c;
}
.sws-accessory-home-card .sws-accessory-hint {
  margin-top: 0;
}
.sws-accessory-layout .sws-device-selector > .b1 {
  display: flex;
  align-items: center;
  gap: 18px;
  padding: 14px 20px;
}
.sws-device-selector .sws-focuser-select {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 16px;
  flex: 1;
  min-width: 0;
}
.sws-focuser-select big {
  font-size: 13px;
  white-space: nowrap;
}
.sws-focuser-select-buttons {
  display: flex;
  gap: 6px;
  flex-wrap: wrap;
}
.sws-focuser-select-buttons button {
  width: 40px;
  min-height: 36px;
  padding: 4px;
  margin: 0;
}
.sws-accessory-metrics {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
  clear: both;
}
.sws-accessory-metrics > div {
  display: flex;
  flex-direction: column;
  gap: 6px;
  padding: 12px;
  border-radius: 8px;
  background: #1d1d1d;
  min-width: 0;
}
.sws-accessory-metrics span {
  font-size: 12px;
  color: var(--muted);
}
.sws-accessory-metrics strong {
  font-size: 17px;
  font-weight: 500;
  text-align: center;
  margin: auto 0;
}
.sws-accessory-field {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 110px;
  align-items: center;
  gap: 14px;
  margin-bottom: 12px;
  font-size: 12px;
}
.sws-accessory-field input,
.sws-accessory-field select {
  width: 100%;
  min-width: 0 !important;
  margin: 0;
  padding: 9px 8px;
  text-align: center;
  height: 42px;
}
.sws-accessory-settings .content form > button {
  display: block;
  margin: 16px 0 0 auto;
  min-width: 100px;
  min-height: 44px;
}
.sws-accessory-settings .collapsible {
  margin: 0;
  min-height: 40px;
  padding: 8px 12px;
}
.sws-accessory-settings .content {
  padding: 14px;
}
.sws-accessory-settings .b1 > hr {
  margin: 14px 0;
}
.sws-accessory-settings .b1 > div:first-child {
  font-size: 15px;
}
.sws-accessory-derotate {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 8px;
  clear: both;
}
.sws-accessory-derotate > br {
  display: none;
}
.sws-accessory-derotate button {
  margin: 0;
  white-space: normal;
  min-height: 44px;
}
.sws-accessory-derotate button:nth-of-type(n + 3) {
  grid-column: 1/-1;
  background: #292929;
  border-color: #3c3c3c;
}

/* 附件参数卡片统一标题、折叠栏与表单间距 */
.sws-accessory-parameter-card > .sws-device-card-head {
  margin-bottom: 14px;
  flex-wrap: wrap;
}
.sws-accessory-parameter-card > .sws-accessory-metrics {
  margin-bottom: 14px;
}
.sws-accessory-parameter-card .collapsible {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  width: 100%;
  margin: 0;
  min-height: 42px;
  padding: 9px 12px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 8px;
  font-size: 13px;
  color: var(--text);
}
.sws-accessory-parameter-card .collapsible::after {
  content: '+';
  color: var(--muted);
}
.sws-accessory-parameter-card .collapsible.active::after {
  content: '−';
}
.sws-accessory-parameter-card .collapsible:hover {
  background-image: linear-gradient(rgb(185 185 185 / 8%), rgb(185 185 185 / 8%));
}
.sws-accessory-parameter-card .content {
  padding: 14px 0 0;
  background: transparent;
  border-radius: 0;
}
.sws-accessory-parameter-card .content > br {
  display: none;
}
.sws-accessory-parameter-card .sws-accessory-field {
  font-size: 13px;
  line-height: 1.6;
  grid-template-columns: minmax(0, 1fr) 110px;
  gap: 14px;
  margin: 0;
  padding: 10px 0;
  border-bottom: 1px solid #292929;
}
.sws-accessory-parameter-card .sws-accessory-field:first-child {
  padding-top: 0;
}
.sws-accessory-parameter-card .sws-accessory-field input,
.sws-accessory-parameter-card .sws-accessory-field select {
  height: 42px;
  font-size: 14px;
}
.sws-accessory-form-actions {
  display: flex;
  justify-content: flex-end;
  padding-top: 14px;
}
.sws-accessory-form-actions button {
  margin: 0;
  min-width: 110px;
  min-height: 42px;
  padding: 8px 16px;
  font-size: 13px;
}
@media (max-width: 500px) {
  .sws-accessory-parameter-card .sws-accessory-field {
    grid-template-columns: minmax(0, 1fr) 90px;
    gap: 10px;
  }
}

@media (max-width: 900px) {
  .sws-accessory-grid {
    grid-template-columns: 1fr;
  }
  .sws-accessory-layout .b1 {
    padding: 18px;
  }
}
@media (max-width: 500px) {
  .sws-accessory-layout .sws-device-selector > .b1 {
    align-items: flex-start;
    flex-direction: column;
    gap: 8px;
    padding: 14px;
  }
  .sws-device-selector .sws-focuser-select {
    width: 100%;
    flex-wrap: wrap;
    gap: 8px;
  }
  .sws-accessory-goto {
    grid-template-columns: 1fr 1fr;
    gap: 8px;
  }
  .sws-accessory-target {
    grid-column: 1/-1;
  }
  .sws-accessory-goto button {
    width: 100%;
  }
  .sws-accessory-field {
    grid-template-columns: minmax(0, 1fr) 90px;
    gap: 10px;
  }
  .sws-accessory-settings #f_temp {
    float: none !important;
    clear: both;
    text-align: left !important;
    padding-top: 6px;
    font-size: 12px;
  }
  .sws-accessory-layout .b1 {
    padding: 16px;
  }
  .sws-accessory-jog {
    gap: 6px;
  }
}
/* 编码器按同步操作、实时读数与两轴设置排列。 */
.sws-encoder-layout {
  gap: 20px;
}
.sws-encoder-layout .b1 {
  width: 100% !important;
  padding: 20px;
  text-align: left;
}
.sws-encoder-sync-grid {
  display: grid;
  grid-template-columns: minmax(0, 1.15fr) minmax(0, 1fr);
  gap: 24px;
  align-items: center;
}
.sws-encoder-sync-actions {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
}
.sws-encoder-sync-actions button {
  width: 100%;
  min-height: 46px;
  margin: 0;
  padding: 10px;
  white-space: normal;
  line-height: 1.5;
}
.sws-encoder-origin {
  grid-column: 1 / -1;
}
.sws-encoder-auto {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 16px;
  padding-left: 24px;
  border-left: 1px solid var(--border);
}
.sws-encoder-auto > span {
  font-size: 14px;
  color: var(--muted);
}
.sws-encoder-toggle {
  display: flex;
  gap: 8px;
  flex-shrink: 0;
}
.sws-encoder-toggle button {
  margin: 0;
  min-width: 48px;
  min-height: 42px;
}
.sws-encoder-grid {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 18px;
  align-items: start;
}
.sws-encoder-readings {
  display: grid;
  grid-template-columns: repeat(3, minmax(0, 1fr));
  gap: 8px;
  margin: 14px 0;
}
.sws-encoder-metric {
  container-type: inline-size;
  min-width: 0;
  min-height: 88px;
  padding: 12px;
  border-radius: 8px;
  background: var(--field);
  display: flex;
  flex-direction: column;
  gap: 10px;
}
.sws-encoder-metric > span {
  font-size: 12px;
  color: var(--muted);
}
.sws-encoder-metric > strong {
  min-width: 0;
  margin: auto 0;
  text-align: center;
  font-size: clamp(15px, 14cqi, 24px);
  font-weight: 600;
  line-height: 1.3;
  font-variant-numeric: tabular-nums;
}
.sws-encoder-values {
  margin: 0 0 16px;
  font-size: 13px;
}
.sws-encoder-values > div {
  display: flex;
  align-items: baseline;
  justify-content: space-between;
  gap: 14px;
  padding: 9px 0;
  border-bottom: 1px solid var(--border);
}
.sws-encoder-values dt {
  color: var(--muted);
}
.sws-encoder-values dd {
  margin: 0;
  text-align: right;
  font-size: 15px;
  font-variant-numeric: tabular-nums;
}
.sws-encoder-layout .content {
  padding: 14px 0 0;
  background: transparent;
}
.sws-encoder-layout .content > br {
  display: none;
}
.sws-encoder-field {
  display: grid;
  grid-template-columns: minmax(0, 1fr) 140px;
  align-items: center;
  gap: 16px;
  margin: 0 0 12px;
  font-size: 13px;
}
.sws-encoder-field > span {
  overflow-wrap: anywhere;
}
.sws-encoder-field input {
  margin: 0;
  width: 100%;
  min-width: 0;
  text-align: center;
}
.sws-encoder-form-actions {
  display: flex;
  justify-content: flex-end;
  gap: 8px;
  padding-top: 4px;
}
.sws-encoder-form-actions button {
  margin: 0;
  min-width: 96px;
}
@media (max-width: 1000px) {
  .sws-encoder-sync-grid {
    grid-template-columns: 1fr;
    gap: 16px;
  }
  .sws-encoder-auto {
    border-left: 0;
    border-top: 1px solid var(--border);
    padding: 16px 0 0;
  }
}
@media (max-width: 850px) {
  .sws-encoder-grid {
    grid-template-columns: 1fr;
  }
}
@media (max-width: 500px) {
  .sws-encoder-layout .b1 {
    padding: 16px;
  }
  .sws-encoder-readings {
    grid-template-columns: 1fr;
  }
  .sws-encoder-metric {
    display: grid;
    grid-template-columns: 72px minmax(0, 1fr);
    min-height: 58px;
    align-items: center;
  }
  .sws-encoder-metric > span {
    overflow-wrap: anywhere;
  }
  .sws-encoder-metric > strong {
    font-size: clamp(14px, 7cqi, 20px);
  }
  .sws-encoder-sync-actions {
    grid-template-columns: 1fr;
  }
  .sws-encoder-auto {
    align-items: flex-start;
  }
  .sws-encoder-toggle button {
    min-width: 42px;
  }
  .sws-encoder-field {
    grid-template-columns: minmax(0, 1fr) 100px;
    gap: 10px;
  }
  .sws-encoder-metric {
    padding: 10px 8px;
  }
}
/* 时间地点、限位与校准说明共用清楚的层级与紧凑表单。 */
.sws-mount .sws-advanced-grid {
  align-items: start;
}
.sws-mount .sws-advanced-grid > .b1 {
  height: auto;
  align-self: start;
}
.sws-align-correction {
  padding: 16px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 10px;
}
.sws-align-correction > div {
  display: flex;
  align-items: center;
  gap: 12px;
  margin-bottom: 8px;
}
.sws-align-correction strong {
  font-size: 24px;
  line-height: 1.3;
  font-weight: 600;
}
.sws-align-correction > span {
  color: var(--muted);
  font-size: 14px;
  line-height: 1.6;
}
.sws-site .content > br,
.sws-limits .content > br,
.sws-align .content > br,
.sws-pec .content > br {
  display: none;
}
.sws-site .content,
.sws-limits .content,
.sws-align .content {
  padding: 16px 0 0;
  background: transparent;
}
.sws-limit-values {
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 10px;
  margin: 14px 0;
}
.sws-limit-values > div {
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  gap: 12px;
  min-height: 100px;
  padding: 14px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 10px;
}
.sws-limit-values > div > span {
  color: var(--muted);
  font-size: 14px;
  line-height: 1.6;
}
.sws-limit-values strong {
  color: var(--text);
  font-size: 24px;
  line-height: 1.25;
  font-weight: 600;
  text-align: center;
  font-variant-numeric: tabular-nums;
}
.sws-limits-form {
  display: grid;
  gap: 14px;
}
.sws-limits-form fieldset {
  margin: 0;
  padding: 14px;
  min-width: 0;
  background: #1d1d1d;
  border: 1px solid var(--border);
  border-radius: 10px;
}
.sws-limits-form legend {
  padding: 0 6px;
  font-size: 15px;
  font-weight: 600;
  color: var(--text);
}
.sws-limits .sws-number-field {
  grid-template-columns: minmax(0, 1fr) 112px;
  gap: 16px;
  margin: 12px 0;
  font-size: 14px;
  line-height: 1.6;
}
.sws-limits .sws-number-field input {
  height: 46px;
  font-size: 17px;
  text-align: center;
}
.sws-form-footer {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
  padding-top: 4px;
}
.sws-form-footer button {
  margin: 0;
  min-width: 110px;
}
.sws-refine-title {
  margin: 0 0 12px;
  font-size: 16px;
  font-weight: 600;
}
.sws-refine-steps {
  counter-reset: refine-step;
  list-style: none;
  padding: 0;
  margin: 0;
  display: grid;
  gap: 10px;
}
.sws-refine-steps li {
  position: relative;
  padding: 14px 14px 14px 50px;
  background: #222;
  border: 1px solid var(--border);
  border-radius: 9px;
  font-size: 14px;
  line-height: 1.8;
  color: var(--text);
}
.sws-refine-steps li::before {
  counter-increment: refine-step;
  content: counter(refine-step);
  position: absolute;
  left: 14px;
  top: 16px;
  display: grid;
  place-items: center;
  width: 24px;
  height: 24px;
  border-radius: 50%;
  background: #3b3b3b;
  font-size: 13px;
  font-weight: 600;
}
.sws-refine-action {
  display: flex;
  justify-content: flex-end;
  margin-top: 14px;
}
.sws-refine-action button {
  margin: 0;
  min-width: 130px;
}
.sws-pec-status {
  display: flex;
  justify-content: space-between;
  align-items: center;
  gap: 14px;
  padding: 14px;
  background: var(--field);
  border: 1px solid var(--border);
  border-radius: 10px;
  font-size: 14px;
}
.sws-pec-status > span {
  color: var(--muted);
}
.sws-pec-status strong {
  font-size: 17px;
  font-weight: 500;
  color: var(--text);
}
.sws-pec > hr:nth-of-type(2) {
  margin: 14px 0;
}
.sws-time-label,
.sws-site-fields legend {
  font-size: 14px;
  line-height: 1.6;
}
.sws-time-summary > .sws-device-time,
.sws-time-summary > .sws-sidereal-time,
.sws-site .sws-location-values > div {
  container-type: inline-size;
  min-height: 112px;
  padding: 16px;
  gap: 12px;
  border: 1px solid var(--border);
  border-radius: 10px;
  background: var(--field);
}
.sws-time-summary .sws-device-time > div,
.sws-time-summary #time_lst,
.sws-site .sws-location-values strong {
  font-size: clamp(18px, 8cqi, 25px);
  line-height: 1.4;
  font-weight: 600;
}
.sws-site #date_ut,
.sws-site #time_ut {
  font-size: inherit;
  line-height: inherit;
  white-space: nowrap;
}
.sws-time-summary #datetime {
  font-size: 16px;
  font-weight: 500;
}
.sws-site .sws-location-values > div {
  font-size: 14px;
  color: var(--muted);
}
.sws-site .sws-location-values strong {
  color: var(--text);
}
.sws-site .sws-clock-action button {
  min-height: 44px;
  font-size: 15px;
}
.sws-site-fields {
  grid-template-columns: repeat(auto-fit, minmax(min(100%, 260px), 1fr));
  gap: 14px;
}
.sws-site-fields .sws-field-group {
  padding: 12px 14px 14px;
  border: 1px solid var(--border);
  border-radius: 10px;
  background: #1d1d1d;
}
.sws-site-fields legend {
  padding: 0 6px;
}
.sws-site-fields .sws-unit-inputs {
  max-width: none;
}
.sws-site-fields .sws-unit-inputs input,
.sws-site-fields .sws-unit-inputs select {
  height: 46px;
  font-size: 17px;
}
.sws-site .sws-site-fields .sws-help {
  font-size: 13px;
  line-height: 1.8;
}
@media (max-width: 600px) {
  .sws-site-fields {
    grid-template-columns: 1fr;
  }
  .sws-limit-values {
    grid-template-columns: 1fr;
  }
  .sws-limit-values > div {
    min-height: 80px;
    display: grid;
    grid-template-columns: minmax(0, 1fr) 70px;
    align-items: center;
    gap: 10px;
  }
  .sws-limit-values strong {
    font-size: 22px;
  }
  .sws-limits .sws-number-field {
    grid-template-columns: minmax(0, 1fr) 88px;
    gap: 12px;
  }
  .sws-limits-form fieldset {
    padding: 12px;
  }
  .sws-refine-action button,
  .sws-form-footer button {
    width: 100%;
  }
}
</style>
)UI";


PREVIEW_FILE_39


// 页面增强只代理已有控件；设备参数与命令仍由原页面处理。
const char html_page_controls[] PROGMEM = R"UI(
<script>
// 坐标由单次请求提交，保留错误反馈且不在连接中断时自动重发运动指令
document.addEventListener('DOMContentLoaded', () => {
  const form = document.getElementById('sws-coordinate-form');
  if (!form) return;
  let pending = false;
  form.addEventListener('submit', async (event) => {
    event.preventDefault();
    if (pending || !form.reportValidity()) return;
    const values = new FormData(form);
    const result = form.querySelector('[role=status]');
    if (
      Number(values.get('dec_d')) === 90 &&
      (Number(values.get('dec_m')) || Number(values.get('dec_s')))
    ) {
      result.textContent = form.dataset.invalid;
      return;
    }
    values.set('coord_action', event.submitter?.value || 'go');
    pending = true;
    const buttons = [...form.querySelectorAll('button')];
    buttons.forEach((b) => (b.disabled = true));
    result.textContent = form.dataset.send;
    const abort = new AbortController(),
      timer = setTimeout(() => abort.abort(), 15000);
    try {
      const response = await fetch('/mount-ajax-get.txt?' + new URLSearchParams(values), {
        signal: abort.signal,
        cache: 'no-store',
      });
      if (!response.ok) throw Error();
      const reply = (await response.text()).trim();
      result.textContent =
        reply === 'ok'
          ? form.dataset.ok
          : reply === 'invalid'
            ? form.dataset.invalid
            : form.dataset.reject;
    } catch {
      result.textContent = form.dataset.network;
    } finally {
      clearTimeout(timer);
      pending = false;
      buttons.forEach((b) => (b.disabled = false));
    }
  });
});

// 为卡片应用职责样式，并给度分秒输入框提供可识别的名称
document.addEventListener('DOMContentLoaded', () => {
  document
    .querySelectorAll('.sws-card-marker')
    .forEach((marker) => marker.closest('.b1').classList.add('sws-' + marker.dataset.card));
  document.querySelectorAll('.sws-field-group').forEach((group) => {
    const title = group.querySelector('legend').textContent;
    group
      .querySelectorAll('input,select')
      .forEach((field, i) =>
        field.setAttribute(
          'aria-label',
          title + ' ' + (field.nextSibling?.textContent.trim() || String(i + 1)),
        ),
      );
  });
});

// 下拉框代理原有按钮，后台状态更新仍通过原来的标识同步
document.addEventListener('DOMContentLoaded', () => {
  const tracking = document.querySelector('.sws-tracking .b1');
  // 方位控制标题去掉冒号，保留倍率与状态节点
  const guideTitle = document.querySelector('.sws-guide>.b1>div:first-child');
  if (guideTitle)
    guideTitle.childNodes.forEach((node) => {
      if (node.nodeType === Node.TEXT_NODE)
        node.textContent = node.textContent.replace(/[:：]\s*$/, '');
    });
  document.querySelectorAll('.sws-choice-source').forEach((source) => {
    const buttons = [...source.querySelectorAll('button')];
    if (!buttons.length) return;
    const label = document.createElement('label');
    label.className = 'sws-choice';
    const text = document.createElement('span');
    text.textContent = source.dataset.label;
    const select = document.createElement('select');
    buttons.forEach((button, i) => {
      const option = document.createElement('option');
      option.value = i;
      option.textContent = button.title || button.textContent;
      select.appendChild(option);
    });
    label.append(text, select);
    source.before(label);
    label.appendChild(source);
    source.classList.add('sws-choice-originals');
    select.onchange = () =>
      buttons[Number(select.value)].dispatchEvent(
        new PointerEvent('pointerdown', { bubbles: true }),
      );
    const sync = () => {
      buttons.forEach((button, i) => {
        select.options[i].disabled = button.disabled;
        if (button.dataset.selected === 'true') select.value = i;
      });
      select.disabled = buttons.every((button) => button.disabled);
    };
    new MutationObserver(sync).observe(source, {
      subtree: true,
      attributes: true,
      attributeFilter: ['data-selected', 'disabled'],
    });
    sync();
    if (tracking && source.classList.contains('sws-guide-choice')) {
      let collapse = tracking.querySelector('.collapsible');
      while (collapse && collapse.parentElement !== tracking) collapse = collapse.parentElement;
      tracking.insertBefore(label, collapse || null);
    }
  });
  const on = document.getElementById('trk_on'),
    off = document.getElementById('trk_off');
  if (on && off) {
    const wrap = document.createElement('div');
    wrap.className = 'sws-track-switch';
    const name = document.createElement('span');
    name.textContent =
      on.dataset.label || tracking.querySelector('div').textContent.replace(/:$/, '');
    const toggle = document.createElement('button');
    toggle.className = 'sws-switch';
    toggle.type = 'button';
    toggle.setAttribute('role', 'switch');
    toggle.setAttribute('aria-label', name.textContent);
    toggle.setAttribute('aria-checked', 'false');
    on.before(wrap);
    wrap.append(name, toggle);
    on.hidden = true;
    off.hidden = true;
    toggle.onclick = () => {
      const checked = toggle.getAttribute('aria-checked') === 'true';
      (checked ? off : on).dispatchEvent(new PointerEvent('pointerdown', { bubbles: true }));
    };
    const sync = () => {
      toggle.setAttribute('aria-checked', String(on.dataset.selected === 'true'));
      toggle.disabled = on.disabled && off.disabled;
    };
    const observer = new MutationObserver(sync);
    [on, off].forEach((b) =>
      observer.observe(b, { attributes: true, attributeFilter: ['data-selected', 'disabled'] }),
    );
    sync();
  }
});

// 内容页使用独立文档，离开时由浏览器清理轮询与事件，导航和页头保持不动
if (window.parent !== window && window.parent.swsNavigate) {
  document.documentElement.classList.add('sws-embedded');
  document.addEventListener('DOMContentLoaded', () => {
    document.querySelectorAll('form').forEach((form) => {
      if (form.method.toLowerCase() === 'get') {
        const input = document.createElement('input');
        input.type = 'hidden';
        input.name = 'view';
        input.value = '1';
        form.appendChild(input);
      }
    });
    document.querySelectorAll('.t a').forEach((a) =>
      a.addEventListener('click', (e) => {
        if (!e.ctrlKey && !e.metaKey && !e.shiftKey) {
          e.preventDefault();
          parent.swsNavigate(a.pathname, true);
        }
      }),
    );
  });
}
</script>
)UI";


PREVIEW_FILE_40


// 位置、限位卡片的显示与编辑状态；表单仍提交原来的参数。
const char html_settings_editors[] PROGMEM = R"UI(
<style>
.sws-control-grid { grid-template-columns:repeat(2,minmax(0,1fr)); grid-template-areas:'tracking guide'; }
.sws-mount > .sws-target { grid-area:auto; }
.sws-site-limits { display:grid; grid-template-columns:repeat(2,minmax(0,1fr)); gap:18px; align-items:stretch; }
.sws-mount .sws-editor { width:100%; min-height:0; padding:20px; margin:0; }
.sws-editor [hidden], .sws-park-toggle [hidden] { display:none !important; }
.sws-editor-heading { display:flex; align-items:center; justify-content:space-between; gap:12px; padding-bottom:16px; margin-bottom:20px; border-bottom:1px solid var(--border); }
.sws-editor-heading h2 { margin:0; font-size:20px; line-height:1.4; }
.sws-mount .sws-edit-button { display:inline-flex; align-items:center; justify-content:center; gap:6px; flex-shrink:0; width:auto; min-height:36px; margin:0; padding:8px 12px; font-size:14px; }
.sws-edit-button svg { width:16px; height:16px; }
.sws-editor .sws-site-fields { display:grid; grid-template-columns:1fr; gap:22px; }
.sws-editor .sws-site-fields fieldset { margin:0; padding:0; border:0; background:transparent; }
.sws-editor legend, .sws-editor .sws-field-label { display:block; padding:0; margin:0 0 10px; font-size:15px; font-weight:600; line-height:1.6; color:var(--text); }
.sws-editor .sws-unit-inputs { display:flex; align-items:center; gap:7px; width:100%; max-width:none; }
.sws-editor .sws-unit-inputs input { flex:1; min-width:0; width:0 !important; margin:0; }
.sws-editor .sws-unit-inputs select { margin:0; min-width:0; width:64px; }
.sws-editor input, .sws-editor select { height:44px; font-size:17px; color:var(--text); font-weight:500; }
.sws-editor input[readonly], .sws-editor select:disabled { opacity:1; cursor:default; color:var(--text); background:var(--field); -webkit-text-fill-color:var(--text); }
.sws-editor .sws-unit-inputs > .sws-signed-input { flex:1; min-width:0; }
.sws-signed-input { display:flex; align-items:center; gap:8px; }
.sws-signed-input > input:not([hidden]) { width:0 !important; flex:1; min-width:0; margin:0; }
.sws-editor .sws-signed-input > select { width:62px; flex-shrink:0; }
.sws-editor .sws-timezone-select { width:100%; margin:0; }
.sws-editor .sws-help { margin:10px 0 0; font-size:12px; line-height:1.7; color:var(--muted); }
.sws-editor .sws-limits-form { display:grid; gap:22px; }
.sws-editor .sws-number-field { display:block; margin:0; }
.sws-editor .sws-number-field > span { display:block; margin-bottom:10px; font-size:15px; font-weight:600; line-height:1.6; }
.sws-editor .sws-number-field > input { width:100% !important; text-align:center; margin:0; }
.sws-editor .sws-number-field .sws-signed-input > input { width:0 !important; }
.sws-editor-footer { display:flex; justify-content:flex-end; gap:10px; margin-top:20px; }
.sws-editor-footer button { min-width:95px; margin:0; }
.sws-editor[data-editing='false'] .sws-editor-footer { display:none; }
.sws-backlash-editor { padding-top:16px; border-top:1px solid var(--border); }
.sws-backlash-editor summary { cursor:pointer; font-size:14px; color:var(--muted); }
.sws-backlash-editor .sws-number-field { margin-top:16px; }
.sws-clock-controls { display:flex; align-items:center; justify-content:space-between; gap:12px; margin-top:22px; padding-top:18px; border-top:1px solid var(--border); }
.sws-editor .sws-site-clock { min-width:0; }
.sws-clock-value strong { display:block; font-size:19px; line-height:1.6; font-weight:600; font-variant-numeric:tabular-nums; }
.sws-sidereal-clock { display:flex; gap:10px; margin-top:6px; font-size:13px; color:var(--muted); }
.sws-sidereal-clock strong { font-weight:400; }
.sws-editor .sws-clock-action { margin:0; }
.sws-editor .sws-clock-action button { width:auto; min-height:42px; margin:0; font-size:14px; white-space:normal; }
.sws-browser-clock { display:flex; justify-content:space-between; flex-wrap:wrap; gap:6px; margin:14px 0 0; font-size:12px; color:var(--muted); }
.sws-browser-clock strong { font-weight:400; }
.sws-park-toggle { display:flex; flex:1; min-width:0; }
.sws-park-toggle > button { width:100%; margin:0; }
.sws-library-section > .b1 { width:100%; }
@media(max-width:700px) {
  .sws-control-grid { grid-template-columns:1fr; grid-template-areas:'tracking' 'guide'; }
  .sws-site-limits { grid-template-columns:1fr; gap:14px; }
  .sws-mount .sws-editor { padding:16px; }
  .sws-editor-heading { gap:8px; }
  .sws-editor-heading h2 { font-size:18px; }
  .sws-mount .sws-edit-button { font-size:13px; padding:6px 10px; }
  .sws-editor .sws-unit-inputs { gap:4px; font-size:12px; }
  .sws-editor .sws-unit-inputs input, .sws-editor select { font-size:16px; }
  .sws-clock-value strong { font-size:17px; }
  .sws-clock-controls { align-items:flex-start; }
}
</style>
<script>
document.addEventListener('DOMContentLoaded', () => {
  // 有符号原值保留在原输入框，方向选择只提供更直观的显示。
  function signedInput(source, directions) {
    if (!source) return;
    const box = document.createElement('div');
    box.className = 'sws-signed-input';
    source.before(box);
    const value = document.createElement('input'), sign = document.createElement('select');
    value.type = 'number'; value.min = '0'; value.max = Math.max(Math.abs(Number(source.min)), Math.abs(Number(source.max)));
    value.step = source.step || '1'; value.required = true;
    value.defaultValue = String(Math.abs(Number(source.value)));
    value.setAttribute('aria-label', source.getAttribute('aria-label') || source.closest('label,fieldset').textContent.trim());
    directions.forEach(([key, text]) => sign.add(new Option(text, key)));
    sign.value = source.value.startsWith('-') ? '-' : '+';
    sign.selectedOptions[0].defaultSelected = true;
    sign.setAttribute('aria-label', value.getAttribute('aria-label'));
    source.hidden = true;
    const units = box.closest('.sws-unit-inputs');
    if (units) { box.append(source, value); units.append(sign); }
    else box.append(source, sign, value);
    const sync = () => { source.value = value.value === '' ? '' : (sign.value === '-' ? '-' : '') + value.value; };
    value.addEventListener('input', sync); sign.addEventListener('change', sync);
  }
  const site = document.getElementById('sws-site-form');
  if (site) {
    signedInput(site.elements.g1, [['-', )UI" SWS_JS_TEXT(BUTTON_E) R"UI(], ['+', )UI" SWS_JS_TEXT(BUTTON_W) R"UI(]]);
    signedInput(site.elements.t1, [['+', )UI" SWS_JS_TEXT(BUTTON_N) R"UI(], ['-', )UI" SWS_JS_TEXT(BUTTON_S) R"UI(]]);
    const hours = site.elements.u1, minutes = site.elements.u2;
    const group = hours.closest('fieldset'), select = document.createElement('select');
    select.className = 'sws-timezone-select';
    select.setAttribute('aria-label', group.querySelector('legend').textContent);
    const current = (Math.abs(Number(hours.value))*60 + Number(minutes.value)) * (hours.value.startsWith('-') ? -1 : 1);
    const offsets = [];
    for (let west = -840; west <= 720; west += 15) if (Math.abs(west)%60 !== 15) offsets.push(west);
    if (!offsets.includes(current)) offsets.push(current);
    offsets.sort((a,b) => b-a).forEach((west) => {
      const zone = -west, magnitude = Math.abs(zone);
      const text = 'UTC' + (zone < 0 ? '−' : '+') + String(Math.floor(magnitude/60)).padStart(2,'0') + ':' + String(magnitude%60).padStart(2,'0');
      const option = new Option(text, west, west === current, west === current); select.add(option);
    });
    group.querySelector('.sws-unit-inputs').hidden = true;
    group.querySelector('.sws-help').hidden = true;
    group.append(select);
    select.onchange = () => {
      const west = Number(select.value);
      hours.value = (west < 0 ? '-' : '') + Math.floor(Math.abs(west)/60);
      minutes.value = Math.abs(west)%60;
    };
  }
  const limits = document.getElementById('sws-limits-form');
  if (limits) ['hl','el','wl'].forEach(name => signedInput(limits.elements[name], [['+','+'], ['-','−']]));
  document.querySelectorAll('.sws-settings-editor').forEach((form) => {
    const card = form.closest('.sws-editor'), edit = card.querySelector('[data-edit-form]');
    const fields = [...form.querySelectorAll('input,select')];
    const setEditing = (editing) => {
      card.dataset.editing = String(editing);
      fields.forEach(field => { if (field.tagName === 'SELECT') field.disabled = !editing; else field.readOnly = !editing; });
      edit.disabled = editing; edit.setAttribute('aria-expanded', String(editing));
    };
    edit.setAttribute('aria-controls', form.id);
    edit.onclick = () => { setEditing(true); form.querySelector('input:not([hidden]),select:not(:disabled)').focus(); };
    form.addEventListener('reset', () => setTimeout(() => setEditing(false), 0));
    // 表单数据构造完毕后再锁定下拉框，避免 disabled 导致原参数丢失。
    form.addEventListener('formdata', () => queueMicrotask(() => setEditing(false)));
    setEditing(false);
  });
  const park = document.getElementById('park'), unpark = document.getElementById('unpark');
  if (park && unpark) {
    const sync = () => { park.hidden = !unpark.disabled; unpark.hidden = unpark.disabled; };
    new MutationObserver(sync).observe(unpark, {attributes:true,attributeFilter:['disabled']});
    sync();
  }
});
</script>
)UI";


PREVIEW_FILE_41



// 导航仅加载当前标签内容，浏览器前进后退同步到同一内容窗口
const char html_shell_script[] PROGMEM = R"UI(
<script>
(() => {
  // 意外以普通地址打开内容页时直接纠正，避免出现嵌套导航
  if (parent !== window && parent.swsNavigate) {
    location.replace(location.pathname + '?view=1');
    return;
  }
  const frame = document.getElementById('sws-frame'),
    hint = document.getElementById('sws-load'),
    links = [...document.querySelectorAll('.t a')];
  let sequence = 0,
    timer;
  // 参考实心设备图标重绘剪影，望远镜保留倾斜镜筒、镜口与底座
  // 图形内嵌且继承导航颜色，离线可用并同步适配桌面和手机
  const icons = {
    index:
      '<path d="M8 5h16a3 3 0 0 1 3 3v16a3 3 0 0 1-3 3H8a3 3 0 0 1-3-3V8a3 3 0 0 1 3-3Zm3 5v12h10V10Z" fill-rule="evenodd"/><path d="M9 1h3v4H9zm6 0h3v4h-3zm6 0h3v4h-3zM9 27h3v4H9zm6 0h3v4h-3zm6 0h3v4h-3zM1 9h4v3H1zm0 6h4v3H1zm0 6h4v3H1zM27 9h4v3h-4zm0 6h4v3h-4zm0 6h4v3h-4z" opacity=".72"/><rect x="13" y="12" width="6" height="8" rx="1"/>',
    mount:
      '<path d="M13 20h5l1 5 4 4H9l4-5Z" opacity=".78"/><g transform="rotate(-28 16 14)"><path d="M12 8h13c5 0 5 12 0 12H12Zm13.5 1.8c-2.6 0-2.6 8.4 0 8.4s2.6-8.4 0-8.4Z" fill-rule="evenodd"/><path d="M7 10h4v8H7a1 1 0 0 1-1-1v-6a1 1 0 0 1 1-1Z" opacity=".8"/><path d="M2 12h3v4H2a1 1 0 0 1-1-1v-2a1 1 0 0 1 1-1Z" opacity=".65"/></g>',
    focuser:
      '<path d="M3 2h7v3H5v5H2V3a1 1 0 0 1 1-1Zm19 0h7a1 1 0 0 1 1 1v7h-3V5h-5ZM2 22h3v5h5v3H3a1 1 0 0 1-1-1Zm25 0h3v7a1 1 0 0 1-1 1h-7v-3h5Z" opacity=".72"/><path d="M16 6a10 10 0 1 0 0 20 10 10 0 0 0 0-20Zm0 4a6 6 0 1 1 0 12 6 6 0 0 1 0-12Z" fill-rule="evenodd"/><circle cx="16" cy="16" r="3"/>',
    rotator:
      '<path d="M6.2 8.4A12 12 0 0 1 27 10h4L25 17l-6-7h4a8 8 0 0 0-13.7.8Z"/><path d="M25.8 23.6A12 12 0 0 1 5 22H1l6-7 6 7H9a8 8 0 0 0 13.7-.8Z" opacity=".72"/><circle cx="16" cy="16" r="3"/>',
    auxiliary:
      '<path d="M8 3h16a6 6 0 0 1 0 12H8A6 6 0 0 1 8 3Zm0 3a3 3 0 1 0 0 6 3 3 0 0 0 0-6Z" fill-rule="evenodd"/><path d="M8 18h16a6 6 0 0 1 0 12H8a6 6 0 0 1 0-12Zm16 3a3 3 0 1 0 0 6 3 3 0 0 0 0-6Z" fill-rule="evenodd" opacity=".72"/>',
    enc: '<path d="M16 2a14 14 0 1 0 0 28 14 14 0 0 0 0-28Zm-1 2h2v4h-2ZM5.1 8l1.4-1.4 2.8 2.8L8 10.8ZM4 15h4v2H4Zm3 10-1.4-1.4 2.8-2.8L10 22.2ZM15 24h2v4h-2Zm10-1-1.4 1.4-2.8-2.8 1.4-1.4ZM24 15h4v2h-4Zm.9-7-2.8 2.8-1.4-1.4 2.8-2.8ZM16 11a5 5 0 1 0 0 10 5 5 0 0 0 0-10Z" fill-rule="evenodd"/><circle cx="16" cy="16" r="2.5" opacity=".72"/>',
    net: '<path d="M1 9a23 23 0 0 1 30 0l-3 3a19 19 0 0 0-24 0Zm5 6a15 15 0 0 1 20 0l-3 3a11 11 0 0 0-14 0Zm5 6a8 8 0 0 1 10 0l-5 6Z"/>',
    other:
      '<path d="M5 4h22a2 2 0 0 1 2 2v20a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2Zm3 5v5h5V9Zm11 0v5h5V9ZM8 19v4h5v-4Zm11 0v4h5v-4Z" fill-rule="evenodd"/>',
  };
  const nav = document.createElement('nav');
  nav.className = 'sws-nav';
  nav.setAttribute('aria-label', )UI" SWS_JS_TEXT(L_UI_NAVIGATION) R"UI();
  links.forEach((a) => {
    const text = a.textContent,
      key = a.pathname.split('/').pop().split('.')[0];
    a.innerHTML =
      '<svg viewBox="0 0 32 32" aria-hidden="true" focusable="false">' +
      (icons[key] || icons.other) +
      '</svg>';
    const label = document.createElement('span');
    label.textContent = text;
    a.appendChild(label);
    nav.appendChild(a);
  });
  const extras = document.createElement('div');
  extras.className = 'sws-extra-nav';
  links
    .filter((a) => !['/index.htm', '/mount.htm', '/net.htm'].includes(a.pathname))
    .forEach((a) => extras.appendChild(a));
  nav.appendChild(extras);
  const more = document.createElement('button');
  more.type = 'button';
  more.className = 'sws-nav-more';
  more.setAttribute('aria-expanded', 'false');
  more.innerHTML =
    '<svg viewBox="0 0 32 32" aria-hidden="true" focusable="false"><circle cx="6" cy="16" r="2.5"/><circle cx="16" cy="16" r="2.5"/><circle cx="26" cy="16" r="2.5"/></svg><span></span>';
  more.querySelector('span').textContent = )UI" SWS_JS_TEXT(L_UI_MORE) R"UI(;
  const closeMore = () => {
    nav.dataset.expanded = 'false';
    more.setAttribute('aria-expanded', 'false');
  };
  more.onclick = () => {
    const expanded = nav.dataset.expanded !== 'true';
    nav.dataset.expanded = String(expanded);
    more.setAttribute('aria-expanded', String(expanded));
  };
  if (extras.children.length) nav.appendChild(more);
  document.querySelector('.t').appendChild(nav);
  addEventListener('keydown', (e) => {
    if (e.key === 'Escape') closeMore();
  });
  document.addEventListener('click', (e) => {
    if (!nav.contains(e.target)) closeMore();
  });
  // 版本沿用原始页头的服务端输出，不额外写入品牌名称
  // 左侧配置名称保留原始页头来源，说明从当前语言资源读取
  const note = document.createElement('div');
  note.className = 'sws-sidebar-note';
  note.textContent = document.getElementById('sws-sidebar-label').textContent;
  document.querySelector('.t').appendChild(note);
  const topbar = document.createElement('header');
  topbar.className = 'sws-topbar';
  topbar.innerHTML = '<div><h1 id="sws-page-title"></h1><p id="sws-page-description"></p></div>';
  const version = document.createElement('span');
  version.className = 'sws-version';
  version.textContent = document.querySelector('.t td:last-child')?.textContent || '';
  if (version.textContent) topbar.appendChild(version);
  hint.before(topbar);
  const normalize = (path) => (path === '/' ? '/index.htm' : path);
  function resize() {
    const bottom = matchMedia('(max-width:700px)').matches
      ? document.querySelector('.t').getBoundingClientRect().height
      : 0;
    frame.style.height =
      Math.max(180, innerHeight - frame.getBoundingClientRect().top - bottom) + 'px';
  }
  window.swsNavigate = async (path, push = false) => {
    path = normalize(path);
    if (!links.some((a) => a.pathname === path)) return;
    closeMore();
    more.dataset.active = String(!['/index.htm', '/mount.htm', '/net.htm'].includes(path));
    const ticket = ++sequence;
    clearTimeout(timer);
    hint.textContent = )UI" SWS_JS_TEXT(L_UI_OPENING) R"UI(;
    frame.style.pointerEvents = 'none';
    // 正在按住运动按钮时先松开并等待停止请求发送，再销毁旧内容页
    try {
      const child = frame.contentWindow;
      if (child.document.body) child.document.body.inert = true;
      const stops = (child.swsMotionStops || []).map((stop) => stop()).filter(Boolean);
      if (stops.length) await Promise.all(stops);
    } catch (e) {}
    if (ticket !== sequence) return;
    if (push && normalize(location.pathname) !== path) history.pushState({}, '', path);
    links.forEach((a) => {
      a.removeAttribute('style');
      if (a.pathname === path) a.setAttribute('aria-current', 'page');
      else a.removeAttribute('aria-current');
    });
    document.getElementById('sws-page-title').textContent = links.find(
      (a) => a.pathname === path,
    ).textContent;
    document.getElementById('sws-page-description').textContent = '';
    frame.title = document.getElementById('sws-page-title').textContent;
    frame.contentWindow.location.replace(path + '?view=1');
    resize();
    timer = setTimeout(() => {
      hint.textContent = )UI" SWS_JS_TEXT(L_UI_SLOW_LOAD) R"UI(;
      resize();
    }, 12000);
  };
  frame.addEventListener('load', () => {
    clearTimeout(timer);
    hint.textContent = '';
    frame.style.pointerEvents = '';
    resize();
  });
  addEventListener('resize', resize);
  new ResizeObserver(resize).observe(document.querySelector('.t'));
  links.forEach((a) =>
    a.addEventListener('click', (e) => {
      if (e.button !== 0 || e.ctrlKey || e.metaKey || e.shiftKey || e.altKey) return;
      e.preventDefault();
      swsNavigate(a.pathname, true);
    }),
  );
  addEventListener('popstate', () => swsNavigate(location.pathname));
  document.documentElement.classList.add('sws-shell');
  swsNavigate(location.pathname);
})();
</script></body></html>
)UI";


PREVIEW_FILE_42


// 网络页使用独立的响应式样式，不加载外部字体和脚本
// 新版通信与固件更新卡片限制在屏幕宽度内，避免手机页面横向滚动
const char html_network_css[] PROGMEM = R"NET(
<style>
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
  accent-color: #8b2020;
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
  background: #8b2020;
  border-color: #8b2020;
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

#nw-message {
  min-height: 1.7em;
  line-height: 1.7;
  overflow-wrap: anywhere;
}
.nw-error {
  color: #c1c1c1;
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


PREVIEW_FILE_43


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
 R"NET(</span><div id="nw-current" class="nw-networks"></div><dl id="nw-access" class="nw-access" hidden><dt>)NET"
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
 R"NET(</button></div></form></section>
</div><p id="nw-message" role="status" aria-live="polite"></p>
)NET";


PREVIEW_FILE_44



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
    scanTries = 0,
    lastNotice = '',
    statusBusy = false,
    current = null,
    networks = [],
    autoScanned = false,
    listKey = '',
    enableDraft = false;
  function message(text, error = false) {
    $('nw-message').textContent = text;
    $('nw-message').className = error ? 'nw-error' : '';
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
    ap.querySelector('[type=submit]').disabled = !loaded || working;
  }
  function lock(value) {
    working = value || submitting;
    sync();
  }
  // 请求设置超时，断网时停止等待，避免不断堆积连接
  async function api(action, params) {
    const controller = new AbortController(),
      timer = setTimeout(() => controller.abort(), 8000);
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
    } finally {
      clearTimeout(timer);
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
    if (statusBusy) return;
    statusBusy = true;
    try {
      const s = await api('status');
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
      if (s.message && s.message !== lastNotice) {
        lastNotice = s.message;
        message(s.message, s.failed);
      }
      lock(s.connecting || s.pending);
      if (!autoScanned && s.sta.enabled && !working) {
        autoScanned = true;
        startScan();
      }
    } catch (e) {
      lastNotice = '';
      message()NET" SWS_JS_TEXT(L_UI_CONNECTION_LOST) R"NET(, true);
    } finally {
      statusBusy = false;
    }
  }
  async function scan() {
    try {
      const r = await api('scan', {});
      if (r.running) {
        if (++scanTries > 30) throw Error()NET" SWS_JS_TEXT(L_UI_SCAN_TIMEOUT) R"NET();
        setTimeout(scan, 1000);
        return;
      }
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
      $('nw-scan-state').textContent = e.message;
    }
    scanning = false;
    sync();
  }
  // 进入已开启 Wi-Fi 的页面和打开开关时自动搜索，也保留手动刷新入口
  function startScan() {
    if (scanning || working || !loaded) return;
    scanning = true;
    scanTries = 0;
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
        message(r.message);
        form.elements.password.value = '';
        if (action === 'connect') {
          enableDraft = false;
          dialog.close();
        }
      } catch (err) {
        if (action === 'connect') $('nw-dialog-error').textContent = err.message;
        else message(err.message, true);
      } finally {
        submitting = false;
        lock(false);
        await status();
      }
    };
  sync();
  status();
  setInterval(status, 2500);
})();
</script>
)NET";


PREVIEW_FILE_45
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
  const char htmL_NETWORKSSID8[] PROGMEM =
  "<br/><hr><form method='post' action='/net.htm'>"
  L_NETWORK_PWD ": <input style='width:8em; text-align:left;' name='otapwd' type='password' minlength='8' maxlength='63'> (" L_NETWORK_PWD_MSG ")<br/>"
  #if DISPLAY_RESET_CONTROLS != OFF
    "<br/><button " SWS_CONFIRM_ATTRIBUTE " onclick=\"return confirm(this.dataset.confirm)\" type='submit' name='otareboot' value='1'>" L_REBOOT_OTA "</button></form>\n"
  #else
    L_NETWORK_EN_OTA ": <input type='checkbox' name='otaen' value='1' %s><br/>"
    "<br/><button type='submit'>" L_UPLOAD "</button></form>\n"
  #endif
  ;
#endif

const char html_logout[] PROGMEM =
"<br/>" L_NETWORK_CONFIG_SECURITY ":"
"<br/><br/><form method='post' action='/net.htm'>"
L_NETWORK_PWD ": <input style='width:8em; text-align:left;' name='webpwd' type='password' minlength='3' maxlength='31'> "
"<button type='submit'>" L_UPLOAD "</button></form>"
"<form method='post' action='/net.htm'>"
"<button type='submit' name='logout' value='1'>" L_LOGOUT "</button></form><br />\n";

const char html_reboot[] PROGMEM =
"<br/><br/><br/><br/><br/><form method='post' action='/net.htm'>"
"<b>" L_NETWORK_RESTART_MSG "</b><br/><br/>"
"<button type='submit'>" L_CONTINUE "</button>"
"</form><br/><br/><br/><br/>"
"\n";

const char html_login[] PROGMEM =
"<br/><form method='post' action='/net.htm'>"
"<br/>" L_NETWORK_TITLE "<br />"
"<input style='width:8em; text-align:left;' name='login' type='password' minlength='3' maxlength='31'>"
"<button type='submit'>" L_OK "</button>"
"</form><br/><br/><br/>"
#if OPERATIONAL_MODE == WIFI
  L_NETWORK_ADVICE1 L_NETWORK_ADVICE2 L_NETWORK_ADVICE3 L_NETWORK_ADVICE4 L_NETWORK_ADVICE5 L_NETWORK_ADVICE6 L_NETWORK_ADVICE7 L_NETWORK_ADVICE8 L_NETWORK_ADVICE9 
  "<br/><br/>"
#endif
"\n";

