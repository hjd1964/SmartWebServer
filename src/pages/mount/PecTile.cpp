// -----------------------------------------------------------------------------------
// PEC tile
#include "PecTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

// create the related webpage tile
void pecTile(String &data)
{
  char temp[240] = "";

  sprintf_P(temp, html_tile_beg, "22em", "15em", L_PAGE_PEC);
  data.concat(temp);
  data.concat(F("<br /><hr>"));

  if (status.pecEnabled) {
    data.concat(FPSTR(html_pecStatus));
  }

  data.concat(F("<br /><hr>"));

  // display pec controls
  sprintf_P(temp, html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  if (status.pecEnabled) {
    data.concat(FPSTR(html_pecControls1));
    data.concat(FPSTR(html_pecControls2));
    data.concat(FPSTR(html_pecControls3));
    data.concat(FPSTR(html_pecControls4));
  } else {
    data.concat(L_DISABLED_MESSAGE);
  }

  data.concat(FPSTR(html_collapsable_end));
  www.sendContentAndClear(data);

  // display steps per worm rotation
  data.concat(F("<div style='margin-top: 0.5em';></div>"));
  sprintf_P(temp, html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);

  char response[80];
  if (status.pecEnabled && onStep.command(":GXE7#", response)) {
    sprintf_P(temp, html_form_begin, "mount.htm");
    data.concat(temp);

    long value = strtol(response, NULL, 10);
    sprintf_P(temp, html_configAxisSpwr, value, 0, 129600000L);
    data.concat(temp);
    data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button><br />\n"));

    data.concat(FPSTR(html_form_end));
  } else {
    data.concat(L_DISABLED_MESSAGE);
  }
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_collapsable_end));
  www.sendContentAndClear(data);

  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void pecTileAjax(String &data)
{
  char temp[80] = "";

  data.concat(F("pec_sta|"));
  if (status.mountType != MT_ALTAZM && onStep.command(":$QZ?#", temp)) {
    if (temp[0] == 'I') data.concat(L_PEC_IDLE); else
    if (temp[0] == 'p') data.concat(L_PEC_WAIT_PLAY); else
    if (temp[0] == 'P') data.concat(L_PEC_PLAYING); else
    if (temp[0] == 'r') data.concat(L_PEC_WAIT_REC); else
    if (temp[0] == 'R') data.concat(L_PEC_RECORDING); else data.concat(L_PEC_UNK);
    if (status.pecRecording) data.concat(F(" (" L_PEC_EEWRITING ")"));
  } else { data.concat("?"); }
  data.concat("\n");

  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void pecTileGet()
{
  String v;
  char temp[40];

  v = www.arg("pec");
  if (!v.equals(EmptyStr)) {
    if (v.equals("play")) onStep.commandBlind(":$QZ+#"); // play
    if (v.equals("stop")) onStep.commandBlind(":$QZ-#"); // stop
    if (v.equals("clr")) onStep.commandBlind(":$QZZ#");  // clear
    if (v.equals("rec")) onStep.commandBlind(":$QZ/#");  // record
    if (v.equals("wrt")) onStep.commandBlind(":$QZ!#");  // write to eeprom
  }

  v = www.arg("spwr");
  if (!v.equals(EmptyStr)) {
    sprintf(temp, ":SXE7,%s#", v.c_str());
    onStep.commandBool(temp);
  }
}
