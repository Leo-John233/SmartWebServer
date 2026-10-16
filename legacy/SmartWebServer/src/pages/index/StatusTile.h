// -----------------------------------------------------------------------------------
// Controller Status tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

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

const char html_indexSettingsBegin[] PROGMEM =
  "<div class='sws-controller-settings' style='--settings-columns:%d'>";

const char html_indexMountType[] PROGMEM =
  "<form class='sws-mount-type' method='get' action='/index.htm'><label for='sws-mount-type'>" L_MOUNT_TYPE "</label>"
  "<select id='sws-mount-type' name='mountt'><option value='1'%s>" L_MOUNT_GEM "</option>"
  "<option value='2'%s>" L_MOUNT_FORK "</option><option value='3'%s>" L_MOUNT_ALTAZ "</option></select>"
  "<div class='sws-setting-actions'><button type='submit'>" L_UPLOAD "</button>"
  "<button name='revert' value='0' type='submit'>" L_REVERT "</button></div></form>\n";

const char html_resetNotes[] PROGMEM =
  "Notes:<ul>"
  "<li>" L_RESET_MSG1 "</li>"
  "<li>" L_DOWN_MESSAGE1 "</li>"
  "</ul>\n";
