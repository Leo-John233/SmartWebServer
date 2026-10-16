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
  "<button onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) s('rot','reset')\" type='button'>"
  L_RESET_HOME "</button>";
