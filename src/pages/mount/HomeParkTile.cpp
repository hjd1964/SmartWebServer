// -----------------------------------------------------------------------------------
// Home/Park tile
#include "HomeParkTile.h"

#include "../../lib/convert/Convert.h"
#include "../KeyValue.h"

const char *getHomeParkStateStr();

// create the related webpage tile
void homeParkTile(String &data)
{
  char temp[240] = "";

  sprintf_P(temp, html_tile_beg, "22em", "15em", "Homing and Parking");
  data.concat(temp);
  data.concat(F("<div style='float: right; text-align: right;' id='hp_sta' class='c'>"));
  data.concat(getHomeParkStateStr());
  data.concat(F("</div><br /><hr>"));

  data.concat(FPSTR(html_homeReturn));
  data.concat(FPSTR(html_homeReset));
  data.concat(F("<br />"));
  data.concat(FPSTR(html_unpark));
  data.concat(FPSTR(html_park));
  data.concat(F("<hr>"));

  sprintf_P(temp, html_collapsable_beg, L_CONTROLS "...");
  data.concat(temp);

  data.concat(F("Set park position at current coordinates:<br /><br />"));

  data.concat(FPSTR(html_setpark));

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);

}

// use Ajax key/value pairs to pass related data to the web client in the background
void homeParkTileAjax(String &data)
{
  data.concat(keyValueString("hp_sta", getHomeParkStateStr()));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
void homeParkTileGet()
{
  String v;

  v = www.arg("pk");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("p"))
      onStep.commandBool(":hP#"); // park
    if (v.equals("u"))
      onStep.commandBool(":hR#"); // un-park
    if (v.equals("s"))
      onStep.commandBool(":hQ#"); // set-park
  }

  v = www.arg("qb");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("hf"))
      onStep.commandBlind(":hF#"); // home, reset
    if (v.equals("hc"))
      onStep.commandBlind(":hC#"); // home, find
  }

  v = www.arg("dr");
  if (!v.equals(EmptyStr))
  {
    if (v.equals("qr"))
      onStep.commandBlind(":hF#"); // home, reset
    if (v.equals("qh"))
      onStep.commandBlind(":hC#"); // home, find
    if (v.equals("pk"))
      onStep.commandBool(":hP#"); // park
    if (v.equals("pu"))
      onStep.commandBool(":hR#"); // un-park
  }
}

const char *getHomeParkStateStr()
{
  static char hsta[32] = L_INACTIVE;
  if (status.parking) strcpy(hsta, L_PARKING); else
  if (status.homing) strcpy(hsta, L_HOMING); else
  if (status.parkFail) strcpy(hsta, L_PARK_FAILED); else
  if (status.parked) strcpy(hsta, L_PARKED); else
  if (status.atHome) strcpy(hsta, L_AT_HOME);
  return hsta;
}
