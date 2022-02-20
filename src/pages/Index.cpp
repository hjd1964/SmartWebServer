// -----------------------------------------------------------------------------------
// The home page, status information

#include "Index.h"
#include "../lib/convert/Convert.h"

void handleRoot() {
  char temp[420] = "";
  char temp1[80] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  sendHtmlStart();

  String data = FPSTR(html_headB);
  data.concat(FPSTR(html_main_cssB));
  data.concat(FPSTR(html_main_css1));
  data.concat(FPSTR(html_main_css2));
  data.concat(FPSTR(html_main_css3));
  data.concat(FPSTR(html_main_css4));
  data.concat(FPSTR(html_main_css5));
  data.concat(FPSTR(html_main_css6));
  sendHtml(data);
  data.concat(FPSTR(html_main_css7));
  data.concat(FPSTR(html_main_css8));
  data.concat(FPSTR(html_main_css_control1));
  data.concat(FPSTR(html_main_css_control2));
  data.concat(FPSTR(html_main_css_control3));
  data.concat(FPSTR(html_main_cssE));
  data.concat(FPSTR(html_headE));
  data.concat(FPSTR(html_bodyB));
  sendHtml(data);

  // finish the standard http response header
  data.concat(FPSTR(html_onstep_header1));
  data.concat("OnStep");
  data.concat(FPSTR(html_onstep_header2));
  data.concat(firmwareVersion.str);
  data.concat(" (OnStep");
  if (status.getVersionStr(temp1)) data.concat(temp1); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatS));
  data.concat(FPSTR(html_linksCtrlN));
  if (status.featureFound) data.concat(FPSTR(html_linksAuxN));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (status.pecEnabled) data.concat(FPSTR(html_linksPecN));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgN));
  data.concat(FPSTR(html_linksSetupN));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!status.valid) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(); return; }

  // active ajax page is: handleRootAjax();
  data.concat("<script>var ajaxPage='index.txt';</script>\n");
  data.concat(FPSTR(html_ajax_active));
  sendHtml(data);

  data.concat("<div style='width: 27em;'>");

  data.concat("<b>" L_SITE ":</b><br />");
  // Browser time
  data.concat(FPSTR(html_settingsBrowserTime));

  // UTC Date
  sprintf_P(temp, html_indexDate, state.dateStr);
  data.concat(temp);

  // UTC Time
  sprintf_P(temp, html_indexTime, state.timeStr);
  data.concat(temp);

  // LST
  sprintf_P(temp, html_indexSidereal, state.lastStr);
  data.concat(temp);

  // Longitude and Latitude
  sprintf_P(temp, html_indexSite, state.longitudeStr, state.latitudeStr);
  data.concat(temp);
  sendHtml(data);

  // Ambient conditions
  #if DISPLAY_WEATHER == ON
    sprintf_P(temp, html_indexTPHD, L_TEMPERATURE ":", 't', state.siteTemperatureStr); data.concat(temp);
    sprintf_P(temp, html_indexTPHD, L_PRESSURE ":", 'p', state.sitePressureStr); data.concat(temp);
    sprintf_P(temp, html_indexTPHD, L_HUMIDITY ":", 'h', state.siteHumidityStr); data.concat(temp);
    sprintf_P(temp, html_indexTPHD, L_DEW_POINT ":", 'd', state.siteDewPointStr); data.concat(temp);
    sendHtml(data);
  #endif

  // Focuser/telescope temperature
  if (status.focuserFound) {
    sprintf_P(temp, html_indexTPHD, L_TELE_TEMPERATURE ":", 'f', state.telescopeTemperatureStr); data.concat(temp);
  }

  data.concat("<br /><b>" L_COORDINATES ":</b><br />");

  // RA,Dec current
  sprintf_P(temp, html_indexPosition, state.indexRaStr, state.indexDecStr); 
  data.concat(temp);

  // RA,Dec target
  sprintf_P(temp, html_indexTarget, state.targetRaStr, state.targetDecStr); 
  data.concat(temp);
  sendHtml(data);

  #if ENCODERS == ON
    // RA,Dec OnStep position
    sprintf_P(temp, html_indexEncoder1, state.angleAxis1Str, state.angleAxis2Str);
    data.concat(temp);

    // RA,Dec encoder position
    sprintf_P(temp, html_indexEncoder2, state.encAngleAxis1Str, state.encAngleAxis2Str);
    data.concat(temp);
    sendHtml(data);
  #endif

  // pier side and meridian flips
  sprintf_P(temp, html_indexPier, state.pierSideStr, state.meridianFlipStr);
  data.concat(temp);
  sendHtml(data);

  // polar align
  if (!isnan(state.latitude) && fabs(state.latitude) <= 89) {
    data.concat("<br /><b>" L_POLAR_ALIGN ":</b><br />");

    strcpy(temp1, L_ZENITH);
    if (status.mountType != MT_ALTAZM) { if (state.latitude < 0) strcpy(temp1, L_SCP); else strcpy(temp1, L_NCP); }

    sprintf_P(temp, html_indexCorPolar, state.alignLrStr, state.alignUdStr, temp1);
    data.concat(temp);
    sendHtml(data);
  }

  data.concat("<br /><b>" L_OPERATIONS ":</b><br />");

  // Park
  sprintf_P(temp, html_indexPark, state.parkStr);
  data.concat(temp);

  // Tracking
  sprintf_P(temp, html_indexTracking, state.trackStr);
  data.concat(temp);

  // Slew speed
  sprintf_P(temp, html_indexMaxSpeed, state.slewSpeedStr);
  data.concat(temp);
  sendHtml(data);

  data.concat("<br /><b>" L_STATE ":</b><br />");

  // Driver status
  int numAxes = 2;
  if (status.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    if (status.driver[axis].valid) {
      sprintf_P(temp, html_indexDriverStatus, axis + 1, axis, state.driverStatusStr[axis]);
      data.concat(temp);
    }
  }

  // MCU Temperature
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    sprintf_P(temp, html_indexTPHD, L_INTERNAL_TEMP ":", 'm', state.controllerTemperatureStr);
    data.concat(temp);
  #endif

  // General Error
  sprintf_P(temp, html_indexGeneralError, state.lastErrorStr);
  data.concat(temp);

  // Loop time
  if (status.getVersionMajor() < 10) {
    sprintf_P(temp, html_indexWorkload, state.workLoadStr);
    data.concat(temp);
  }

  // wifi signal strength
  #if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
    sprintf_P(temp, html_indexSignalStrength, state.signalStrengthStr);
    data.concat(temp);
  #endif

  data.concat("</div><br class=\"clear\" />\r\n");
  data.concat("</div></body></html>");

  sendHtml(data);
  sendHtmlDone();
}

void handleRootAjax() {
  String data = "";
  char temp[120] = "";

  sendTextStart();

  if (status.valid) {

    // UTC Date
    data.concat("date_ut|"); data.concat(state.dateStr); data.concat("\n");

    // UTC Time
    data.concat("time_ut|"); data.concat(state.timeStr); data.concat("\n");

    // LST
    data.concat("time_lst|"); data.concat(state.lastStr); data.concat("\n");

    // Longitude and Latitude
    data.concat("site_long|"); data.concat(state.longitudeStr); data.concat("\n");
    data.concat("site_lat|"); data.concat(state.latitudeStr); data.concat("\n");

    // Ambient conditions
    #if DISPLAY_WEATHER == ON
      data.concat("tphd_t|"); data.concat(state.siteTemperatureStr); data.concat("\n");
      data.concat("tphd_p|"); data.concat(state.sitePressureStr); data.concat("\n");
      data.concat("tphd_h|"); data.concat(state.siteHumidityStr); data.concat("\n");
      data.concat("tphd_d|"); data.concat(state.siteDewPointStr); data.concat("\n");
    #endif

    // Focuser/telescope temperature
    if (status.focuserFound) {
      data.concat("tphd_f|"); data.concat(state.telescopeTemperatureStr); data.concat("\n");
    }

    // RA,Dec current
    data.concat("idx_a1|"); data.concat(state.indexRaStr); data.concat("\n");
    data.concat("idx_a2|"); data.concat(state.indexDecStr); data.concat("\n");

    // RA,Dec target
    data.concat("tgt_a1|"); data.concat(state.targetRaStr); data.concat("\n");
    data.concat("tgt_a2|"); data.concat(state.targetDecStr); data.concat("\n");

    #if ENCODERS == ON
      // RA,Dec OnStep position
      data.concat("raw_a1|"); data.concat(state.angleAxis1Str); data.concat("\n");
      data.concat("raw_a2|"); data.concat(state.angleAxis2Str); data.concat("\n");

      // RA,Dec encoder position
      data.concat("enc_a1|"); data.concat(state.encAngleAxis1Str); data.concat("\n");
      data.concat("enc_a2|"); data.concat(state.encAngleAxis2Str); data.concat("\n");
    #endif

    // pier side and meridian flips
    data.concat("pier_side|"); data.concat(state.pierSideStr); data.concat("\n");
    data.concat("mdn_flip|"); data.concat(state.meridianFlipStr); data.concat("\n");

    // polar align
    if (!isnan(state.latitude) && fabs(state.latitude) <= 89) {
      data.concat("align_lr|"); data.concat(state.alignLrStr); data.concat("\n");
      data.concat("align_ud|"); data.concat(state.alignUdStr); data.concat("\n");
    }

    // Park
    data.concat("park|"); data.concat(state.parkStr); data.concat("\n");

    // Tracking
    data.concat("track|"); data.concat(state.trackStr); data.concat("\n");

    // Slew speed
    data.concat("mr_slew|"); data.concat(state.slewSpeedStr); data.concat("\n");

    // Driver status
    int numAxes = 2;
    if (status.getVersionMajor() >= 10) numAxes = 9;
    for (int axis = 0; axis < numAxes; axis++) {
      if (status.driver[axis].valid) {
        sprintf(temp, "dvr_stat%d|%s", axis, state.driverStatusStr[axis]);
        data.concat(temp); data.concat("\n");
      }
    }

    // MCU Temperature
    #if DISPLAY_INTERNAL_TEMPERATURE == ON
      data.concat("tphd_m|"); data.concat(state.controllerTemperatureStr); data.concat("\n");
    #endif

    // General Error
    data.concat("last_err|"); data.concat(state.lastErrorStr); data.concat("\n");

    // Loop time
    if (status.getVersionMajor() < 10) {
      data.concat("work_load|"); data.concat(state.workLoadStr); data.concat("\n");
    }

    // wifi signal strength
    #if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
      data.concat("signal|"); data.concat(state.signalStrengthStr); data.concat("\n");
    #endif

    // Update web-browser time
    data.concat("call|update_date_time\n");
  }

  sendText(data);
  sendTextDone();
}
