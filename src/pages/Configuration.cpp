// -----------------------------------------------------------------------------------
// Configuration

#include "Configuration.h"

bool processConfigurationGet();
void sendAxisParams(AxisSettings* a, int axis);

void handleConfiguration() {
  char temp[360] = "";
  char temp1[120] = "";
  char temp2[120] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();
 
  processConfigurationGet();

  sendHtmlStart();

  // send a standard http response header
  String data = FPSTR(html_headB);
  data.concat(FPSTR(html_main_cssB));
  data.concat(FPSTR(html_main_css1));
  data.concat(FPSTR(html_main_css2));
  data.concat(FPSTR(html_main_css3));
  data.concat(FPSTR(html_main_css4));
  sendHtml(data);
  data.concat(FPSTR(html_main_css5));
  data.concat(FPSTR(html_main_css6));
  data.concat(FPSTR(html_main_css7));
  data.concat(FPSTR(html_main_css8));
  data.concat(FPSTR(html_main_css_collapse1));
  data.concat(FPSTR(html_main_css_collapse2));
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
  data.concat(FPSTR(html_linksStatN));
  data.concat(FPSTR(html_linksCtrlN));
  if (status.featureFound) data.concat(FPSTR(html_linksAuxN));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (status.pecEnabled) data.concat(FPSTR(html_linksPecN));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgS));
  data.concat(FPSTR(html_linksSetupN));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);
  
  // OnStep wasn't found, show warning and info.
  if (!status.valid) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(); return; }
  
  // scripts
  sprintf_P(temp, html_ajaxScript, "configurationA.txt"); data.concat(temp);
  
  data+="<div style='width: 35em;'>";
  data.concat(L_BASIC_SET_TITLE "<br /><br />");
  sendHtml(data);

  data.concat(F("<button type='button' class='collapsible'>" L_LOCATION_TITLE "</button>"));
  data.concat(FPSTR(html_configFormBegin));

  // Longitude
  if (status.getVersionMajor() > 3) {
    if (!onStep.command(":GgH#", temp1)) strcpy(temp1, "+000*00:00");
  } else {
    if (!onStep.command(":Gg#", temp1)) strcpy(temp1, "+000*00");
  }
  temp1[10] = 0;
  temp1[4] = 0;                        // deg part only
  temp1[7] = 0;                        // min part only
  if (temp1[0] == '+') temp1[0] = '0'; // remove +
  stripNum(temp1);
  while (temp1[0] == '0' && strlen(temp1) > 1) memmove(&temp1[0], &temp1[1], strlen(temp1)); // remove leading 0's
  sprintf_P(temp, html_configLongDeg, temp1);
  data.concat(temp);
  sprintf_P(temp, html_configLongMin, (char*)&temp1[5]);
  data.concat(temp);
  if (status.getVersionMajor() > 3) {
    sprintf_P(temp, html_configLongSec, (char*)&temp1[8]);
    data.concat(temp);
  }
  sendHtml(data);
  data.concat(FPSTR(html_configLongMsg));

  // Latitude
  if (status.getVersionMajor() > 3) {
    if (!onStep.command(":GtH#", temp1)) strcpy(temp1, "+00*00:00");
  } else {
    if (!onStep.command(":Gt#", temp1)) strcpy(temp1, "+00*00");
  }
  temp1[9] = 0;
  temp1[3] = 0;                        // deg part only
  temp1[6] = 0;                        // min part only
  if (temp1[0] == '+') temp1[0] = '0'; // remove +
  stripNum(temp1);
  sprintf_P(temp, html_configLatDeg, temp1);
  data.concat(temp);
  sprintf_P(temp, html_configLatMin, (char*)&temp1[4]);
  data.concat(temp);
  if (status.getVersionMajor() > 3) {
    sprintf_P(temp, html_configLatSec, (char*)&temp1[7]);
    data.concat(temp);
  }
  data.concat(FPSTR(html_configLatMsg));
  sendHtml(data);

  // UTC Offset
  if (!onStep.command(":GG#", temp1)) strcpy(temp1, "+00");
  strcpy(temp2, temp1);
  temp2[3] = 0;                        // deg. part only
  if (temp2[0] == '+') temp2[0] = '0'; // remove +
  stripNum(temp2);
  sprintf_P(temp, html_configOffsetHrs, temp2);
  data.concat(temp);
  strcpy(temp2, temp1);
  if (temp2[4] == '3') sprintf_P(temp, html_configOffsetMin, "", "selected", ""); else
    if (temp2[4] == '4') sprintf_P(temp, html_configOffsetMin, "", "", "selected"); else
      sprintf_P(temp, html_configOffsetMin, "selected", "", "");
  data.concat(temp);
  data.concat(F("<button type='submit'>" L_UPLOAD "</button>\r\n"));
  data.concat(FPSTR(html_configFormEnd));
  sendHtml(data);

  // Overhead and Horizon Limits
  data.concat(F("<button type='button' class='collapsible'>" L_LIMITS_TITLE "</button>"));
  data.concat(FPSTR(html_configFormBegin));
  if (!onStep.command(":Gh#", temp1)) strcpy(temp1, "0");
  int minAlt = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configMinAlt, minAlt);
  data.concat(temp);
  if (!onStep.command(":Go#", temp1)) strcpy(temp1, "0");
  int maxAlt = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configMaxAlt, maxAlt);
  data.concat(temp);
  data.concat(F("<button type='submit'>" L_UPLOAD "</button>\r\n"));
  data.concat(FPSTR(html_configFormEnd));
  sendHtml(data);

  // Axis1 RA/Azm
  data.concat(F("<br /><button type='button' class='collapsible'>Axis1 RA/Azm</button>"));
  data.concat(FPSTR(html_configFormBegin));
  // Backlash
  if (!onStep.command(":%BR#", temp1)) strcpy(temp1, "0");
  int backlashAxis1 = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configBlAxis1, backlashAxis1);
  data.concat(temp);
  sendHtml(data);
  // Meridian Limits
  if (status.mountType == MT_GEM && (onStep.command(":GXE9#", temp1)) && (onStep.command(":GXEA#",temp2))) {
    int degPastMerE = (int)strtol(&temp1[0], NULL, 10);
    degPastMerE = round((degPastMerE*15.0)/60.0);
    sprintf_P(temp, html_configPastMerE, degPastMerE);
    data.concat(temp);
    int degPastMerW = (int)strtol(&temp2[0], NULL, 10);
    degPastMerW = round((degPastMerW*15.0)/60.0);
    sprintf_P(temp, html_configPastMerW, degPastMerW);
    data.concat(temp);
  } else data.concat("<br />\r\n");
  sendHtml(data);
  data.concat(F("<button type='submit'>" L_UPLOAD "</button>\r\n"));
  data.concat(FPSTR(html_configFormEnd));
  sendHtml(data);

  // Axis2 Dec/Alt
  data.concat(F("<button type='button' class='collapsible'>Axis2 Dec/Alt</button>"));
  data.concat(FPSTR(html_configFormBegin));
  // Backlash
  if (!onStep.command(":%BD#", temp1)) strcpy(temp1, "0");
  int backlashAxis2 = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configBlAxis2, backlashAxis2);
  data.concat(temp);
  data.concat("<button type='submit'>" L_UPLOAD "</button>\r\n");
  data.concat(FPSTR(html_configFormEnd));
  sendHtml(data);

  if (status.getVersionMajor() > 3) {

    if (status.rotatorFound) {
      data.concat(F("<button type='button' class='collapsible'>Axis3 " L_ROTATOR "</button>"));
      data.concat(FPSTR(html_configFormBegin));
      // Backlash
      if (!onStep.command(":rb#", temp1)) strcpy(temp1, "0");
      sprintf_P(temp, html_configBlAxis3, atoi(temp1));
      data.concat(temp);
      data.concat(F("<button type='submit'>" L_UPLOAD "</button>\r\n"));
      data.concat(FPSTR(html_configFormEnd));
      sendHtml(data);
    }

    for (int focuser = 0; focuser < 6; focuser++) {
      if (status.focuserPresent[focuser]) {
        sprintf_P(temp, html_configFocuser, focuser + 4, focuser + 1);
        data.concat(temp);
        data.concat(FPSTR(html_configFormBegin));
        // Backlash
        if (!onStep.command(":Fb#", temp1)) strcpy(temp1, "0");
        sprintf_P(temp, html_configBacklash, atoi(temp1), focuser + 4);
        data.concat(temp);
        // TCF Enable
        sprintf_P(temp, html_configTcfEnable, (int)onStep.commandBool(":Fc#"), focuser + 4);
        data.concat(temp);
        // TCF Deadband
        if (!onStep.command(":Fd#", temp1)) strcpy(temp1, "0");
        sprintf_P(temp,html_configDeadband, atoi(temp1), focuser + 4);
        data.concat(temp);
        sendHtml(data);
        // TCF Coef
        if (!onStep.command(":FC#", temp1)) strcpy(temp1, "0");
        char *conv_end;
        double f = strtod(temp1,&conv_end);
        if (&temp1[0] == conv_end) f = 0.0;
        dtostrf(f, 1, 5, temp1);
        stripNum(temp1);
        sprintf_P(temp, html_configTcfCoef, temp1, focuser + 4);
        data.concat(temp);
        data.concat(F("<button type='submit'>" L_UPLOAD "</button>\r\n"));
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
      }
    }

    data.concat("<br />\r\n");

    int numShown = 0;

    // Mount type
    int mt = 0;
    if (status.getVersionMajor() >= 5) {
      if (!onStep.command(":GXEM#", temp1)) strcpy(temp1, "0");
      mt = atoi(temp1);
    }
    if ((mt >= 1 && mt <= 3) || DRIVE_CONFIGURATION == ON) data.concat(FPSTR(html_configAdvanced));
    if (mt >= 1 && mt <= 3) {
      data.concat(F("<button type='button' class='collapsible'>Mount Type</button>"));
      data.concat(FPSTR(html_configFormBegin));
      sprintf_P(temp, html_configMountType, mt); data.concat(temp);
      data.concat(F("<button type='submit'>" L_UPLOAD "</button> "));
      data.concat(F("<button name='revert' value='0' type='submit'>" L_REVERT "</button>\r\n"));
      data.concat(FPSTR(html_configFormEnd));
      data.concat("<br />");
      numShown++;
    }

    #if DRIVE_CONFIGURATION == ON
      AxisSettings a;

      // Axis1 RA/Azm
      if (!onStep.command(":GXA1#", temp1)) strcpy(temp1, "0");
      if (decodeAxisSettings(temp1, &a)) {
        data.concat(F("<button type='button' class='collapsible'>Axis1 RA/Azm</button>"));
        data.concat(FPSTR(html_configFormBegin));
        if (validateAxisSettings(1, status.mountType == MT_ALTAZM, a)) {
          if (!onStep.command(":GXE7#", temp1)) strcpy(temp1, "0");

          long spwr = strtol(temp1, NULL, 10);
          sprintf_P(temp, html_configAxisSpwr, spwr, 1, 0, 129600000L);
          data.concat(temp);
          sendHtml(data);

          dtostrf(a.stepsPerMeasure, 1, 3, temp1);
          stripNum(temp1);
          sprintf_P(temp, html_configAxisSpd, temp1, 1, 150, 122400L);
          data.concat(temp);
          sendHtml(data);

          #if DRIVE_MAIN_AXES_REVERSE == ON
            sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, 1);
            data.concat(temp);
            sendHtml(data);
          #endif

          sprintf_P(temp, html_configAxisMin, (int)a.min, 1, -360, -90, "&deg;,");
          data.concat(temp);
          sendHtml(data);

          sprintf_P(temp, html_configAxisMax, (int)a.max, 1, 90, 360, "&deg;,");
          data.concat(temp);
          sendHtml(data);

          sendAxisParams(&a, 1);

          data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button> "));
        }
        sprintf_P(temp, html_configAxisRevert, 1);
        data.concat(temp);
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
        numShown++;
      }

      // Axis2 Dec/Alt
      if (!onStep.command(":GXA2#", temp1)) strcpy(temp1, "0");
      if (decodeAxisSettings(temp1, &a)) {
        data.concat(F("<button type='button' class='collapsible'>Axis2 Dec/Alt</button>"));
        data.concat(FPSTR(html_configFormBegin));
        if (validateAxisSettings(2, status.mountType == MT_ALTAZM, a)) {

          dtostrf(a.stepsPerMeasure, 1, 3, temp1);
          stripNum(temp1);
          sprintf_P(temp, html_configAxisSpd, temp1, 2, 150, 122400L);
          data.concat(temp);
          sendHtml(data);

          #if DRIVE_MAIN_AXES_REVERSE == ON
            sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, 2);
            data.concat(temp);
          #endif

          sprintf_P(temp, html_configAxisMin, (int)a.min, 2, -90, 0, "&deg;,");
          data.concat(temp);
          sendHtml(data);

          sprintf_P(temp, html_configAxisMax, (int)a.max, 2, 0, 90, "&deg;,");
          data.concat(temp);
          sendHtml(data);

          sendAxisParams(&a, 2);

          data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button> "));
        }
        sprintf_P(temp, html_configAxisRevert, 2);
        data.concat(temp);
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
        numShown++;
      }

      // Axis3 Rotator
      if (!onStep.command(":GXA3#", temp1)) strcpy(temp1, "0");
      if (decodeAxisSettings(temp1, &a)) {
        data.concat(F("<button type='button' class='collapsible'>Axis3 " L_ROTATOR "</button>"));
        data.concat(FPSTR(html_configFormBegin));
        if (validateAxisSettings(3, status.mountType == MT_ALTAZM, a)) {

          dtostrf(a.stepsPerMeasure, 1, 3, temp1);
          stripNum(temp1);
          sprintf_P(temp, html_configAxisSpd, temp1, 3, 10, 3600L);
          data.concat(temp);
          sendHtml(data);

          sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, 3);
          data.concat(temp);
          sendHtml(data);

          sprintf_P(temp, html_configAxisMin, (int)a.min, 3, -360, 0, "&deg;,");
          data.concat(temp);
          sendHtml(data);

          sprintf_P(temp, html_configAxisMax, (int)a.max, 3, 0, 360, "&deg;,");
          data.concat(temp);
          sendHtml(data);

          sendAxisParams(&a, 3);

          data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button> "));
        }
        sprintf_P(temp, html_configAxisRevert, 3);
        data.concat(temp);
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
        numShown++;
      }

      // Axis4 to Axis9 Focusers
      for (int focuser = 0; focuser < 6; focuser++) {
        if (status.focuserPresent[focuser]) {
          sprintf(temp2, ":GXA%d#", focuser + 4);
          if (!onStep.command(temp2, temp1)) strcpy(temp1, "0");
          if (decodeAxisSettings(temp1, &a)) {
            sprintf_P(temp, html_configFocuser, focuser + 4, focuser + 1);
            data.concat(temp);
            data.concat(FPSTR(html_configFormBegin));
            if (validateAxisSettings(focuser + 4, status.mountType == MT_ALTAZM, a)) {

              dtostrf(a.stepsPerMeasure, 1, 3, temp1);
              stripNum(temp1);
              sprintf_P(temp, html_configAxisSpu, temp1, focuser + 4);
              data.concat(temp);
              sendHtml(data);

              sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, focuser + 4);
              data.concat(temp);
              sendHtml(data);

              sprintf_P(temp, html_configAxisMin, (int)a.min, focuser + 4, -500, 500, "mm,");
              data.concat(temp);
              sendHtml(data);

              sprintf_P(temp, html_configAxisMax, (int)a.max, focuser + 4, -500, 500, "mm,");
              data.concat(temp);
              sendHtml(data);

              sendAxisParams(&a, focuser + 4);

              data.concat(F("<br /><button type='submit'>" L_UPLOAD "</button> "));
            }
            sprintf_P(temp, html_configAxisRevert, focuser + 4);
            data.concat(temp);
            data.concat(FPSTR(html_configFormEnd));
            sendHtml(data);
            numShown++;
          }
        }
      }

      if (numShown == 0) data.concat(L_ADV_SET_NO_EDIT "<br />");
      data.concat(F("<br /><form method='get' action='/configuration.htm'>"));
      data.concat(F("<button name='advanced' type='submit' "));
      if (numShown == 0) data.concat("value='enable'>" L_ADV_ENABLE "</button>"); else data.concat("value='disable'>" L_ADV_DISABLE "</button>");
      data.concat("</form>\r\n");
      if (numShown > 0) data.concat(FPSTR(html_configAxesNotes));
    #endif

    #if DISPLAY_RESET_CONTROLS != OFF
      sendHtml(data);
      data.concat("<hr>" L_RESET_TITLE "<br/><br/>");
      data.concat("<button onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) s('advanced','reset')\" type='button'>" L_RESET "!</button>");
      #if defined(BOOT0_PIN) && DISPLAY_RESET_CONTROLS == FWU
        data.concat(" &nbsp;&nbsp;<button onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) s('advanced','fwu')\" type='button'>" L_RESET_FWU "!</button>");
      #endif
      data.concat("<br/>\r\n");
      data.concat(FPSTR(html_resetNotes));
    #endif
  }

  // collapsible script
  data.concat(FPSTR(html_collapseScript));

  data.concat("<br/><br/>");

  strcpy(temp,"</div></div></body></html>");
  data.concat(temp);

  sendHtml(data);
  sendHtmlDone();
}

void configurationAjaxGet() {
  sendTextStart();
  processConfigurationGet();
  sendTextDone();
}

bool processConfigurationGet() {
  String v, v1, v2;
  char temp[80] = "";

  // Overhead limit
  v = www.arg("ol");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 60 && v.toInt() <= 90) { 
      sprintf(temp, ":So%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Horizon limit
  v = www.arg("hl");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= -30 && v.toInt() <= 30) { 
      sprintf(temp, ":Sh%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }

  // Meridian limit E
  v = www.arg("el");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= -270 && v.toInt() <= 270) { 
      sprintf(temp, ":SXE9,%d#", (int16_t)round((v.toInt()*60.0)/15.0));
      onStep.commandBool(temp);
    }
  }

  // Meridian limit W
  v = www.arg("wl");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= -270 && v.toInt() <= 270) { 
      sprintf(temp, ":SXEA,%d#", (int16_t)round((v.toInt()*60.0)/15.0));
      onStep.commandBool(temp);
    }
  }

  // Backlash
  v = www.arg("b1");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 0 && v.toInt() <= 3600) { 
      sprintf(temp, ":$BR%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }
  v = www.arg("b2");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 0 && v.toInt() <= 3600) { 
      sprintf(temp, ":$BD%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }
  v = www.arg("b3");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 0 && v.toInt() <= 32767) { 
      sprintf(temp, ":rb%d#", (int16_t)v.toInt());
      onStep.commandBool(temp);
    }
  }
  v = www.arg("b4");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 0 && v.toInt() <= 32767) { 
      sprintf(temp, ":Fb%d#", (int16_t)v.toInt());
      onStep.commandBool(":FA1#");
      onStep.commandBool(temp);
    }
  }
  v = www.arg("b5");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 0 && v.toInt() <= 32767) { 
      sprintf(temp, ":fb%d#", (int16_t)v.toInt());
      onStep.commandBool(":FA1#");
      onStep.commandBool(temp);
    }
  }

  // TCF deadband
  v = www.arg("d4");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 1 && v.toInt() <= 32767) { 
      sprintf(temp, ":Fd%d#", (int16_t)v.toInt());
      onStep.commandBool(":FA1#");
      onStep.commandBool(temp);
    }
  }
  v = www.arg("d5");
  if (!v.equals(EmptyStr)) {
    if (v.toInt() >= 1 && v.toInt() <= 32767) { 
      sprintf(temp, ":fd%d#", (int16_t)v.toInt());
      onStep.commandBool(":FA1#");
      onStep.commandBool(temp);
    }
  }

  // TCF Coef
  v = www.arg("tc4");
  if (!v.equals(EmptyStr)) {
    if (v.toFloat() >= -999.0 && v.toFloat() <= 999.0) { 
      sprintf(temp, ":FC%s#", v.c_str());
      onStep.commandBool(":FA1#");
      onStep.commandBool(temp);
    }
  }
  v = www.arg("tc5");
  if (!v.equals(EmptyStr)) {
    if (v.toFloat() >= -999.0 && v.toFloat() <= 999.0) { 
      sprintf(temp, ":fC%s#", v.c_str());
      onStep.commandBool(":FA1#");
      onStep.commandBool(temp);
    }
  }

  // TCF Enable
  v = www.arg("en4");
  if (v.equals("0") || v.equals("1")) {
    sprintf(temp, ":Fc%s#", v.c_str());
    onStep.commandBool(":FA1#");
    onStep.commandBool(temp);
  }
  v = www.arg("en5");
  if (v.equals("0") || v.equals("1")) {
    sprintf(temp, ":fc%s#", v.c_str());
    onStep.commandBool(":FA1#");
    onStep.commandBool(temp);
  }

  // Location
  v = www.arg("g1");  // long deg
  v1 = www.arg("g2"); // long min
  if (status.getVersionMajor() > 3) v2 = www.arg("g3"); else v2 = "0"; // long sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.toInt() >= -180 && v.toInt() <= 180 && v1.toInt() >= 0 && v1.toInt() <= 60 && v2.toInt() >= 0 && v2.toInt() <= 60) {
      if (status.getVersionMajor() > 3)
        sprintf(temp,":Sg%+04d*%02d:%02d#",(int16_t)v.toInt(),(int16_t)v1.toInt(),(int16_t)v2.toInt());
      else
        sprintf(temp,":Sg%+04d*%02d#", (int16_t)v.toInt(), (int16_t)v1.toInt());
      onStep.commandBool(temp);
    }
  }

  v = www.arg("t1");  // lat deg
  v1 = www.arg("t2"); // lat min
  if (status.getVersionMajor() > 3) v2 = www.arg("t3"); else v2 = "0"; // lat sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.toInt() >= -90 && v.toInt() <= 90 && v1.toInt() >= 0 && v1.toInt() <= 60 && v2.toInt() >= 0 && v2.toInt() <= 60) {
      sprintf(temp,":St%+03d*%02d:%02d#",(int16_t)v.toInt(),(int16_t)v1.toInt(),(int16_t)v2.toInt());
      onStep.commandBool(temp);
    }
  }

  v = www.arg("u1");  // UT hrs
  v1 = www.arg("u2"); // UT min
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr)) {
    if (v.toInt() >= -14 && v.toInt() <= 12 && (v1.toInt() == 0 || v1.toInt() == 30 || v1.toInt() == 45)) {
      sprintf(temp, ":SG%+03d:%02d#", (int16_t)v.toInt(), (int16_t)v1.toInt());
      onStep.commandBool(temp);
    }
  }

  String ssa = www.arg("advanced");
  #if DISPLAY_RESET_CONTROLS != OFF
    if (ssa.equals("reset")) {

      #if RESET_PIN == OFF
        onStep.commandBlind(":ERESET#");
      #else
        digitalWrite(RESET_PIN, RESET_PIN_STATE);
        pinMode(RESET_PIN, OUTPUT);
        delay(250);
        pinMode(RESET_PIN, INPUT);
      #endif

      delay(250);
      return false;
    }
    #ifdef BOOT0_PIN
      if (ssa.equals("fwu")) {
        digitalWrite(BOOT0_PIN, HIGH);
        pinMode(BOOT0_PIN, OUTPUT);

        #if RESET_PIN == OFF
          onStep.commandBlind(":ERESET#");
        #else
          digitalWrite(RESET_PIN, RESET_PIN_STATE);
          pinMode(RESET_PIN, OUTPUT);
          delay(250);
          pinMode(RESET_PIN, INPUT);
        #endif

        delay(250);
        pinMode(BOOT0_PIN, INPUT);
        return false;
      }
    #endif
  #endif
  String ssm = www.arg("mountt");
  if (!ssm.equals(EmptyStr)) {
    sprintf(temp,":SXEM,%s#",ssm.c_str());
    onStep.commandBool(temp);
  }

  #if DRIVE_CONFIGURATION == ON
    // Axis settings
    if (ssa.equals("enable")) { onStep.commandBool(":SXAC,0#"); return true; }
    if (ssa.equals("disable")) { onStep.commandBool(":SXAC,1#"); return true; }

    String ssr=www.arg("revert");
    if (!ssr.equals(EmptyStr)) {
      int axis=ssr.toInt();
      if (axis > 0 && axis < 5) { sprintf(temp,":SXA%d,R#",axis); onStep.commandBool(temp); }
      if (axis == 0) { strcpy(temp,":SXEM,0#"); onStep.commandBool(temp); }
      return true;
    }

    String axisStr = "0";
    if (!www.arg("a1spd").equals(EmptyStr)) axisStr = "1"; else
    if (!www.arg("a2spd").equals(EmptyStr)) axisStr = "2"; else
    if (!www.arg("a3spd").equals(EmptyStr)) axisStr = "3"; else
    if (!www.arg("a4spu").equals(EmptyStr)) axisStr = "4"; else
    if (!www.arg("a5spu").equals(EmptyStr)) axisStr = "5"; else
    if (!www.arg("a6spu").equals(EmptyStr)) axisStr = "6"; else
    if (!www.arg("a7spu").equals(EmptyStr)) axisStr = "7"; else
    if (!www.arg("a8spu").equals(EmptyStr)) axisStr = "8"; else
    if (!www.arg("a9spu").equals(EmptyStr)) axisStr = "9";

    if (status.getVersionMajor() < 10) {
      // send axis settings to OnStep
      String s1 = www.arg("a" + axisStr + "spd");
      String s2 = www.arg("a" + axisStr + "ustp");
      String s3 = www.arg("a" + axisStr + "I");
      String s4 = www.arg("a" + axisStr + "rev");
      String s5 = www.arg("a" + axisStr + "min");
      String s6 = www.arg("a" + axisStr + "max");

      if (axisStr.toInt() > 0 && axisStr.toInt() < 6) {
        if (s2.equals(EmptyStr)) s2 = "-1";
        if (s3.equals(EmptyStr)) s3 = "-1";
        if (s4.equals(EmptyStr)) s4 = "-1";
        if (s5.equals(EmptyStr)) s5 = "-1";
        if (s6.equals(EmptyStr)) s6 = "-1";
        if (s4.equals("0")) s4 = "-1"; else if (s4.equals("1")) s4 = "-2";

        v = s1 + "," + s2 + "," + s3 + "," + s4 + "," + s5 + "," + s6;
        sprintf(temp, ":SXA%d,%s#", (int)axisStr.toInt(), v.c_str());
        onStep.commandBool(temp);

        if (!www.arg("a1spwr").equals(EmptyStr)) {
          sprintf(temp, ":SXE7,%s#", www.arg("a1spwr").c_str());
          onStep.commandBool(temp);
        }
      }
    } else {
      // send axis settings to OnStepX
      String s1 = www.arg("a" + axisStr + "spd");
      String s2 = www.arg("a" + axisStr + "rev");
      String s3 = www.arg("a" + axisStr + "min");
      String s4 = www.arg("a" + axisStr + "max");
      String s5 = www.arg("a" + axisStr + "ustp");
      String s6, s7, s8, s9, s10;
      if (s5.equals(EmptyStr)) {
        s5 = www.arg("a" + axisStr + "p");
        s6 = www.arg("a" + axisStr + "i");
        s7 = www.arg("a" + axisStr + "d");
        s8 = www.arg("a" + axisStr + "pGoto");
        s9 = www.arg("a" + axisStr + "iGoto");
        s10 = www.arg("a" + axisStr + "dGoto");
      } else {
        s6 = www.arg("a" + axisStr + "ustpGoto");
        s7 = www.arg("a" + axisStr + "Ih");;
        s8 = www.arg("a" + axisStr + "I");
        s9 = www.arg("a" + axisStr + "Is");
        s10 = "";
      }

      if (axisStr.toInt() > 0 && axisStr.toInt() < 10) {
        if (s2.equals(EmptyStr)) s2 = "-1";
        if (s2.equals("0")) s2 = "-1"; else if (s2.equals("1")) s2 = "-2";
        if (s3.equals(EmptyStr)) s3 = "-1";
        if (s4.equals(EmptyStr)) s4 = "-1";
        if (s5.equals(EmptyStr)) s5 = "-1";
        if (s6.equals(EmptyStr)) s6 = "-1";
        if (s7.equals(EmptyStr)) s7 = "-1";
        if (s8.equals(EmptyStr)) s8 = "-1";
        if (s9.equals(EmptyStr)) s9 = "-1";
        if (s10.equals(EmptyStr)) s10 = "-1";

        v = s1 + "," + s2 + "," + s3 + "," + s4 + "," + s5 + "," + s6 + "," + s7 + "," + s8 + "," + s9 + "," + s10;
        sprintf(temp, ":SXA%d,%s#", (int)axisStr.toInt(), v.c_str());
        onStep.commandBool(temp);

        if (!www.arg("a1spwr").equals(EmptyStr)) {
          sprintf(temp, ":SXE7,%s#", www.arg("a1spwr").c_str());
          onStep.commandBool(temp);
        }
      }
    }

  #endif

  return true;
}

void sendAxisParams(AxisSettings* a, int axis) {
  char temp[300], temp1[40];
  String data = "";

  if (a->driverType == DT_SERVO) {
    data.concat(L_ADV_SET_IMMEDIATE);
    data.concat("<br/><br/>");
    sendHtml(data);

    dtostrf(a->p, 1, 3, temp1);
    stripNum(temp1);
    sprintf_P(temp, html_configAxisP, temp1, axis, 0, 99999999L);
    data.concat(temp);
    sendHtml(data);

    dtostrf(a->i, 1, 3, temp1);
    stripNum(temp1);
    sprintf_P(temp, html_configAxisI, temp1, axis, 0, 99999999L);
    data.concat(temp);
    sendHtml(data);

    dtostrf(a->d, 1, 3, temp1);
    stripNum(temp1);
    sprintf_P(temp, html_configAxisD, temp1, axis, 0, 99999999L);
    data.concat(temp);
    sendHtml(data);

    dtostrf(a->pGoto, 1, 3, temp1);
    stripNum(temp1);
    sprintf_P(temp, html_configAxisGotoP, temp1, axis, 0, 99999999L);
    data.concat(temp);
    sendHtml(data);

    dtostrf(a->iGoto, 1, 3, temp1);
    stripNum(temp1);
    sprintf_P(temp, html_configAxisGotoI, temp1, axis, 0, 99999999L);
    data.concat(temp);
    sendHtml(data);

    dtostrf(a->dGoto, 1, 3, temp1);
    stripNum(temp1);
    sprintf_P(temp, html_configAxisGotoD, temp1, axis, 0, 99999999L);
    data.concat(temp);
    sendHtml(data);
  } else

  if (a->driverType == DT_STEP_DIR_STANDARD) {
    data.concat(L_ADV_SET_SPECIAL);
    data.concat("<br/><br/>");
    sendHtml(data);

    #if DRIVE_MAIN_AXES_MICROSTEPS == ON
      sprintf_P(temp, html_configAxisMicroSteps, (int)a->microsteps, axis);
      data.concat(temp);
      sendHtml(data);

      sprintf_P(temp, html_configAxisMicroStepsGoto, (int)a->microstepsGoto, axis);
      data.concat(temp);
      sendHtml(data);
    #endif
  } else

  if (a->driverType == DT_STEP_DIR_TMC_SPI) {
    data.concat(L_ADV_SET_SPECIAL);
    data.concat("<br/><br/>");
    sendHtml(data);

    #if DRIVE_MAIN_AXES_MICROSTEPS == ON
      sprintf_P(temp, html_configAxisMicroSteps, (int)a->microsteps, axis);
      data.concat(temp);
      sendHtml(data);

      sprintf_P(temp, html_configAxisMicroStepsGoto, (int)a->microstepsGoto, axis);
      data.concat(temp);
      sendHtml(data);
    #endif
    #if DRIVE_MAIN_AXES_CURRENT == ON
      sprintf_P(temp, html_configAxisCurrentHold, (int)a->currentHold, axis, 3000);
      data.concat(temp);
      sendHtml(data);

      sprintf_P(temp, html_configAxisCurrentTrak, (int)a->currentRun, axis, 3000);
      data.concat(temp);
      sendHtml(data);

      sprintf_P(temp, html_configAxisCurrentSlew, (int)a->currentGoto, axis, 3000);
      data.concat(temp);
      sendHtml(data);
    #endif
  } else

  if (a->driverType == DT_STEP_DIR_LEGACY) {
    #if DRIVE_MAIN_AXES_MICROSTEPS == ON
      if (a->microsteps != OFF) {
        sprintf_P(temp, html_configAxisMicroSteps, (int)a->microsteps, axis);
        data.concat(temp);
        sendHtml(data);
      }
    #endif
    #if DRIVE_MAIN_AXES_CURRENT == ON
      if (a->currentRun != OFF) {
        sprintf_P(temp, html_configAxisCurrentTrak, (int)a->currentRun, axis, 3000);
        data.concat(temp);
        sendHtml(data);
      }
      if (a->currentGoto != OFF) {
        sprintf_P(temp, html_configAxisCurrentSlew, (int)a->currentGoto, axis, 3000);
        data.concat(temp);
        sendHtml(data);
      }
    #endif
  }
}
