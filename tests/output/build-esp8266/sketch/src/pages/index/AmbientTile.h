#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\index\\AmbientTile.h"
// -----------------------------------------------------------------------------------
// Ambient tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

#if DISPLAY_WEATHER == ON

extern void ambientTile(String &data);
extern void ambientTileAjax(String &data);
extern void ambientTileGet();

const char html_indexTPHD[] PROGMEM = "<div class='sws-metric'><span>%s</span><strong id='tphd_%c' class='c'>%s</strong></div>";

#endif
