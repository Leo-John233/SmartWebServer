#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\focuser\\BacklashTcfTile.h"
// -----------------------------------------------------------------------------------
// Focuser Backlash tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

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
