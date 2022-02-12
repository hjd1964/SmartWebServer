// -----------------------------------------------------------------------------------
// Telescope control related functions 

#include "Control.h"

#include "../lib/convert/Convert.h"

void processControlGet();

int focuserCount = 0;
bool focuserPresent[6];
bool rotator;
bool deRotator;

void handleControl() {
  char temp[240] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  mountStatus.update(true);

  processControlGet();

  sendHtmlStart();
  
  String data=FPSTR(html_headB);
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
  data.concat(FPSTR(html_main_css_no_select));
  data.concat(FPSTR(html_main_css_control1));
  data.concat(FPSTR(html_main_css_control2));
  data.concat(FPSTR(html_main_css_control3));
  data.concat(FPSTR(html_main_css_btns1));
  sendHtml(data);
  data.concat(FPSTR(html_main_css_btns2));
  data.concat(FPSTR(html_main_css_btns3));
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
  if (mountStatus.getVersionStr(temp)) data.concat(temp); else data.concat("?");
  data.concat(FPSTR(html_onstep_header3));
  data.concat(FPSTR(html_linksStatN));
  data.concat(FPSTR(html_linksCtrlS));
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

  // scripts
  sprintf_P(temp, html_ajaxScript, "controlA.txt"); data.concat(temp);
  data.concat(FPSTR(html_ajaxScriptShort));
  data.concat(FPSTR(html_dateTimeScriptA));
  data.concat(FPSTR(html_dateTimeScriptB));
  data.concat(FPSTR(html_dateTimeScriptC));

  // active ajax page is: controlAjax();
  data.concat("<script>var ajaxPage='control.txt';</script>\n");
  data.concat(FPSTR(html_ajax_active));
  sendHtml(data);

  // Quick controls ------------------------------------------
  data.concat(FPSTR(html_controlQuick1));
  data.concat(FPSTR(html_controlQuick1a));
  if (mountStatus.mountType()==MT_GEM) data.concat(FPSTR(html_controlQuick2));
  data.concat(FPSTR(html_controlQuick3));
  data.concat(FPSTR(html_controlQuick4));
  data.concat(FPSTR(html_controlQuick5));
  sendHtml(data);

  // Tracking control ----------------------------------------
  data.concat(FPSTR(html_controlTrack1));
  data.concat(FPSTR(html_controlTrack2));
  data.concat(FPSTR(html_controlTrack3));
  data.concat(FPSTR(html_controlTrack4));
  sendHtml(data);

  // Get the align mode --------------------------------------
  data.concat(FPSTR(html_controlAlign1));
  byte sc[3];
  int n=1;
  if (mountStatus.alignMaxStars()<3) { n=1; sc[0]=1; } else
  if (mountStatus.alignMaxStars()<4) { n=3; sc[0]=1; sc[1]=2; sc[2]=3; } else
  if (mountStatus.alignMaxStars()<6) { n=3; sc[0]=1; sc[1]=3; sc[2]=4; } else
  if (mountStatus.alignMaxStars()<8) { n=3; sc[0]=1; sc[1]=3; sc[2]=6; } else
                                     { n=3; sc[0]=1; sc[1]=3; sc[2]=9; }
  for (int i=0; i<n; i++) { char temp2[120]=""; sprintf_P(temp2,html_controlAlign2,i+1,sc[i],sc[i],SIDEREAL_CH); data.concat(temp2); }
  data.concat(FPSTR(html_controlAlign3));
  if (mountStatus.mountType() != MT_ALTAZM) {
    data.concat(FPSTR(html_controlAlign4));
  }
  sendHtml(data);
  
  // Tracking ------------------------------------------------
  data.concat(FPSTR(html_controlTrack5));

  // Guiding -------------------------------------------------
  data.concat(FPSTR(html_controlGuide1));
  data.concat(FPSTR(html_controlGuide2));
  data.concat(FPSTR(html_controlGuide3));
  data.concat(FPSTR(html_controlGuide4));
  sendHtml(data);
  data.concat(FPSTR(html_controlGuide5));
  data.concat(FPSTR(html_controlGuide6));
  data.concat(FPSTR(html_controlGuide7));
  sendHtml(data);

  // Focusing ------------------------------------------------
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
  if (focuserCount > 0) {
    data.concat(FPSTR(html_controlFocusBeg));
    data.concat("<div style='float: left;'>" L_FOCUSER ":</div><div style='float: right; text-align: right;' id='focuserpos'>?</div><br />");
    if (focuserCount > 1) {
      if (focuserPresent[0]) data.concat(FPSTR(html_selectFocuser1));
      if (focuserPresent[1]) data.concat(FPSTR(html_selectFocuser2));
      if (focuserPresent[2]) data.concat(FPSTR(html_selectFocuser3));
      if (focuserPresent[3]) data.concat(FPSTR(html_selectFocuser4));
      if (focuserPresent[4]) data.concat(FPSTR(html_selectFocuser5));
      if (focuserPresent[5]) data.concat(FPSTR(html_selectFocuser6));
    }
    data.concat(FPSTR(html_setFocus1));
    data.concat(FPSTR(html_setFocus2));
    data.concat("<br />");
    data.concat(FPSTR(html_controlFocus1));
    data.concat(FPSTR(html_controlFocus2));
    data.concat(FPSTR(html_controlFocus3));
    data.concat(FPSTR(html_controlFocusEnd));
    sendHtml(data);
  }

  // Rotate/De-Rotate ----------------------------------------
  rotator = false;
  deRotator = false;
  if (onStep.command(":GX98#", temp)) {
    if (temp[0] == 'R') rotator = true;
    if (temp[0] == 'D') { rotator = true; deRotator = true; }
  }
  if (rotator) {
    data.concat(FPSTR(html_controlRotateBeg));
    data.concat("<div style='float: left;'>" L_ROTATOR ":</div><div style='float: right; text-align: right;' id='rotatorpos'>?</div><br />");
    data.concat(FPSTR(html_setRotate1));
    data.concat(FPSTR(html_setRotate2));
    data.concat("<br />");
    data.concat(FPSTR(html_controlRotate1));
    data.concat(FPSTR(html_controlRotate2));
    data.concat(FPSTR(html_controlRotate3));
    sendHtml(data);
    if (deRotator) {
      data.concat(FPSTR(html_controlDeRotate1));
      data.concat(FPSTR(html_controlDeRotate2));
      sendHtml(data);
    }
    data.concat(FPSTR(html_controlRotateEnd));
    sendHtml(data);
  }

  data.concat(FPSTR(html_controlEnd));

  // collapsible script
  data.concat(FPSTR(html_collapseScript));
  
  data.concat("</div></body></html>");

  sendHtml(data);
  sendHtmlDone();
}

void controlAjaxGet() {
  processControlGet();
  #if OPERATIONAL_MODE != WIFI
    www.sendContent("");
  #else
    www.send(200, "text/html", "");
  #endif
}

void controlAjax() {
  String data = "";
  char temp[120] = "";

  mountStatus.update();
  if (mountStatus.valid()) {
    if (mountStatus.tracking()) {
      data.concat("trk_on|disabled\n");
      data.concat("trk_off|enabled\n");
    } else {
      data.concat("trk_on|enabled\n");
      data.concat("trk_off|disabled\n");
    }
    if (mountStatus.atHome() || mountStatus.parked()) {
      data.concat("park|disabled\n");
      data.concat("unpark|enabled\n");
    } else {
      if (mountStatus.parkFail() || mountStatus.parking()) data.concat("park|disabled\n"); else data.concat("park|enabled\n");
      data.concat("unpark|disabled\n");
    }
    if (mountStatus.inGoto()) {
      data.concat("c_goto|enabled\n");
    } else {
      data.concat("c_goto|disabled\n");
    }
    if (mountStatus.tracking()) {
      data.concat("alg1|disabled\n");
      data.concat("alg2|disabled\n");
      data.concat("alg3|disabled\n");
      if (mountStatus.aligning()) {
        data.concat("alga|enabled\n");
      } else {
        data.concat("alga|disabled\n");
      }
    } else {
      if (!mountStatus.parked() && mountStatus.atHome()) {
        data.concat("alg1|enabled\n");
        data.concat("alg2|enabled\n");
        data.concat("alg3|enabled\n");
        data.concat("alga|disabled\n");
      } else {
        data.concat("alg1|disabled\n");
        data.concat("alg2|disabled\n");
        data.concat("alg3|disabled\n");
        data.concat("alga|disabled\n");
      }
    }
    if (mountStatus.tracking() && !mountStatus.aligning()) {
      data.concat("rpa|enabled\n");
    } else {
      data.concat("rpa|disabled\n");
    }
  } else {
    data.concat("trk_on|disabled\n");
    data.concat("trk_off|disabled\n");
    data.concat("park|disabled\n");
    data.concat("unpark|disabled\n");
    data.concat("c_goto|disabled\n");
    data.concat("alg1|disabled\n");
    data.concat("alg2|disabled\n");
    data.concat("alg3|disabled\n");
    data.concat("alga|disabled\n");
    data.concat("rpa|disabled\n");
  }

  if (focuserCount > 0) {
    data.concat("focuserpos|");
    if (onStep.command(":FG#", temp)) { data.concat(temp); data.concat(" microns\n"); } else { data.concat("?\n"); }

    if (mountStatus.getVersionMajor() >= 10) {
      if (onStep.command(":FA#", temp)) {
        if (temp[0] == '1' && temp[1] == 0) data.concat("foc1_sel|disabled\n"); else data.concat("foc1_sel|enabled\n");
        if (temp[0] == '2' && temp[1] == 0) data.concat("foc2_sel|disabled\n"); else data.concat("foc2_sel|enabled\n");
        if (temp[0] == '3' && temp[1] == 0) data.concat("foc3_sel|disabled\n"); else data.concat("foc3_sel|enabled\n");
        if (temp[0] == '4' && temp[1] == 0) data.concat("foc4_sel|disabled\n"); else data.concat("foc4_sel|enabled\n");
        if (temp[0] == '5' && temp[1] == 0) data.concat("foc5_sel|disabled\n"); else data.concat("foc5_sel|enabled\n");
        if (temp[0] == '6' && temp[1] == 0) data.concat("foc6_sel|disabled\n"); else data.concat("foc6_sel|enabled\n");
      }
    } else {
        data.concat("foc1_sel|enabled\n");
        if (focuserCount > 1) data.concat("foc2_sel|enabled\n");
    }
  }

  if (rotator) {
    data.concat("rotatorpos|");
    if (onStep.command(":rG#", temp)) { temp[9]=temp[5]; temp[10]=temp[6]; temp[11]=0; temp[4]='&'; temp[5]='d'; temp[6]='e'; temp[7]='g'; temp[8]=';'; data.concat(temp); data.concat("&#39;\n"); } else { data.concat("?\n"); }
  }

  sendHtml(data);
  sendHtmlDone();
}

int get_temp_month;
int get_temp_day;
int get_temp_year;
int get_temp_hour;
int get_temp_minute;
int get_temp_second;

void processControlGet() {
  String v;
  int16_t i;

  // Quick bar
  v = www.arg("qb");
  if (!v.equals(EmptyStr)) {
    if (v.equals("q")) onStep.commandBlind(":Q#");       // stop goto/guide
    if (v.equals("co")) onStep.commandBool(":SX99,1#");  // meridian flip, pause->continue
    if (v.equals("hf")) onStep.commandBlind(":hF#");     // home, reset
    if (v.equals("hc")) onStep.commandBlind(":hC#");     // home, find
    if (v.equals("pk")) onStep.commandBool(":hP#");      // park
    if (v.equals("pu")) onStep.commandBool(":hR#");      // un-park
  }

  // Align
  v = www.arg("al");
  if (!v.equals(EmptyStr)) {
    if (v.equals("1")) onStep.commandBool(":A1#");
    if (v.equals("2")) onStep.commandBool(":A2#");
    if (v.equals("3")) onStep.commandBool(":A3#");
    if (v.equals("4")) onStep.commandBool(":A4#");
    if (v.equals("5")) onStep.commandBool(":A5#");
    if (v.equals("6")) onStep.commandBool(":A6#");
    if (v.equals("7")) onStep.commandBool(":A7#");
    if (v.equals("8")) onStep.commandBool(":A8#");
    if (v.equals("9")) onStep.commandBool(":A9#");
    if (v.equals("n")) onStep.commandBool(":A+#");
    if (v.equals("q")) onStep.commandBlind(":Q#");
  }

  // Set DATE/TIME
  v = www.arg("dm");
  if (!v.equals(EmptyStr)) {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 11)) get_temp_month = i + 1;
  }
  v = www.arg("dd");
  if (!v.equals(EmptyStr)) {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 1 && i <= 31)) get_temp_day = i;
  }
  v = www.arg("dy");
  if (!v.equals(EmptyStr)) {
    if ( (convert.atoi2((char *)v.c_str(), &i)) && (i >= 2016 && i <= 9999)) {
      get_temp_year = i - 2000;
      char temp[16];
      sprintf(temp, ":SC%02d/%02d/%02d#", get_temp_month, get_temp_day, get_temp_year);
      onStep.commandBool(temp);
    }
  }
  v = www.arg("th");
  if (!v.equals(EmptyStr)) {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 23)) get_temp_hour = i;
  }
  v = www.arg("tm");
  if (!v.equals(EmptyStr)) {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 59)) get_temp_minute = i;
  }
  v = www.arg("ts");
  if (!v.equals(EmptyStr)) {
    if (convert.atoi2((char *)v.c_str(), &i) && (i >= 0 && i <= 59)) {
      get_temp_second = i;
      char temp[20];
      sprintf(temp, ":SL%02d:%02d:%02d#", get_temp_hour, get_temp_minute, get_temp_second);
      onStep.commandBool(temp);
    }
  }

  v = www.arg("dr");
  if (!v.equals(EmptyStr)) {
    // Tracking control
    if (v.equals("T1")) onStep.commandBool(":Te#");     // enable tracking
    if (v.equals("T0")) onStep.commandBool(":Td#");     // disable tracking
    if (v.equals("Ts")) onStep.commandBlind(":TQ#");    // sidereal
    if (v.equals("Tl")) onStep.commandBlind(":TL#");    // lunar
    if (v.equals("Th")) onStep.commandBlind(":TS#");    // solar

    // quick
    if (v.equals("qq")) onStep.commandBlind(":Q#");     // stop goto/guide
    if (v.equals("qc")) onStep.commandBool(":SX99,1#"); // meridian flip, pause->continue
    if (v.equals("qr")) onStep.commandBlind(":hF#");    // home, reset
    if (v.equals("qh")) onStep.commandBlind(":hC#");    // home, find
    if (v.equals("pk")) onStep.commandBool(":hP#");     // park
    if (v.equals("pu")) onStep.commandBool(":hR#");     // un-park

    // GUIDE control direction
    if (v.equals("n1")) onStep.commandBlind(":Mn#");    // move n... s,e,w
    if (v.equals("s1")) onStep.commandBlind(":Ms#");
    if (v.equals("e1")) onStep.commandBlind(":Me#");
    if (v.equals("w1")) onStep.commandBlind(":Mw#");

    if (v.equals("n0")) onStep.commandBlind(":Qn#");    // quit n... s,e,w
    if (v.equals("s0")) onStep.commandBlind(":Qs#");
    if (v.equals("e0")) onStep.commandBlind(":Qe#");
    if (v.equals("w0")) onStep.commandBlind(":Qw#");

    if (v.equals("sy")) onStep.commandBool(":CS#");     // sync

    // GUIDE control rate
    if (v.equals("R0")) onStep.commandBlind(":R0#");    // guide rate 0..9
    if (v.equals("R1")) onStep.commandBlind(":R1#");
    if (v.equals("R2")) onStep.commandBlind(":R2#");
    if (v.equals("R3")) onStep.commandBlind(":R3#");
    if (v.equals("R4")) onStep.commandBlind(":R4#");
    if (v.equals("R5")) onStep.commandBlind(":R5#");
    if (v.equals("R6")) onStep.commandBlind(":R6#");
    if (v.equals("R7")) onStep.commandBlind(":R7#");
    if (v.equals("R8")) onStep.commandBlind(":R8#");
    if (v.equals("R9")) onStep.commandBlind(":R9#");

    // Rotate/De-Rotate
    if (v.equals("rB")) onStep.commandBlind(":r3#:rc#:r<#"); // rate 3, move ccw
    if (v.equals("rr")) onStep.commandBlind(":r1#:rc#:r<#"); // rate 1, move ccw
    if (v.equals("rf")) onStep.commandBlind(":r1#:rc#:r>#"); // rate 1, move cw
    if (v.equals("rF")) onStep.commandBlind(":r3#:rc#:r>#"); // rate 3, move cw
    if (v.equals("rH")) onStep.commandBlind(":rF#");         // reset rotator at home
    if (v.equals("rh")) onStep.commandBlind(":rC#");         // move rotator to home
    if (v.equals("r0")) onStep.commandBlind(":r-#");         // disable de-rotator
    if (v.equals("r1")) onStep.commandBlind(":r+#");         // enable de-rotator
    if (v.equals("rR")) onStep.commandBlind(":rR#");         // reverse rotator
    if (v.equals("rp")) onStep.commandBlind(":rP#");         // move rotator to parallactic angle
    if (v.equals("rq")) onStep.commandBlind(":rQ#");

    // Focuser
    if (v.equals("F1")) onStep.commandBool(":FA1#");      // select focuser 1
    if (v.equals("F2")) onStep.commandBool(":FA2#");      // select focuser 2
    if (v.equals("F3")) onStep.commandBool(":FA3#");      // select focuser 3
    if (v.equals("F4")) onStep.commandBool(":FA4#");      // select focuser 4
    if (v.equals("F5")) onStep.commandBool(":FA5#");      // select focuser 5
    if (v.equals("F6")) onStep.commandBool(":FA6#");      // select focuser 6
    if (v.equals("FH")) onStep.commandBlind(":FH#");      // reset focuser at home (half travel)
    if (v.equals("Fh")) onStep.commandBlind(":Fh#");      // move focuser to home (half travel)
    if (v.equals("FI")) onStep.commandBlind(":F4#:F-#");  // rate fast, move in
    if (v.equals("Fi")) onStep.commandBlind(":F2#:F-#");  // rate slow, move in
    if (v.equals("Fo")) onStep.commandBlind(":F2#:F+#");  // rate slow, move out
    if (v.equals("FO")) onStep.commandBlind(":F4#:F+#");  // rate fast, move out
    if (v.equals("Fq")) onStep.commandBlind(":FQ#");
  }

  // Set the rotator position
  v = www.arg("rs");
  if (!v.equals(EmptyStr)) {
    double f = v.toFloat();
    if (f >= -360.0 || f <= 360.0) {
      char temp[80], temp1[40];
      convert.doubleToDms(temp1, f, true, true, PM_HIGH);
      sprintf(temp, ":rS%s#", temp1);
      onStep.commandBool(temp);
    }
  }

  // Set the focuser position
  v = www.arg("fs");
  if (!v.equals(EmptyStr)) {
    int p = v.toInt();
    if (p >= -500000 || p <= 500000) {
      char temp[80];
      sprintf(temp, ":FS%d#", p);
      onStep.commandBool(temp);
    }
  }

  // refine polar align
  v = www.arg("rp");
  if (!v.equals(EmptyStr)) {
    if (v.equals("a")) onStep.commandBool(":MP#");
  }

}
