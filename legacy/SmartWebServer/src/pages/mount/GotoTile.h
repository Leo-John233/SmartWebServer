// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Goto tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

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
  "<button id='gto_mfh_off' class='btns_right' onpointerdown=\"s('goto','mh_off')\" type='button'>" L_OFF "</button>"
  "<button id='gto_mfh_visit' class='btns_mid' onpointerdown=\"s('goto','mh_visit')\" type='button'>" L_VISIT "</button>"
  "<button id='gto_mfh_pause' class='btns_left' onpointerdown=\"s('goto','mh_pause')\" type='button'>" L_PAUSE "</button>"
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
"<form id='sws-coordinate-form' class='sws-coordinate-form' data-send='" L_COORD_SEND "' data-ok='" L_COORD_OK "' data-invalid='" L_COORD_INVALID "' data-reject='" L_COORD_REJECT "' data-network='" L_COORD_NETWORK "'>\n<h3>" L_COORD_GOTO "</h3><div class='sws-coordinate-grid'><label class='sws-coordinate-system'><span>" L_COORD_SYSTEM "</span><input value='" L_COORD_JNOW "' readonly></label><div class='sws-coordinate-actions'><button type='submit' name='coord_action' value='go'>" L_GOTO_ACTION "</button><button class='sws-secondary' type='submit' name='coord_action' value='sync'>" L_COORD_SYNC "</button></div><fieldset><legend>" L_COORD_RA "</legend><div class='sws-coordinate-units'><label class='sws-coordinate-unit'><input name='ra_h' type='number' min='0' max='23' step='1' inputmode='numeric' required aria-label='" L_COORD_H "'><span>" L_COORD_H "</span></label><label class='sws-coordinate-unit'><input name='ra_m' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_M "'><span>" L_COORD_M "</span></label><label class='sws-coordinate-unit'><input name='ra_s' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_S "'><span>" L_COORD_S "</span></label></div></fieldset><fieldset><legend>" L_COORD_DEC "</legend><div class='sws-coordinate-units'><select name='dec_sign' aria-label='" L_COORD_SIGN "'><option value='+'>+</option><option value='-'>−</option></select><label class='sws-coordinate-unit'><input name='dec_d' type='number' min='0' max='90' step='1' inputmode='numeric' required aria-label='" L_COORD_D "'><span>" L_COORD_D "</span></label><label class='sws-coordinate-unit'><input name='dec_m' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_M "'><span>" L_COORD_M "</span></label><label class='sws-coordinate-unit'><input name='dec_s' type='number' min='0' max='59' step='1' inputmode='numeric' required aria-label='" L_COORD_S "'><span>" L_COORD_S "</span></label></div></fieldset></div><p role='status' class='sws-coordinate-message'></p></form>";

extern void gotoCoordinatesGet();
