// -----------------------------------------------------------------------------------
// The home page, status information

#include <limits.h>
#include "Index.h"
#include "../lib/convert/Convert.h"

void handleRoot() {
  char temp[420] = "";
  char temp1[180] = "";
  char temp2[180] = "";
  char temp3[180] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  mountStatus.update(true);

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
  data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
  data.concat(FPSTR(html_onstep_header2)); data.concat(firmwareVersion.str);
  data.concat(" (OnStep");
  if (mountStatus.getVersionStr(temp1)) data.concat(temp1); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatS));
  data.concat(FPSTR(html_linksCtrlN));
  if (mountStatus.featureFound()) data.concat(FPSTR(html_linksAuxN));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (mountStatus.pecEnabled()) data.concat(FPSTR(html_linksPecN));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgN));
  data.concat(FPSTR(html_linksSetupN));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(); return; }

  // active ajax page is: handleRootAjax();
  data.concat("<script>var ajaxPage='index.txt';</script>\n");
  data.concat(FPSTR(html_ajax_active));
  sendHtml(data);

  data.concat("<div style='width: 27em;'>");

  data.concat("<b>" L_SITE ":</b><br />");
  // Browser time
  data.concat(FPSTR(html_settingsBrowserTime));

  // UTC Date
  if (!onStep.command(":GX81#", temp1)) strcpy(temp1, "?");
  stripNum(temp1);
  sprintf_P(temp, html_indexDate, "?");
  data.concat(temp);

  // UTC Time
  if (!onStep.command(":GX80#", temp1)) strcpy(temp1, "?");
  sprintf_P(temp, html_indexTime, "?");
  data.concat(temp);

  // LST
  if (!onStep.command(":GS#", temp1)) strcpy(temp1, "?");
  sprintf_P(temp, html_indexSidereal, "?");
  data.concat(temp);

  // Longitude and Latitude
  sprintf_P(temp, html_indexSite, "?", "?");
  data.concat(temp);
  sendHtml(data);

  // Ambient conditions
  #if DISPLAY_WEATHER == ON
    sprintf_P(temp, html_indexTPHD, L_TEMPERATURE ":", 't', "?"); data.concat(temp);
    sprintf_P(temp, html_indexTPHD, L_PRESSURE ":", 'p', "?"); data.concat(temp);
    sprintf_P(temp, html_indexTPHD, L_HUMIDITY ":", 'h', "?"); data.concat(temp);
    sprintf_P(temp, html_indexTPHD, L_DEW_POINT ":", 'd', "?"); data.concat(temp);
    sendHtml(data);
  #endif

  // Focuser/telescope temperature
  if (mountStatus.focuserPresent()) {
    sprintf_P(temp, html_indexTPHD, L_TELE_TEMPERATURE ":", 'f', "?"); data.concat(temp);
  }

  data.concat("<br /><b>" L_COORDINATES ":</b><br />");

  #if DISPLAY_HIGH_PRECISION_COORDS == ON
    // RA,Dec current
    sprintf_P(temp, html_indexPosition, "?", "?"); 
    data.concat(temp);

    // RA,Dec target
    sprintf_P(temp, html_indexTarget, "?", "?"); 
    data.concat(temp);
  #else
    // RA,Dec current
    sprintf_P(temp, html_indexPosition, "?", "?"); 
    data.concat(temp);

    // RA,Dec target
    sprintf_P(temp, html_indexTarget, "?", "?"); 
    data.concat(temp);
  #endif
  sendHtml(data);

  #if ENCODERS == ON
    // RA,Dec OnStep position
    sprintf_P(temp, html_indexEncoder1, "?", "?");
    data.concat(temp);

    // RA,Dec encoder position
    sprintf_P(temp, html_indexEncoder2, "?", "?");
    data.concat(temp);
    sendHtml(data);
  #endif

  // pier side and meridian flips
  sprintf_P(temp, html_indexPier, "?", "?");
  data.concat(temp);
  sendHtml(data);

  // polar align

  // keep numeric latitude for later, in degrees
  long lat = LONG_MIN;
  if (onStep.command(":Gt#", temp1)) {
    temp1[3] = 0;
    if (temp1[0] == '+') temp1[0] = '0';
    lat = atol(temp1);
  }

  if (lat != LONG_MIN && abs(lat) <= 89) {
    data.concat("<br /><b>" L_POLAR_ALIGN ":</b><br />");

    strcpy(temp1, L_ZENITH);
    if (mountStatus.mountType() != MT_ALTAZM) { if (lat < 0) strcpy(temp1, L_SCP); else strcpy(temp1, L_NCP); }

    sprintf_P(temp, html_indexCorPolar, "?", "?", temp1);
    data.concat(temp);
    sendHtml(data);
  }

  data.concat("<br /><b>" L_OPERATIONS ":</b><br />");

  // Park
  sprintf_P(temp, html_indexPark, "?");
  data.concat(temp);

  // Tracking
  sprintf_P(temp, html_indexTracking, "?", "?");
  data.concat(temp);
  sendHtml(data);

  // Slew speed
  if ((onStep.command(":GX97#", temp1)) && (strlen(temp1) > 2)) {
    sprintf_P(temp, html_indexMaxSpeed, "?");
    data.concat(temp);
  } else {
    // fall back to MaxRate display if not supported
    if ((onStep.command(":GX92#", temp1)) && (onStep.command(":GX93#", temp2))) { 
      sprintf_P(temp, html_indexMaxRate, "?", "?");
    } else sprintf_P(temp, html_indexMaxSpeed, "?");
    data.concat(temp);
  }
  sendHtml(data);

  data.concat("<br /><b>" L_STATE ":</b><br />");

  // Driver status
  int numAxes = 2;
  if (mountStatus.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    if (mountStatus.driver[axis].valid) {
      sprintf_P(temp, html_indexDriverStatus, axis + 1, axis, "?");
      data.concat(temp);
    }
  }

  // MCU Temperature
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    sprintf_P(temp, html_indexTPHD, L_INTERNAL_TEMP ":", 'm', "?");
    data.concat(temp);
  #endif

  // General Error
  sprintf_P(temp, html_indexGeneralError, "?");
  data.concat(temp);

  // Loop time
  if (mountStatus.getVersionMajor() < 10) {
    sprintf_P(temp, html_indexWorkload, "?");
    data.concat(temp);
  }

  // wifi signal strength
  #if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
    sprintf_P(temp, html_indexSignalStrength, "?");
    data.concat(temp);
  #endif

  data.concat("</div><br class=\"clear\" />\r\n");
  data.concat("</div></body></html>");

  sendHtml(data);
  sendHtmlDone();
}

void handleRootAjax() {
  String data = "";
  char temp[120] = "", temp1[120] = "", temp2[120] = "";
  long lat = LONG_MIN;

  mountStatus.update();
  if (mountStatus.valid()) {

    // Longitude and Latitude
    if (mountStatus.getVersionMajor() > 3) {
      if (!onStep.command(":GgH#", temp1)) strcpy(temp1, "?");
      temp1[10] = 0;
      if (!onStep.command(":GtH#", temp2)) strcpy(temp2, "?");
      temp2[9] = 0;
    } else {
      if (!onStep.command(":Gg#", temp1)) strcpy(temp1, "?");
      if (!onStep.command(":Gt#", temp2)) strcpy(temp2, "?");
    }
    data.concat("site_long|"); data.concat(temp1); data.concat("\n");
    data.concat("site_lat|"); data.concat(temp2); data.concat("\n");
    temp2[3] = 0;
    if (temp2[0] == '+') temp2[0] = '0';
    lat = atol(temp2);

    // Ambient conditions
    #if DISPLAY_WEATHER == ON
      if (!onStep.command(":GX9A#", temp1)) strcpy(temp1, "?"); else localeTemperature(temp1);
      data.concat("tphd_t|"); data.concat(temp1); data.concat("\n");
      if (!onStep.command(":GX9B#", temp1)) strcpy(temp1, "?"); else localePressure(temp1);
      data.concat("tphd_p|"); data.concat(temp1); data.concat("\n");
      if (!onStep.command(":GX9C#", temp1)) strcpy(temp1, "?"); else localeHumidity(temp1);
      data.concat("tphd_h|"); data.concat(temp1); data.concat("\n");
      if (!onStep.command(":GX9E#", temp1)) strcpy(temp1, "?"); else localeTemperature(temp1);
      data.concat("tphd_d|"); data.concat(temp1); data.concat("\n");
    #endif

    // Focuser/telescope temperature
    if (mountStatus.focuserPresent()) {
      if (!onStep.command(":Ft#", temp1)) strcpy(temp1, "?"); else localeTemperature(temp1);
      data.concat("tphd_f|"); data.concat(temp1); data.concat("\n");
    }

    #if DISPLAY_HIGH_PRECISION_COORDS == ON
      // RA,Dec current
      if (!onStep.command(":GRa#", temp1)) strcpy(temp1, "?");
      if (!onStep.command(":GDe#", temp2)) strcpy(temp2, "?");
      data.concat("idx_a1|"); data.concat(temp1); data.concat("\n");
      data.concat("idx_a2|"); data.concat(temp2); data.concat("\n");

      // RA,Dec target
      if (!onStep.command(":Gra#", temp1)) strcpy(temp1, "?");
      if (!onStep.command(":Gde#", temp2)) strcpy(temp2, "?");
      data.concat("tgt_a1|"); data.concat(temp1); data.concat("\n");
      data.concat("tgt_a2|"); data.concat(temp2); data.concat("\n");
    #else
      // RA,Dec current
      if (!onStep.command(":GR#", temp1)) strcpy(temp1, "?");
      if (!onStep.command(":GD#", temp2)) strcpy(temp2, "?");
      data.concat("idx_a1|"); data.concat(temp1); data.concat("\n");
      data.concat("idx_a2|"); data.concat(temp2); data.concat("\n");

      // RA,Dec target
      if (!onStep.command(":Gr#", temp1)) strcpy(temp1, "?");
      if (!onStep.command(":Gd#", temp2)) strcpy(temp2, "?");
      data.concat("tgt_a1|"); data.concat(temp1); data.concat("\n");
      data.concat("tgt_a2|"); data.concat(temp2); data.concat("\n");
    #endif

    #if ENCODERS == ON
      // RA,Dec OnStep position
      double f;
      f=encoders.getOnStepAxis1(); convert.doubleToDms(temp1, f, true, true, PM_HIGH);
      f=encoders.getOnStepAxis2(); convert.doubleToDms(temp2, f, true, true, PM_HIGH);
      data.concat("raw_a1|"); data.concat(temp1); data.concat("\n");
      data.concat("raw_a2|"); data.concat(temp2); data.concat("\n");

      // RA,Dec encoder position
      if (encoders.validAxis1()) { f=encoders.getAxis1(); convert.doubleToDms(temp1, f, true, true, PM_HIGH); } else strcpy(temp1," ** " L_FAULT " ** ");
      if (encoders.validAxis2()) { f=encoders.getAxis2(); convert.doubleToDms(temp2, f, true, true, PM_HIGH); } else strcpy(temp2," ** " L_FAULT " ** ");
      data.concat("enc_a1|"); data.concat(temp1); data.concat("\n");
      data.concat("enc_a2|"); data.concat(temp2); data.concat("\n");
    #endif

    // pier side and meridian flips
    if ((mountStatus.pierSide() == PierSideFlipWE1) || (mountStatus.pierSide() == PierSideFlipWE2) || (mountStatus.pierSide() == PierSideFlipWE3)) strcpy(temp1, L_MERIDIAN_FLIP_W_TO_E); else
    if ((mountStatus.pierSide() == PierSideFlipEW1) || (mountStatus.pierSide() == PierSideFlipEW2) || (mountStatus.pierSide() == PierSideFlipEW3)) strcpy(temp1, L_MERIDIAN_FLIP_E_TO_W); else
    if (mountStatus.pierSide() == PierSideWest) strcpy(temp1, L_WEST); else
    if (mountStatus.pierSide() == PierSideEast) strcpy(temp1, L_EAST); else
    if (mountStatus.pierSide() == PierSideNone) strcpy(temp1, L_NONE); else strcpy(temp1,L_UNKNOWN);
    if (!mountStatus.valid()) strcpy(temp1, "?");
    if (mountStatus.meridianFlips()) {
      strcpy(temp2, "On");
      if (mountStatus.autoMeridianFlips()) strcat(temp2, ", " L_AUTO);
    } else strcpy(temp2, "Off");
    if (!mountStatus.valid()) strcpy(temp2, "?");
    data.concat("pier_side|"); data.concat(temp1); data.concat("\n");
    data.concat("mdn_flip|"); data.concat(temp2); data.concat("\n");

    // polar align
    if (lat != LONG_MIN && abs(lat) <= 89) {
      long ud = LONG_MIN; if (onStep.command(":GX02#", temp1)) { ud = strtol(&temp1[0], NULL, 10); if (lat < 0) ud = -ud; }
      long lr = LONG_MIN; if (onStep.command(":GX03#", temp1)) { lr = strtol(&temp1[0], NULL, 10); lr = lr/cos(lat/57.295); }

      if (ud != LONG_MIN && lr != LONG_MIN) {
        char units = '"';
        if (abs(ud) >= 300 || abs(lr) >= 300) { 
          ud = ud/60.0; lr = lr/60.0;
          units = '\'';
        }

        char lr_s[2];
        if (lr >= 0) strcpy(lr_s, leftTri); else strcpy(lr_s, rightTri);
        char ud_s[2];
        if (ud >= 0) strcpy(ud_s, upTri); else strcpy(ud_s, downTri);

        sprintf_P(temp1, "%s %ld%c", lr_s, (long)(abs(lr)), units);
        sprintf_P(temp2, "%s %ld%c", ud_s, (long)(abs(ud)), units);

        data.concat("align_lr|"); data.concat(temp1); data.concat("\n");
        data.concat("align_ud|"); data.concat(temp2); data.concat("\n");
      }
    }

    // Park
    if (mountStatus.parked()) strcpy(temp1, L_PARKED); else strcpy(temp1, L_NOT_PARKED);
    if (mountStatus.parking()) strcpy(temp1, L_PARKING); else
    if (mountStatus.parkFail()) strcpy(temp1, L_PARK_FAILED);
    if (mountStatus.atHome()) strcat(temp1, " (" L_AT_HOME ")");
    if (!mountStatus.valid()) strcpy(temp1, "?");
    data.concat("park|"); data.concat(temp1); data.concat("\n");

    // Tracking
    if (mountStatus.tracking()) {
      if (onStep.command(":GT#", temp1)) {
        double tr = atof(temp1);
        dtostrf(tr, 5, 3, temp1);
      } else strcpy(temp1, "?");
    } else strcpy(temp1, L_OFF);
    if (mountStatus.inGoto()) strcpy(temp1, L_INGOTO);
    if (!mountStatus.valid()) strcpy(temp1, "?");
    
    strcpy(temp2, "(");
    if (mountStatus.ppsSync()) strcat(temp2, L_PPS_SYNC ", ");
    if (mountStatus.rateCompensation() == RC_REFR_RA)   strcat(temp2, L_REFR_COMP_RA ", ");
    if (mountStatus.rateCompensation() == RC_REFR_BOTH) strcat(temp2, L_REFR_COMP_BOTH ", ");
    if (mountStatus.rateCompensation() == RC_FULL_RA)   strcat(temp2, L_FULL_COMP_RA ", ");
    if (mountStatus.rateCompensation() == RC_FULL_BOTH) strcat(temp2, L_FULL_COMP_BOTH ", ");
    if (!mountStatus.valid()) strcpy(temp2, "?");
    if (temp2[strlen(temp2) - 2] == ',') { temp2[strlen(temp2) - 2] = 0; strcat(temp2, ")"); } else strcpy(temp2, "");
    data.concat("track|"); data.concat(temp1); data.concat(" "); data.concat(temp2); data.concat("\n");

    // Slew speed
    if ((onStep.command(":GX97#", temp1)) && (strlen(temp1)>2)) {
      data.concat("mr_slew|"); data.concat(temp1); data.concat("\n");
    } else {
      // fall back to MaxRate display if not supported
      if ((onStep.command(":GX92#", temp1)) && (onStep.command(":GX93#", temp2))) { 
        long maxRate = strtol(&temp1[0], NULL, 10);
        long MaxRate = strtol(&temp2[0], NULL, 10);
        sprintf_P(temp1, "%ld", maxRate);
        sprintf_P(temp2, "%ld", MaxRate);
        data.concat("mr_current|"); data.concat(temp1); data.concat("\n");
        data.concat("mr_slew|"); data.concat(temp2); data.concat("\n");
      } else {
        data.concat("mr_slew|"); data.concat("?"); data.concat("\n");
      }
    }

    // Driver status
    int numAxes = 2;
    if (mountStatus.getVersionMajor() >= 10) numAxes = 9;
    for (int axis = 0; axis < numAxes; axis++) {
      if (mountStatus.driver[axis].valid) {
        strcpy(temp1, "");
        if (mountStatus.driver[axis].fault) strcat(temp1, L_DRIVER_FAULT "  ");
        if (mountStatus.driver[axis].communicationFailure) strcat(temp1, L_COMMS_FAILURE ", ");
        if (!mountStatus.driver[axis].communicationFailure) {
          if (mountStatus.driver[axis].standstill) strcat(temp1, L_STANDSTILL ", "); else {
            if (mountStatus.driver[axis].outputA.openLoad || mountStatus.driver[axis].outputB.openLoad) {
              strcat(temp1, L_OPEN_LOAD " ");
              if (mountStatus.driver[axis].outputA.openLoad) strcat(temp1,"A");
              if (mountStatus.driver[axis].outputB.openLoad) strcat(temp1,"B");
              strcat(temp1,", ");
            }
          }
          if (mountStatus.driver[axis].outputA.shortToGround || mountStatus.driver[axis].outputB.shortToGround) {
            strcat(temp1, L_SHORT_GND " ");
            if (mountStatus.driver[axis].outputA.shortToGround) strcat(temp1,"A");
            if (mountStatus.driver[axis].outputB.shortToGround) strcat(temp1,"B");
            strcat(temp1,", ");
          }
          if (mountStatus.driver[axis].overTemperature) strcat(temp1, L_SHUTDOWN_OVER " 150C, ");
          if (mountStatus.driver[axis].overTemperaturePreWarning) strcat(temp1, L_PRE_WARNING " &gt;120C, ");
        }
        if (strlen(temp1) > 2) temp1[strlen(temp1) - 2] = 0;
        if (strlen(temp1) == 0) strcpy(temp1, "Ok");
        sprintf(temp, "dvr_stat%d|", axis);
        data.concat(temp); data.concat("temp1"); data.concat("\n");
      }
    }

    // MCU Temperature
    #if DISPLAY_INTERNAL_TEMPERATURE == ON
      if (!onStep.command(":GX9F#", temp1)) strcpy(temp1, "?"); else localeTemperature(temp1);
      data.concat("tphd_m|"); data.concat(temp1); data.concat("\n");
    #endif

    // General Error
    strcpy(temp1, "");
    if (mountStatus.lastError() != ERR_NONE) strcat(temp1, "<font class=\"y\">"); 
    mountStatus.getLastErrorMessage(temp2);
    if (!mountStatus.valid()) strcat(temp1, "?"); else strcat(temp1, temp2);
    if (mountStatus.lastError() != ERR_NONE) strcat(temp1, "</font>"); 
    data.concat("last_err|"); data.concat(temp1); data.concat("\n");

    // Loop time
    if (mountStatus.getVersionMajor() < 10) {
      if (!onStep.command(":GXFA#", temp1)) strcpy(temp1, "?%");
      data.concat("work_load|"); data.concat(temp1); data.concat("\n");
    }

    // wifi signal strength
    #if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
      long signal_strength_dbm = WiFi.RSSI();
      long signal_strength_qty = 2*(signal_strength_dbm + 100);
      if (signal_strength_qty > 100) signal_strength_qty = 100; 
      else if (signal_strength_qty < 0) signal_strength_qty = 0;
      sprintf(temp1,"%lddBm (%ld%%)", signal_strength_dbm, signal_strength_qty);
      data.concat("signal|"); data.concat(temp1); data.concat("\n");
    #endif

    // UTC Date
    if (!onStep.command(":GX81#", temp)) strcpy(temp, "?");
    stripNum(temp);
    data.concat("date_ut|"); data.concat(temp); data.concat("\n");

    // UTC Time
    if (!onStep.command(":GX80#", temp)) strcpy(temp, "?");
    data.concat("time_ut|"); data.concat(temp); data.concat("\n");

    // LST
    if (!onStep.command(":GS#", temp)) strcpy(temp, "?");
    data.concat("time_lst|"); data.concat(temp); data.concat("\n");

    // Update web-browser time
    data.concat("call|update_date_time\n");
  }

  sendHtml(data);
  sendHtmlDone();
}
