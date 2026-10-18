// -----------------------------------------------------------------------------------
// Tracking tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void trackingTile(String &data);
extern void trackingSettingsTile(String &data);
extern void trackingTileAjax(String &data);
extern void trackingTileGet();

const char html_trackingEnable[] PROGMEM = 
"<button id='trk_on' hidden data-label='" L_DEVICE_TRACKING "' class='btns_right' onpointerdown=\"s('dr','T1')\" type='button'>" L_ON "</button>"
"<button id='trk_off' hidden class='btns_left' onpointerdown=\"s('dr','T0')\" type='button'>" L_OFF "</button>";

const char html_trackingPreset[] PROGMEM =
"<label class='sws-choice'><span>" L_TRACKING_RATE "</span><select class='sws-choice-select' aria-label='" L_TRACKING_RATE "' disabled>"
"<option value='0'>" L_HINT_RATE_SIDEREAL "</option>"
"<option value='1'>" L_HINT_RATE_LUNAR "</option>"
"<option value='2'>" L_HINT_RATE_SOLAR "</option>"
"<option value='3'>" L_HINT_RATE_KING "</option>"
"</select><span class='sws-choice-source sws-choice-originals'>"
"<button id='trk_sid' class='bbh' style='width: 2em' onpointerdown=\"g('Ts')\" title='" L_HINT_RATE_SIDEREAL "' type='button'>" SIDEREAL_CH "</button>"
"<button id='trk_lun' class='bbh' style='width: 2em' onpointerdown=\"g('Tl')\" title='" L_HINT_RATE_LUNAR "' type='button'>" LUNAR_CH "</button>"
"<button id='trk_sol' class='bbh' style='width: 2em' onpointerdown=\"g('Th')\" title='" L_HINT_RATE_SOLAR "' type='button'>" SOLAR_CH "</button>"
"<button id='trk_king' class='bbh' style='width: 2em' onpointerdown=\"g('Tk')\" title='" L_HINT_RATE_KING "' type='button'>" KING_CH "</button>"
"</span></label>";

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
