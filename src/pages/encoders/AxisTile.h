// -----------------------------------------------------------------------------------
// Axis tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

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
