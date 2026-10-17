// -----------------------------------------------------------------------------------
// Encoder Sync tile
#include "SyncTile.h"

#include "../../lib/nv/Nv.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#if ENCODERS == ON

// create the related webpage tile
void syncTile(String &data)
{
  data.concat(F("<div class='b1 sws-encoder-sync'><div class='sws-device-card-head'><h2>" L_ENC_SYNC_CONTROL "</h2></div><div class='sws-encoder-sync-grid'><div class='sws-encoder-sync-actions'>"));
  data.concat(FPSTR(html_syncOnStepNow));
  data.concat(FPSTR(html_syncEncodersNow));
  #ifdef ENC_ABSOLUTE
    data.concat(FPSTR(html_zeroEncodersNow));
  #endif
  data.concat(F("</div>"));
  data.concat(FPSTR(html_syncAuto));
  data.concat(F("</div>"));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void syncTileAjax(String &data)
{
  data.concat(keyValueToggleBoolSelected("enc_as_on", "enc_as_off", encoders.settings.autoSync));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void syncTileGet()
{
  String v;

  v = www.arg("enc");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("o2e")) encoders.syncFromOnStep();
    if (v.equals("e2o")) encoders.syncToOnStep();
    #ifdef ENC_ABSOLUTE
      if (v.equals("zro")) encoders.originFromOnStep();
    #endif
    if (v.equals("on")) encoders.settings.autoSync = true;
    if (v.equals("off")) encoders.settings.autoSync = false;
    nv().kv().put("ENCODER_SETTINGS", encoders.settings);
  }
}

#endif
