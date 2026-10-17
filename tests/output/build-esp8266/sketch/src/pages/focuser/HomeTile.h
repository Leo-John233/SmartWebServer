#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\focuser\\HomeTile.h"
// -----------------------------------------------------------------------------------
// Focuser Home tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void focuserHomeTile(String &data);
extern void focuserHomeTileAjax(String &data);
extern void focuserHomeTileGet();

const char html_focuserHome[] PROGMEM =
  "<button onpointerdown=\"gf('Fh')\" type='button'>"
  L_RETURN_HOME "</button>"
  "<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) gf('FH')\" type='button'>"
  L_RESET_HOME "</button>";
