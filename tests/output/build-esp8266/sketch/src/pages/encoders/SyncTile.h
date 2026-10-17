#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\pages\\encoders\\SyncTile.h"
// -----------------------------------------------------------------------------------
// Encoder Sync tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

extern void syncTile(String &data);
extern void syncTileAjax(String &data);
extern void syncTileGet();

const char html_syncOnStepNow[] PROGMEM =
  "<button onpointerdown=\"s('enc','o2e')\" type='button'>" L_ENC_SYNC_TO_ENC "</button>";

#ifdef ENC_ABSOLUTE
  const char html_zeroEncodersNow[] PROGMEM =
    "<button class='sws-secondary sws-encoder-origin'" SWS_CONFIRM_ATTRIBUTE " onpointerdown=\"if (confirm(this.dataset.confirm)) s('enc','zro')\" type='button'>" L_ENC_ORIGIN_TO_ONS "</button>";
#endif

const char html_syncEncodersNow[] PROGMEM =
  "<button onpointerdown=\"s('enc','e2o')\" type='button'>" L_ENC_SYNC_TO_ONS "</button>";

const char html_syncAuto[] PROGMEM =
  "<div class='sws-encoder-auto'><span>" L_ENC_AUTO_SYNC "</span><div class='sws-encoder-toggle'>"
  "<button id='enc_as_on' onpointerdown=\"s('enc','on')\" type='button'>" L_ON "</button>"
  "<button id='enc_as_off' onpointerdown=\"s('enc','off')\" type='button'>" L_OFF "</button>"
  "</div></div>";
