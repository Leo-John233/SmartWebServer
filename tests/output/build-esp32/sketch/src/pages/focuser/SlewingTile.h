#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\focuser\\SlewingTile.h"
// -----------------------------------------------------------------------------------
// Focuser Slewing tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

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
