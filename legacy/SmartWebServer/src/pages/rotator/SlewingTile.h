// -----------------------------------------------------------------------------------
// Rotator Goto tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

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
"<div class='sws-accessory-live-rate'><span>" L_CURRENT_MAXRATE "</span><strong id='rot_rate' class='c'>%s</strong></div>";

const char html_rotateGotoSelect[] PROGMEM =
"<div class='sws-choice-source' data-label='" L_SET_LABEL " " L_RATE "'>"
  "<button id='rot_rate_vf' class='btns_right' onpointerdown=\"s('rot','vf')\" type='button'>" L_VFAST "</button>"
  "<button id='rot_rate_f' class='btns_mid' onpointerdown=\"s('rot','f')\"  type='button'>" L_FAST "</button>"
  "<button id='rot_rate_n' class='btns_mid' onpointerdown=\"s('rot','n')\"  type='button'>" L_NORMAL "</button>"
  "<button id='rot_rate_s' class='btns_mid' onpointerdown=\"s('rot','s')\"  type='button'>" L_SLOW "</button>"
  "<button id='rot_rate_vs' class='btns_left' onpointerdown=\"s('rot','vs')\" type='button'>" L_VSLOW "</button>"
  "</div>\n";
