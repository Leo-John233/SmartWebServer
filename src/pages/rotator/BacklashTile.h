// -----------------------------------------------------------------------------------
// Rotator Backlash tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void rotatorBacklashTile(String &data);
extern void rotatorBacklashTileAjax(String &data);
extern void rotatorBacklashTileGet();

const char html_backlashValue[] PROGMEM =
"<div class='sws-accessory-metrics'><div><span>" L_BACKLASH "</span><strong class='c'>%d " L_UNIT_STEPS "</strong></div></div>";

const char html_configBlAxis3[] PROGMEM =
"<label class='sws-accessory-field'><span>" L_BACKLASH_RANGE_AXIS3UP "</span><input value='%d' type='number' name='b3' min='0' max='32767' required></label>";
