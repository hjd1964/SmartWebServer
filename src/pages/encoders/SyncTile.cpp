// -----------------------------------------------------------------------------------
// Encoder Sync tile
#include "SyncTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

#if ENCODERS == ON

// create the related webpage tile
void syncTile(String &data)
{
  char temp[240] = "";

  sprintf_P(temp, html_tile_beg, "22em", "13em", "Sync Control");
  data.concat(temp);
  data.concat("<br /><hr>");

  sprintf_P(temp, html_form_begin, "enc.htm");
  data.concat(temp);

  data.concat(FPSTR(html_syncOnStepNow));
  data.concat(FPSTR(html_syncEncodersNow));

  #ifdef ENC_ABSOLUTE
    data.concat(FPSTR(html_zeroEncodersNow));
  #endif

  data.concat(FPSTR(html_syncAuto));
  www.sendContentAndClear(data);

  data.concat("<hr>");

  data.concat(html_form_end);

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
    if (v.equals("o2e")) encoders.syncToOnStep();
    if (v.equals("e2o")) encoders.syncFromOnStep(true);
    #ifdef ENC_ABSOLUTE
      if (v.equals("zro")) encoders.zeroFromOnStep();
    #endif
    if (v.equals("on")) encoders.settings.autoSync = true;
    if (v.equals("off")) encoders.settings.autoSync = false;
    nv.updateBytes(NV_ENCODER_SETTINGS_BASE, &encoders.settings, sizeof(EncoderSettings));
  }
}

#endif
