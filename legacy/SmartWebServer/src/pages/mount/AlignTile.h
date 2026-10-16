// 控件按用途分组，保留原有参数名称和中文语言资源
// -----------------------------------------------------------------------------------
// Align tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void alignTile(String &data);
extern void alignTileAjax(String &data);
extern void alignTileGet();

const char html_alignCorrection[] PROGMEM =
  "<span id='align_lr' class='c'>%s</span> &nbsp;&amp;&nbsp; <span id='align_ud' class='c'>%s</span> &nbsp;(" L_ALIGN_MESSAGE " %s)<br /><br />";

const char html_alignStart[] PROGMEM =
  "<button id='alg%d' class='bbh' onpointerdown=\"s('al','%d')\" type='button' disabled>%d%s</button>";

const char html_alignAccept[] PROGMEM =
  "&nbsp;&nbsp;&nbsp;<button id='alga' class='bbh' onpointerdown=\"s('al','n')\" type='button' disabled>" L_ACCEPT "</button>";

const char html_alignRefine[] PROGMEM =
  "<p class='sws-help'>" L_REFINE_MESSAGE1 "</p><p class='sws-help'>" L_REFINE_MESSAGE2 "</p><p class='sws-help'>" L_REFINE_MESSAGE3 "</p>"
  "<button id='rpa'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('rp','a')\" type='button'>" L_REFINE_PA "</button>"
  "<br />";
