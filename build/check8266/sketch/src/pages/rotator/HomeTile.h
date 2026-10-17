#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\rotator\\HomeTile.h"
// -----------------------------------------------------------------------------------
// Rotator Home tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void rotatorHomeTile(String &data);
extern void rotatorHomeTileAjax(String &data);
extern void rotatorHomeTileGet();

const char html_rotatorHome[] PROGMEM =
  "<button onpointerdown=\"s('rot','home')\" type='button'>"
  L_RETURN_HOME "</button>"
  "<button" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('rot','reset')\" type='button'>"
  L_RESET_HOME "</button>";
