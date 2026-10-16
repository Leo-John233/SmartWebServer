#pragma once

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
