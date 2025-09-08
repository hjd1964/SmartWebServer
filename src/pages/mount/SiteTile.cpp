// -----------------------------------------------------------------------------------
// Date/Time/Location tile
#include "SiteTile.h"

#include "../KeyValue.h"
#include "../Pages.common.h"

int get_temp_month;
int get_temp_day;
int get_temp_year;
int get_temp_hour;
int get_temp_minute;
int get_temp_second;

// create the related webpage tile
void siteTile(String &data)
{
  char temp[240] = "";
  char reply[80] = "";

  sprintf_P(temp, html_tile_text_beg, "22em", "15em", L_LOCATION_TITLE);
  data.concat(temp);
  data.concat(F("<br/><hr>"));

  data.concat(FPSTR(html_browserTime));
  sprintf_P(temp, html_date, state.dateStr);
  data.concat(temp);
  sprintf_P(temp, html_time, state.timeStr);
  data.concat(temp);
  sprintf_P(temp, html_sidereal, state.lastStr);
  data.concat(temp);
  www.sendContentAndClear(data);
  
  sprintf_P(temp, html_site, state.latitudeStr, state.longitudeStr);
  data.concat(temp);
  data.concat(FPSTR(html_setDateTime));
  data.concat(F("<hr>"));

  sprintf_P(temp, html_collapsable_beg, L_SETTINGS "...");
  data.concat(temp);
  sprintf_P(temp, html_form_begin, "mount.htm");
  data.concat(temp);
  www.sendContentAndClear(data);

  // Latitude
  data.concat(FPSTR(html_latMsg));
  if (!onStep.command(status.getVersionMajor() > 3 ? ":GtH#" : ":Gt#", reply))
    strcpy(reply, status.getVersionMajor() > 3 ? "+00*00:00" : "+00*00");
  reply[3] = 0;
  reply[6] = 0;
  reply[9] = 0;
  if (reply[0] == '+') reply[0] = '0';
  convert.stripNumericStr(reply);
  sprintf_P(temp, html_ninput_wide, "t1", reply, "-90", "90", "&deg;");
  data.concat(temp);
  sprintf_P(temp, html_ninput, "t2", &reply[4], "0", "60", "'");
  data.concat(temp);
  if (status.getVersionMajor() > 3)
  {
    sprintf_P(temp, html_ninput, "t3", &reply[7], "0", "60", "\"");
    data.concat(temp);
  }
  data.concat(F("<br />"));
  www.sendContentAndClear(data);

  // Longitude
  data.concat(FPSTR(html_longMsg));
  if (!onStep.command(status.getVersionMajor() > 3 ? ":GgH#" : ":Gg#", reply))
    strcpy(reply, status.getVersionMajor() > 3 ? "+000*00:00" : "+000*00");
  reply[4] = 0;
  reply[7] = 0;
  reply[10] = 0;
  if (reply[0] == '+') reply[0] = '0';
  convert.stripNumericStr(reply);
  sprintf_P(temp, html_ninput_wide, "g1", reply, "-180", "180", "&deg;");
  data.concat(temp);
  sprintf_P(temp, html_ninput, "g2", &reply[5], "0", "60", "'");
  data.concat(temp);
  if (status.getVersionMajor() > 3)
  {
    sprintf_P(temp, html_ninput, "g3", &reply[8], "0", "60", "\"");
    data.concat(temp);
  }
  data.concat(F("<br />"));
  www.sendContentAndClear(data);

  // UTC Offset
  data.concat(FPSTR(html_offsetMsg));
  if (!onStep.command(":GG#", reply)) strcpy(reply, "+00");
  reply[3] = 0;
  if (reply[0] == '+') reply[0] = '0';
  convert.stripNumericStr(reply);
  sprintf_P(temp, html_ninput_wide, "u1", reply, "-14", "12", "h");
  data.concat(temp);
  sprintf_P(temp, html_offsetMin, reply[4] == '0' ? "selected" : "", reply[4] == '3' ? "selected" : "", reply[4] == '4' ? "selected" : "");
  data.concat(temp);
  data.concat(FPSTR(html_offsetFooterMsg));
  www.sendContentAndClear(data);

  data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button>\n"));
  data.concat(FPSTR(html_form_end));

  data.concat(FPSTR(html_collapsable_end));
  data.concat(FPSTR(html_tile_end));
  www.sendContentAndClear(data);
}

// use Ajax key/value pairs to pass related data to the web client in the background
void siteTileAjax(String &data)
{
  data.concat(keyValueString("date_ut", state.dateStr));
  data.concat(keyValueString("time_ut", state.timeStr));
  data.concat(keyValueString("time_lst", state.lastStr));
  data.concat(keyValueString("site_long", state.longitudeStr));
  data.concat(keyValueString("site_lat", state.latitudeStr));
  data.concat(keyValueString("call", "update_date_time"));
  www.sendContentAndClear(data);
}

// pass related data back to OnStep
extern void siteTileGet()
{
  String v, v1, v2;
  int16_t i, sign;
  char temp[80];

  // Date/Time
  v = www.arg("dm");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 11))
      get_temp_month = i + 1;
  }
  v = www.arg("dd");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 1 && i <= 31))
      get_temp_day = i;
  }
  v = www.arg("dy");
  if (!v.equals(EmptyStr))
  {
    if ((convert.atoi2((char *)v.c_str(), &i)) && (i >= 2016 && i <= 9999))
    {
      get_temp_year = i - 2000;
      sprintf(temp, ":SC%02d/%02d/%02d#", get_temp_month, get_temp_day, get_temp_year);
      onStep.commandBool(temp);
    }
  }
  v = www.arg("th");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 23))
      get_temp_hour = i;
  }
  v = www.arg("tm");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 59))
      get_temp_minute = i;
  }
  v = www.arg("ts");
  if (!v.equals(EmptyStr))
  {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 59))
    {
      get_temp_second = i;
      char temp[20];
      sprintf(temp, ":SL%02d:%02d:%02d#", get_temp_hour, get_temp_minute, get_temp_second);
      onStep.commandBool(temp);
    }
  }

  // Location
  v = www.arg("g1");  // long deg
  v1 = www.arg("g2"); // long min
  if (status.getVersionMajor() > 3) v2 = www.arg("g3"); else v2 = "0"; // long sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.charAt(0) == '-') sign = '-'; else sign = '+';
    int16_t d = abs(v.toInt());
    int16_t m = v1.toInt();
    int16_t s = v2.toInt();
    if (d >= 0 && d <= 180 && m >= 0 && m <= 60 && s >= 0 && s <= 60)
    {
      if (status.getVersionMajor() > 3)
        sprintf(temp, ":Sg%c%03d*%02d:%02d#", sign, d, m, s);
      else
        sprintf(temp, ":Sg%c%03d*%02d#", sign, d, m);
      onStep.commandBool(temp);
    }
  }

  v = www.arg("t1");  // lat deg
  v1 = www.arg("t2"); // lat min
  if (status.getVersionMajor() > 3) v2 = www.arg("t3"); else v2 = "0"; // lat sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.charAt(0) == '-') sign = '-'; else sign = '+';
    int16_t d = abs(v.toInt());
    int16_t m = v1.toInt();
    int16_t s = v2.toInt();
    if (d >= 0 && d <= 90 && m >= 0 && m <= 60 && s >= 0 && s <= 60)
    {
      sprintf(temp, ":St%c%02d*%02d:%02d#", sign, d, m, s);
      onStep.commandBool(temp);
    }
  }

  v = www.arg("u1");  // UT offset hrs
  v1 = www.arg("u2"); // UT offset min
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr)) {
    if (v.charAt(0) == '-') sign = '-'; else sign = '+';
    int16_t h = abs(v.toInt());
    int16_t m = v1.toInt();
    if (v.toInt() >= -14 && h <= 12 && (m == 0 || m == 30 || m == 45))
    {
      sprintf(temp, ":SG%c%02d:%02d#", sign, h, m);
      onStep.commandBool(temp);
    }
  }

}
