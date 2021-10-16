// -----------------------------------------------------------------------------------
// Configuration

#include "Configuration.h"

extern int focuserCount;
extern bool focuserPresent[6];

bool processConfigurationGet();

void handleConfiguration() {
  char temp[360] = "";
  char temp1[120] = "";
  char temp2[120] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();
  
  mountStatus.update();
 
  bool success=processConfigurationGet();

  sendHtmlStart();

  // send a standard http response header
  String data=FPSTR(html_headB);
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
  data.concat(FPSTR(html_onstep_header1)); data.concat("OnStep");
  data.concat(FPSTR(html_onstep_header2)); data.concat(firmwareVersion.str);
  data.concat(" (OnStep");
  if (mountStatus.getVersionStr(temp1)) data.concat(temp1); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatN));
  data.concat(FPSTR(html_linksCtrlN));
  if (mountStatus.featureFound()) data.concat(FPSTR(html_linksAuxN));
  data.concat(FPSTR(html_linksLibN));
  #if ENCODERS == ON
    data.concat(FPSTR(html_linksEncN));
  #endif
  sendHtml(data);
  if (mountStatus.pecEnabled()) data.concat(FPSTR(html_linksPecN));
  data.concat(FPSTR(html_linksSetN));
  data.concat(FPSTR(html_linksCfgS));
  data.concat(FPSTR(html_linksSetupN));
  data.concat(FPSTR(html_onstep_header4));
  sendHtml(data);
  
  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid() || !success) { data+= FPSTR(html_bad_comms_message); sendHtml(data); sendHtmlDone(data); return; }
  
  // scripts
  sprintf_P(temp, html_ajaxScript, "configurationA.txt"); data.concat(temp);
  
  data+="<div style='width: 35em;'>";
  data.concat(L_BASIC_SET_TITLE "<br /><br />");
  sendHtml(data);

  data.concat(F("<button type='button' class='collapsible'>" L_LOCATION_TITLE "</button>"));
  data.concat(FPSTR(html_configFormBegin));

  // Longitude
  if (mountStatus.getVersionMajor() > 3) {
    if (!onStep.command(":GgH#",temp1)) strcpy(temp1,"+000*00:00");
  } else {
    if (!onStep.command(":Gg#",temp1)) strcpy(temp1,"+000*00");
  }
  temp1[10] = 0;
  temp1[4] = 0;                        // deg part only
  temp1[7] = 0;                        // min part only
  if (temp1[0] == '+') temp1[0] = '0'; // remove +
  stripNum(temp1);
  while (temp1[0] == '0' && strlen(temp1) > 1) memmove(&temp1[0], &temp1[1], strlen(temp1)); // remove leading 0's
  sprintf_P(temp,html_configLongDeg,temp1);
  data.concat(temp);
  sprintf_P(temp,html_configLongMin,(char*)&temp1[5]);
  data.concat(temp);
  if (mountStatus.getVersionMajor() > 3) {
    sprintf_P(temp,html_configLongSec,(char*)&temp1[8]);
    data.concat(temp);
  }
  sendHtml(data);
  data.concat(FPSTR(html_configLongMsg));

  // Latitude
  if (mountStatus.getVersionMajor() > 3) {
    if (!onStep.command(":GtH#",temp1)) strcpy(temp1,"+00*00:00");
  } else {
    if (!onStep.command(":Gt#",temp1)) strcpy(temp1,"+00*00");
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
  if (mountStatus.getVersionMajor() > 3) {
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
  if (!onStep.command(":Gh#",temp1)) strcpy(temp1,"0");
  int minAlt = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configMinAlt, minAlt);
  data.concat(temp);
  if (!onStep.command(":Go#",temp1)) strcpy(temp1,"0");
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
  if (!onStep.command(":%BR#",temp1)) strcpy(temp1,"0");
  int backlashAxis1 = (int)strtol(&temp1[0], NULL, 10);
  sprintf_P(temp, html_configBlAxis1, backlashAxis1);
  data.concat(temp);
  sendHtml(data);
  // Meridian Limits
  if (mountStatus.mountType() == MT_GEM && (onStep.command(":GXE9#",temp1)) && (onStep.command(":GXEA#",temp2))) {
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

  if (mountStatus.getVersionMajor() > 3) {

    // Axis3 Rotator
    int i = 0;
    static int rotatorPresent = -1;
    if (rotatorPresent == -1) {
      onStep.command(":rT#",temp1);
      if (temp1[0] == '0') rotatorPresent = false; else rotatorPresent = true;
    }

    if (rotatorPresent) {
      data.concat(F("<button type='button' class='collapsible'>Axis3 " L_ROTATOR "</button>"));
      data.concat(FPSTR(html_configFormBegin));
      // Backlash
      if (!onStep.command(":rb#", temp1)) strcpy(temp1, "0");
      i = (int)strtol(&temp1[0], NULL, 10);
      sprintf_P(temp, html_configBlAxis3, i);
      data.concat(temp);
      data.concat(F("<button type='submit'>" L_UPLOAD "</button>\r\n"));
      data.concat(FPSTR(html_configFormEnd));
      sendHtml(data);
    }

    // Axis4 to 9 Focusers
    focuserCount = 0;
    for (int i = 0; i < 6; i++) focuserPresent[i] = false;
    if (mountStatus.getVersionMajor() >= 10) {
      if (onStep.commandBool(":F1a#")) { focuserPresent[0] = true; focuserCount++; }
      if (onStep.commandBool(":F2a#")) { focuserPresent[1] = true; focuserCount++; }
      if (onStep.commandBool(":F3a#")) { focuserPresent[2] = true; focuserCount++; }
      if (onStep.commandBool(":F4a#")) { focuserPresent[3] = true; focuserCount++; }
      if (onStep.commandBool(":F5a#")) { focuserPresent[4] = true; focuserCount++; }
      if (onStep.commandBool(":F6a#")) { focuserPresent[5] = true; focuserCount++; }
    } else {
      if (onStep.commandBool(":FA#")) { focuserPresent[0] = true; focuserCount++; }
      if (onStep.commandBool(":fA#")) { focuserPresent[1] = true; focuserCount++; }
    }

    for (int focuser = 0; focuser < 6; focuser++) {
      if (focuserPresent[focuser]) {
        sprintf_P(temp, html_configFocuser, focuser + 4, focuser + 1);
        data.concat(temp);
        data.concat(FPSTR(html_configFormBegin));
        // Backlash
        if (!onStep.command(":Fb#",temp1)) strcpy(temp1,"0");
        i = (int)strtol(&temp1[0], NULL, 10);
        sprintf_P(temp, html_configBacklash, i, focuser + 4);
        data.concat(temp);
        // TCF Enable
        sprintf_P(temp, html_configTcfEnable, (int)onStep.commandBool(":Fc#"), focuser + 4);
        data.concat(temp);
        // TCF Deadband
        if (!onStep.command(":Fd#",temp1)) strcpy(temp1,"0");
        i = (int)strtol(&temp1[0], NULL, 10);
        sprintf_P(temp,html_configDeadband, i, focuser + 4);
        data.concat(temp);
        sendHtml(data);
        // TCF Coef
        if (!onStep.command(":FC#",temp1)) strcpy(temp1,"0");
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
    if (mountStatus.getVersionMajor() >= 5) {
      if (!onStep.command(":GXEM#",temp1)) strcpy(temp1,"0");
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
      if (!onStep.command(":GXA1#", temp1)) strcpy(temp1,"0");
      if (decodeAxisSettings(temp1, &a)) {
        data.concat(F("<button type='button' class='collapsible'>Axis1 RA/Azm</button>"));
        data.concat(FPSTR(html_configFormBegin));
        if (validateAxisSettings(1, mountStatus.mountType() == MT_ALTAZM, a)) {
          if (!onStep.command(":GXE7#", temp1)) strcpy(temp1, "0");
          long spwr = strtol(temp1, NULL, 10);
          sprintf_P(temp, html_configAxisSpwr, spwr, 1, 0, 129600000L);
          data.concat(temp);
          dtostrf(a.stepsPerMeasure, 1, 3, temp1);
          stripNum(temp1);
          sprintf_P(temp, html_configAxisSpd, temp1, 1, 3000, 122400L);
          data.concat(temp);
          #if DRIVE_MAIN_AXES_MICROSTEPS == ON
            if (a.microsteps != OFF) {
              sprintf_P(temp, html_configAxisMicroSteps, (int)a.microsteps, 1);
              data.concat(temp);
            }
          #endif
          sendHtml(data);
          #if DRIVE_MAIN_AXES_CURRENT == ON
            if (a.IRUN != OFF) {
              sprintf_P(temp, html_configAxisCurrent, (int)a.IRUN, 1, 3000);
              data.concat(temp);
            }
          #endif
          #if DRIVE_MAIN_AXES_REVERSE == ON
            sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, 1);
            data.concat(temp);
          #endif
          sprintf_P(temp, html_configAxisMin, (int)a.min, 1, -360, -90, "&deg;,");
          data.concat(temp);
          sendHtml(data);
          sprintf_P(temp, html_configAxisMax, (int)a.max, 1, 90, 360, "&deg;,");
          data.concat(temp);
          data.concat(F("<button type='submit'>" L_UPLOAD "</button> "));
        }
        sprintf_P(temp, html_configAxisRevert, 1);
        data.concat(temp);
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
        numShown++;
      }

      // Axis2 Dec/Alt
      if (!onStep.command(":GXA2#",temp1)) strcpy(temp1,"0");
      if (decodeAxisSettings(temp1, &a)) {
        data.concat(F("<button type='button' class='collapsible'>Axis2 Dec/Alt</button>"));
        data.concat(FPSTR(html_configFormBegin));
        if (validateAxisSettings(2, mountStatus.mountType() == MT_ALTAZM, a)) {
          dtostrf(a.stepsPerMeasure, 1, 3, temp1);
          stripNum(temp1);
          sprintf_P(temp, html_configAxisSpd, temp1, 2, 3000, 122400L);
          data.concat(temp);
          #if DRIVE_MAIN_AXES_MICROSTEPS == ON
            if (a.microsteps != OFF) {
              sprintf_P(temp, html_configAxisMicroSteps, (int)a.microsteps, 2);
              data.concat(temp);
            }
          #endif
          sendHtml(data);
          #if DRIVE_MAIN_AXES_CURRENT == ON
            if (a.IRUN != OFF) {
              sprintf_P(temp, html_configAxisCurrent, (int)a.IRUN, 2, 3000);
              data.concat(temp);
            }
          #endif
          #if DRIVE_MAIN_AXES_REVERSE == ON
            sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, 2);
            data.concat(temp);
          #endif
          sprintf_P(temp, html_configAxisMin, (int)a.min, 2, -90, 0, "&deg;,");
          data.concat(temp);
          sendHtml(data);
          sprintf_P(temp, html_configAxisMax, (int)a.max, 2, 0, 90, "&deg;,");
          data.concat(temp);
          data.concat(F("<button type='submit'>" L_UPLOAD "</button> "));
        }
        sprintf_P(temp, html_configAxisRevert, 2);
        data.concat(temp);
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
        numShown++;
      }

      // Axis3 Rotator
      if (!onStep.command(":GXA3#",temp1)) strcpy(temp1,"0");
      if (decodeAxisSettings(temp1, &a)) {
        data.concat(F("<button type='button' class='collapsible'>Axis3 " L_ROTATOR "</button>"));
        data.concat(FPSTR(html_configFormBegin));
        if (validateAxisSettings(3, mountStatus.mountType() == MT_ALTAZM, a)) {
          dtostrf(a.stepsPerMeasure, 1, 3, temp1);
          stripNum(temp1);
          sprintf_P(temp, html_configAxisSpd, temp1, 3, 10, 3600L);
          data.concat(temp);
          if (a.microsteps != OFF) {
            sprintf_P(temp, html_configAxisMicroSteps, (int)a.microsteps, 3);
            data.concat(temp);
          }
          sendHtml(data);
          if (a.IRUN != OFF) {
            sprintf_P(temp, html_configAxisCurrent, (int)a.IRUN, 3, 1000);
            data.concat(temp);
          }
          sprintf_P(temp, html_configAxisReverse, a.reverse == ON ? 1 : 0, 3);
          data.concat(temp);
          sprintf_P(temp, html_configAxisMin, (int)a.min, 3, -360, 0, "&deg;,");
          data.concat(temp);
          sendHtml(data);
          sprintf_P(temp, html_configAxisMax, (int)a.max, 3, 0, 360, "&deg;,");
          data.concat(temp);
          data.concat(F("<button type='submit'>" L_UPLOAD "</button> "));
        }
        sprintf_P(temp, html_configAxisRevert, 3);
        data.concat(temp);
        data.concat(FPSTR(html_configFormEnd));
        sendHtml(data);
        numShown++;
      }

      // Axis4 to Axis9 Focusers
      for (int focuser = 0; focuser < 6; focuser++) {
        if (focuserPresent[focuser]) {
          sprintf(temp2, ":GXA%d#", focuser + 4);
          if (!onStep.command(temp2, temp1)) strcpy(temp1, "0");
          if (decodeAxisSettings(temp1, &a)) {
            sprintf_P(temp, html_configFocuser, focuser + 4, focuser + 1);
            data.concat(temp);
            data.concat(FPSTR(html_configFormBegin));
            if (validateAxisSettings(focuser + 4, mountStatus.mountType() == MT_ALTAZM, a)) {
              dtostrf(a.stepsPerMeasure, 1, 3, temp1);
              stripNum(temp1);
              sprintf_P(temp, html_configAxisSpu, temp1, focuser + 4);
              data.concat(temp);
              if (a.microsteps != OFF) {
                sprintf_P(temp, html_configAxisMicroSteps, (int)a.microsteps, focuser + 4);
                data.concat(temp);
              }
              sendHtml(data);
              if (a.IRUN != OFF) {
                sprintf_P(temp, html_configAxisCurrent, (int)a.IRUN, focuser + 4, 1000);
                data.concat(temp);
              }
              sprintf_P(temp, html_configAxisReverse,a.reverse == ON ? 1 : 0, focuser + 4);
              data.concat(temp);
              sprintf_P(temp, html_configAxisMin, (int)a.min, focuser + 4, -500, 500, "mm,");
              data.concat(temp);
              sendHtml(data);
              sprintf_P(temp, html_configAxisMax, (int)a.max, focuser + 4, -500, 500, "mm,");
              data.concat(temp);
              data.concat(F("<button type='submit'>" L_UPLOAD "</button> "));
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
      data.concat(FPSTR(html_configAxesNotes));
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
  sendHtmlDone(data);
}

void configurationAjaxGet() {
  processConfigurationGet();
  #if OPERATIONAL_MODE != WIFI
    www.sendContent("");
  #else
    www.send(200, "text/html", "");
  #endif
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
  if (mountStatus.getVersionMajor() > 3) v2 = www.arg("g3"); else v2 = "0"; // long sec
  if (!v.equals(EmptyStr) && !v1.equals(EmptyStr) && !v2.equals(EmptyStr)) {
    if (v.toInt() >= -180 && v.toInt() <= 180 && v1.toInt() >= 0 && v1.toInt() <= 60 && v2.toInt() >= 0 && v2.toInt() <= 60) {
      if (mountStatus.getVersionMajor() > 3)
        sprintf(temp,":Sg%+04d*%02d:%02d#",(int16_t)v.toInt(),(int16_t)v1.toInt(),(int16_t)v2.toInt());
      else
        sprintf(temp,":Sg%+04d*%02d#", (int16_t)v.toInt(), (int16_t)v1.toInt());
      onStep.commandBool(temp);
    }
  }

  v = www.arg("t1");  // lat deg
  v1 = www.arg("t2"); // lat min
  if (mountStatus.getVersionMajor() > 3) v2 = www.arg("t3"); else v2 = "0"; // lat sec
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

  String ssa=www.arg("advanced");
  #if DISPLAY_RESET_CONTROLS != OFF
    if (ssa.equals("reset")) { onStep.commandBlind(":ERESET#"); return false; }
    #ifdef BOOT0_PIN
      if (ssa.equals("fwu")) {
        pinMode(BOOT0_PIN,OUTPUT);
        digitalWrite(BOOT0_PIN,HIGH);
        onStep.commandBlind(":ERESET#");
        delay(500);
        pinMode(BOOT0_PIN,INPUT);
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

    int axis = 0;
    String ss1,ss2,ss3,ss4,ss5,s1,s2,s3,s4,s5,s6;
    ss1=www.arg("a1spd");
    ss2=www.arg("a2spd");
    ss3=www.arg("a3spd");
    ss4=www.arg("a4spu");
    ss5=www.arg("a5spu");
    if (!ss1.equals(EmptyStr)) { axis=1; s1=www.arg("a1spd"); s2=www.arg("a1ustp"); s3=www.arg("a1I"); s4=www.arg("a1rev"); s5=www.arg("a1min"); s6=www.arg("a1max"); } else
    if (!ss2.equals(EmptyStr)) { axis=2; s1=www.arg("a2spd"); s2=www.arg("a2ustp"); s3=www.arg("a2I"); s4=www.arg("a2rev"); s5=www.arg("a2min"); s6=www.arg("a2max"); } else
    if (!ss3.equals(EmptyStr)) { axis=3; s1=www.arg("a3spd"); s2=www.arg("a3ustp"); s3=www.arg("a3I"); s4=www.arg("a3rev"); s5=www.arg("a3min"); s6=www.arg("a3max"); } else
    if (!ss4.equals(EmptyStr)) { axis=4; s1=www.arg("a4spu"); s2=www.arg("a4ustp"); s3=www.arg("a4I"); s4=www.arg("a4rev"); s5=www.arg("a4min"); s6=www.arg("a4max"); } else
    if (!ss5.equals(EmptyStr)) { axis=5; s1=www.arg("a5spu"); s2=www.arg("a5ustp"); s3=www.arg("a5I"); s4=www.arg("a5rev"); s5=www.arg("a5min"); s6=www.arg("a5max"); }

    if (axis > 0 && axis < 6) {
      if (s2.equals(EmptyStr)) s2 = "-1";
      if (s3.equals(EmptyStr)) s3 = "-1";
      if (s4.equals(EmptyStr)) s4 = "-1";
      if (s5.equals(EmptyStr)) s5 = "-1";
      if (s6.equals(EmptyStr)) s6 = "-1";
      if (s4.equals("0")) s4 = "-1"; else if (s4.equals("1")) s4 = "-2";
      v=s1+","+s2+","+s3+","+s4+","+s5+","+s6;
      sprintf(temp,":SXA%d,%s#",axis,v.c_str());
      onStep.commandBool(temp);
    }
    ss1=www.arg("a1spwr"); if (!ss1.equals(EmptyStr)) { sprintf(temp,":SXE7,%s#",ss1.c_str()); onStep.commandBool(temp); }
  #endif

  return true;
}
