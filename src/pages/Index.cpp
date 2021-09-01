// -----------------------------------------------------------------------------------
// The home page, status information

#include <limits.h>
#include "Index.h"

#if OPERATIONAL_MODE != WIFI
void handleRoot(EthernetClient *client) {
#else
void handleRoot() {
#endif
  char temp[400] = "";
  char temp1[120] = "";
  char temp2[120] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  serialRecvFlush();

  mountStatus.update(true);

  sendHtmlStart();

  String data=FPSTR(html_headB);
  data.concat(FPSTR(FPSTR(html_headerIdx))); // page refresh
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
  if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(data); return; }

  data.concat("<div style='width: 27em;'>");

  data.concat("<b>" L_SITE ":</b><br />");
  // Browser time
  data.concat(FPSTR(html_settingsBrowserTime));

  // UTC Date
  if (!command(":GX81#", temp1)) strcpy(temp1, "?");
  stripNum(temp1);
  sprintf_P(temp, html_indexDate, temp1);
  data.concat(temp);

  // UTC Time
  if (!command(":GX80#", temp1)) strcpy(temp1, "?");
  sprintf_P(temp, html_indexTime, temp1);
  data.concat(temp);

  // LST
  if (!command(":GS#", temp1)) strcpy(temp1, "?");
  sprintf_P(temp, html_indexSidereal, temp1);
  data.concat(temp);

  // Longitude and Latitude
  if (mountStatus.getVersionMajor() > 3) {
    if (!command(":GgH#", temp1)) strcpy(temp1, "?");
    temp1[10] = 0;
    if (!command(":GtH#", temp2)) strcpy(temp2, "?");
    temp2[9] = 0;
  } else {
    if (!command(":Gg#", temp1)) strcpy(temp1, "?");
    if (!command(":Gt#", temp2)) strcpy(temp2, "?");
  }
  sprintf_P(temp,html_indexSite,temp1,temp2);
  data.concat(temp);
  sendHtml(data);

  // keep numeric latitude for later, in degrees
  long lat = LONG_MIN;
  temp2[3] = 0;
  if (temp2[0] == '+') temp2[0] = '0';
  lat = atol(temp2);

  // Ambient conditions
  #if DISPLAY_WEATHER == ON
    if (!command(":GX9A#",temp1)) strcpy(temp1,"?"); else localeTemperature(temp1,temp2);
    sprintf_P(temp,html_indexTPHD,L_TEMPERATURE ":",temp1,temp2); data.concat(temp);
    if (!command(":GX9B#",temp1)) strcpy(temp1,"?"); else localePressure(temp1,temp2);
    sprintf_P(temp,html_indexTPHD,L_PRESSURE ":",temp1,temp2); data.concat(temp);
    if (!command(":GX9C#",temp1)) strcpy(temp1,"?");
    sprintf_P(temp,html_indexTPHD,L_HUMIDITY ":",temp1,"%"); data.concat(temp);
    if (!command(":GX9E#",temp1)) strcpy(temp1,"?"); else localeTemperature(temp1,temp2);
    sprintf_P(temp,html_indexTPHD,L_DEW_POINT ":",temp1,temp2); data.concat(temp);
  #endif

  // Focuser/telescope temperature
  if (mountStatus.focuserPresent()) {
    if (!command(":Ft#", temp1)) strcpy(temp1, "?"); else localeTemperature(temp1, temp2);
    sprintf_P(temp, html_indexTPHD, L_TELE_TEMPERATURE ":",temp1,temp2); data.concat(temp);
  }
  
  data.concat("<br /><b>" L_COORDINATES ":</b><br />");

  #if DISPLAY_HIGH_PRECISION_COORDS == ON
    // RA,Dec current
    if (!command(":GRa#",temp1)) strcpy(temp1,"?");
    if (!command(":GDe#",temp2)) strcpy(temp2,"?");
    sprintf_P(temp,html_indexPosition,temp1,temp2); 
    data.concat(temp);

    // RA,Dec target
    if (!command(":Gra#",temp1)) strcpy(temp1,"?");
    if (!command(":Gde#",temp2)) strcpy(temp2,"?");
    sprintf_P(temp,html_indexTarget,temp1,temp2); 
    data.concat(temp);
  #else
    // RA,Dec current
    if (!command(":GR#",temp1)) strcpy(temp1,"?");
    if (!command(":GD#",temp2)) strcpy(temp2,"?");
    sprintf_P(temp,html_indexPosition,temp1,temp2); 
    data.concat(temp);

    // RA,Dec target
    if (!command(":Gr#",temp1)) strcpy(temp1,"?");
    if (!command(":Gd#",temp2)) strcpy(temp2,"?");
    sprintf_P(temp,html_indexTarget,temp1,temp2); 
    data.concat(temp);
  #endif

  #if ENCODERS == ON
    // RA,Dec OnStep position
    double f;
    f=encoders.getOnStepAxis1(); doubleToDms(temp1, f, true, true, PM_HIGH);
    f=encoders.getOnStepAxis2(); doubleToDms(temp2, f, true, true, PM_HIGH);
    sprintf_P(temp,html_indexEncoder1,temp1,temp2);
    data.concat(temp);

    // RA,Dec encoder position
    if (encoders.validAxis1()) { f=encoders.getAxis1(); doubleToDms(temp1, f, true, true, PM_HIGH); } else strcpy(temp1," ** " L_FAULT " ** ");
    if (encoders.validAxis2()) { f=encoders.getAxis2(); doubleToDms(temp2, f, true, true, PM_HIGH); } else strcpy(temp2," ** " L_FAULT " ** ");
    sprintf_P(temp,html_indexEncoder2,temp1,temp2);
    data.concat(temp);
  #endif

  // pier side and meridian flips
  if ((mountStatus.pierSide()==PierSideFlipWE1) || (mountStatus.pierSide()==PierSideFlipWE2) || (mountStatus.pierSide()==PierSideFlipWE3)) strcpy(temp1,L_MERIDIAN_FLIP_W_TO_E); else
  if ((mountStatus.pierSide()==PierSideFlipEW1) || (mountStatus.pierSide()==PierSideFlipEW2) || (mountStatus.pierSide()==PierSideFlipEW3)) strcpy(temp1,L_MERIDIAN_FLIP_E_TO_W); else
  if (mountStatus.pierSide()==PierSideWest) strcpy(temp1,L_WEST); else
  if (mountStatus.pierSide()==PierSideEast) strcpy(temp1,L_EAST); else
  if (mountStatus.pierSide()==PierSideNone) strcpy(temp1,L_NONE); else strcpy(temp1,L_UNKNOWN);
  if (!mountStatus.valid()) strcpy(temp1,"?");
  if (mountStatus.meridianFlips()) {
    strcpy(temp2,"On");
    if (mountStatus.autoMeridianFlips()) strcat(temp2,"</font>, <font class=\"c\">" L_AUTO);
  } else strcpy(temp2,"Off");
  if (!mountStatus.valid()) strcpy(temp2,"?");
  sprintf_P(temp,html_indexPier,temp1,temp2);
  data.concat(temp);
  sendHtml(data);

  if (abs(lat) <= 89) {
    long ud = LONG_MIN; if (command(":GX02#",temp1)) { ud = strtol(&temp1[0], NULL, 10); if (lat < 0) ud = -ud; }
    long lr = LONG_MIN; if (command(":GX03#",temp1)) { lr = strtol(&temp1[0], NULL, 10); lr = lr/cos(lat/57.295); }

    if (lat != LONG_MIN && ud != LONG_MIN && lr != LONG_MIN) {
      data.concat("<br /><b>" L_POLAR_ALIGN ":</b><br />");

      // default to arc-minutes unless we get close, then arc-seconds
      char units='"';
      if (abs(ud) >= 300 || abs(lr) >= 300) { 
        ud = ud/60.0; lr = lr/60.0;
        units='\'';
      }

      if (mountStatus.mountType()==MT_ALTAZM) {
        strcpy(temp1, L_ZENITH);
      } else {
        if (lat<0) strcpy(temp1, L_SCP); else strcpy(temp1, L_NCP);
      }

      // show direction
      if ((ud< 0) && (lr< 0)) sprintf_P(temp,html_indexCorPolar,rightTri,(long)(abs(lr)),units,downTri,(long)(abs(ud)),units,temp1); else
      if ((ud>=0) && (lr< 0)) sprintf_P(temp,html_indexCorPolar,rightTri,(long)(abs(lr)),units,upTri  ,(long)(abs(ud)),units,temp1); else
      if ((ud< 0) && (lr>=0)) sprintf_P(temp,html_indexCorPolar,leftTri ,(long)(abs(lr)),units,downTri,(long)(abs(ud)),units,temp1); else
      if ((ud>=0) && (lr>=0)) sprintf_P(temp,html_indexCorPolar,leftTri ,(long)(abs(lr)),units,upTri  ,(long)(abs(ud)),units,temp1);

      data.concat(temp);
    }
  }
  sendHtml(data);

  data.concat("<br /><b>" L_OPERATIONS ":</b><br />");

  // Park
  if (mountStatus.parked()) strcpy(temp1,L_PARKED); else strcpy(temp1,L_NOT_PARKED);
  if (mountStatus.parking()) strcpy(temp1,L_PARKING); else
  if (mountStatus.parkFail()) strcpy(temp1,L_PARK_FAILED);
  if (mountStatus.atHome()) strcat(temp1," </font>(<font class=\"c\">" L_AT_HOME "</font>)<font class=\"c\">");
  if (!mountStatus.valid()) strcpy(temp1,"?");
  sprintf_P(temp,html_indexPark,temp1);
  data.concat(temp);

  // Tracking
  if (mountStatus.tracking()) strcpy(temp1,L_ON); else strcpy(temp1,L_OFF);
  if (mountStatus.inGoto()) strcpy(temp1,L_INGOTO);
  if (!mountStatus.valid()) strcpy(temp1,"?");
  
  strcpy(temp2,"</font>(<font class=\"c\">");
  if (mountStatus.ppsSync()) strcat(temp2,L_PPS_SYNC ", ");
  if (mountStatus.rateCompensation()==RC_REFR_RA)   strcat(temp2,L_REFR_COMP_RA ", ");
  if (mountStatus.rateCompensation()==RC_REFR_BOTH) strcat(temp2,L_REFR_COMP_BOTH ", ");
  if (mountStatus.rateCompensation()==RC_FULL_RA)   strcat(temp2,L_FULL_COMP_RA ", ");
  if (mountStatus.rateCompensation()==RC_FULL_BOTH) strcat(temp2,L_FULL_COMP_BOTH ", ");
  if (!mountStatus.valid()) strcpy(temp2,"?");
  if (temp2[strlen(temp2)-2]==',') { temp2[strlen(temp2)-2]=0; strcat(temp2,"</font>)<font class=\"c\">"); } else strcpy(temp2,"");
  sprintf_P(temp,html_indexTracking,temp1,temp2);
  data.concat(temp);
  sendHtml(data);

  // Tracking rate
  if ((command(":GT#",temp1)) && (strlen(temp1)>6)) {
    double tr=atof(temp1);
    dtostrf(tr,5,3,temp1);
    sprintf(temp,"&nbsp;&nbsp;" L_TRACKING_RATE ": <font class=\"c\">%s</font>Hz<br />",temp1);
    data.concat(temp);
  }

  // Slew speed
  if ((command(":GX97#",temp1)) && (strlen(temp1)>2)) {
    sprintf_P(temp,html_indexMaxSpeed,temp1);
    data.concat(temp);
  } else {
    // fall back to MaxRate display if not supported
    if ((command(":GX92#",temp1)) && (command(":GX93#",temp2))) { 
      long maxRate=strtol(&temp1[0],NULL,10);
      long MaxRate=strtol(&temp2[0],NULL,10);
      sprintf_P(temp,html_indexMaxRate,maxRate,MaxRate);
    } else sprintf_P(temp,html_indexMaxSpeed,"?");
    data.concat(temp);
  }
  sendHtml(data);

  data.concat("<br /><b>" L_STATE ":</b><br />");

  // Driver status
  int numAxes = 2;
  if (mountStatus.getVersionMajor() >= 10) numAxes = 9;
  for (int axis = 0; axis < numAxes; axis++) {
    if (mountStatus.driver[axis].valid) {
      sprintf(temp, "&nbsp;&nbsp;Axis%d", axis + 1);
      data.concat(temp);
      strcpy(temp1,"");
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
      sprintf_P(temp, html_indexDriverStatus, temp1);
      data.concat(temp);
    }
  }

  // MCU Temperature
  #if DISPLAY_INTERNAL_TEMPERATURE == ON
    if (!command(":GX9F#",temp1)) strcpy(temp1,"?"); else localeTemperature(temp1,temp2); sprintf_P(temp,html_indexTPHD,L_INTERNAL_TEMP ":",temp1,temp2); data.concat(temp);
  #endif

  // General Error
  if (mountStatus.lastError()!=ERR_NONE) strcpy(temp1,"</font><font class=\"y\">"); else strcpy(temp1,"");
  mountStatus.getLastErrorMessage(temp2);
  strcat(temp1,temp2);
  if (!mountStatus.valid()) strcpy(temp1,"?");
  sprintf_P(temp,html_indexGeneralError,temp1);
  data.concat(temp);

  // Loop time
  if (!command(":GXFA#",temp1)) strcpy(temp1,"?%");
  sprintf_P(temp,html_indexWorkload,temp1);
  data.concat(temp);

  #if OPERATIONAL_MODE == WIFI && DISPLAY_WIFI_SIGNAL_STRENGTH == ON
    long signal_strength_dbm = WiFi.RSSI();
    long signal_strength_qty = 2*(signal_strength_dbm + 100);
    if (signal_strength_qty > 100) signal_strength_qty = 100; 
    else if (signal_strength_qty < 0) signal_strength_qty = 0;
    sprintf(temp1,"%lddBm (%ld%%)", signal_strength_dbm, signal_strength_qty);
    sprintf_P(temp, html_indexSignalStrength, temp1);
    data.concat(temp);
  #endif
  data.concat("</div><br class=\"clear\" />\r\n");
  data.concat("</div></body></html>");

  sendHtml(data);
  sendHtmlDone(data);
}
