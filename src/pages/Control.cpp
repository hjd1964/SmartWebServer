// -----------------------------------------------------------------------------------
// Telescope control related functions 

#include "Control.h"

void processControlGet();

int focuserCount = 0;
bool focuserPresent[6];
bool rotator;
bool deRotator;

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500
void handleControl(EthernetClient *client) {
#else
void handleControl() {
#endif
  char temp[240] = "";

  SERIAL_ONSTEP.setTimeout(webTimeout);
  serialRecvFlush();

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
  if (!mountStatus.valid()) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(data); return; }

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
    if (commandBool(":F1a#")) { focuserPresent[0] = true; focuserCount++; }
    if (commandBool(":F2a#")) { focuserPresent[1] = true; focuserCount++; }
    if (commandBool(":F3a#")) { focuserPresent[2] = true; focuserCount++; }
    if (commandBool(":F4a#")) { focuserPresent[3] = true; focuserCount++; }
    if (commandBool(":F5a#")) { focuserPresent[4] = true; focuserCount++; }
    if (commandBool(":F6a#")) { focuserPresent[5] = true; focuserCount++; }
  } else {
    if (commandBool(":FA#")) { focuserPresent[0] = true; focuserCount++; }
    if (commandBool(":fA#")) { focuserPresent[1] = true; focuserCount++; }
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
  if (command(":GX98#", temp)) {
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
  sendHtmlDone(data);
}

#if OPERATIONAL_MODE != WIFI
void controlAjaxGet(EthernetClient *client) {
#else
void controlAjaxGet() {
#endif
  processControlGet();
#if OPERATIONAL_MODE != WIFI
  client->print("");
#else
  server.send(200, "text/html","");
#endif
}

#if OPERATIONAL_MODE != WIFI
void controlAjax(EthernetClient *client) {
#else
void controlAjax() {
#endif
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
      data.concat("park|enabled\n");
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
    if (command(":FG#",temp)) { data.concat(temp); data.concat(" microns\n"); } else { data.concat("?\n"); }

    if (mountStatus.getVersionMajor() >= 10) {
      if (command(":FA#",temp)) {
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
    if (command(":rG#",temp)) { temp[9]=temp[5]; temp[10]=temp[6]; temp[11]=0; temp[4]='&'; temp[5]='d'; temp[6]='e'; temp[7]='g'; temp[8]=';'; data.concat(temp); data.concat("&#39;\n"); } else { data.concat("?\n"); }
  }

#if OPERATIONAL_MODE != WIFI
  client->print(data);
#else
  server.send(200, "text/plain",data);
#endif
}

int get_temp_month;
int get_temp_day;
int get_temp_year;
int get_temp_hour;
int get_temp_minute;
int get_temp_second;

void processControlGet() {
  String v;
  int i;

  // Quick bar
  v=server.arg("qb");
  if (!v.equals(EmptyStr)) {
    if (v.equals("q")) commandBlind(":Q#");       // stop goto/guide
    if (v.equals("co")) commandBool(":SX99,1#");  // meridian flip, pause->continue
    if (v.equals("hf")) commandBlind(":hF#");     // home, reset
    if (v.equals("hc")) commandBlind(":hC#");     // home, find
    if (v.equals("pk")) commandBool(":hP#");      // park
    if (v.equals("pu")) commandBool(":hR#");      // un-park
  }

  // Align
  v=server.arg("al");
  if (!v.equals(EmptyStr)) {
    if (v.equals("1")) commandBool(":A1#");
    if (v.equals("2")) commandBool(":A2#");
    if (v.equals("3")) commandBool(":A3#");
    if (v.equals("4")) commandBool(":A4#");
    if (v.equals("5")) commandBool(":A5#");
    if (v.equals("6")) commandBool(":A6#");
    if (v.equals("7")) commandBool(":A7#");
    if (v.equals("8")) commandBool(":A8#");
    if (v.equals("9")) commandBool(":A9#");
    if (v.equals("n")) commandBool(":A+#");
    if (v.equals("q")) commandBlind(":Q#");
  }

  // Set DATE/TIME
  v=server.arg("dm");
  if (!v.equals(EmptyStr)) {
    if ( (atoi2((char *)v.c_str(),&i)) && ((i>=0) && (i<=11))) { get_temp_month=i+1; }
  }
  v=server.arg("dd");
  if (!v.equals(EmptyStr)) {
    if ( (atoi2((char *)v.c_str(),&i)) && ((i>=1) && (i<=31))) { get_temp_day=i; }
  }
  v=server.arg("dy");
  if (!v.equals(EmptyStr)) {
    if ( (atoi2((char *)v.c_str(),&i)) && ((i>=2016) && (i<=9999))) {
      get_temp_year=i-2000;
      char temp[16];
      sprintf(temp,":SC%02d/%02d/%02d#",get_temp_month,get_temp_day,get_temp_year);
      commandBool(temp);
    }
  }
  v=server.arg("th");
  if (!v.equals(EmptyStr)) {
    if ( (atoi2((char *)v.c_str(),&i)) && ((i>=0) && (i<=23))) { get_temp_hour=i; }
  }
  v=server.arg("tm");
  if (!v.equals(EmptyStr)) {
    if ( (atoi2((char *)v.c_str(),&i)) && ((i>=0) && (i<=59))) { get_temp_minute=i; }
  }
  v=server.arg("ts");
  if (!v.equals(EmptyStr)) {
    if ( (atoi2((char *)v.c_str(),&i)) && ((i>=0) && (i<=59))) {
      get_temp_second=i;
      char temp[16];
      sprintf(temp,":SL%02d:%02d:%02d#",get_temp_hour,get_temp_minute,get_temp_second);
      commandBool(temp);
    }
  }

  v=server.arg("dr");
  if (!v.equals(EmptyStr)) {
    // Tracking control
    if (v.equals("T1")) commandBool(":Te#");     // enable tracking
    if (v.equals("T0")) commandBool(":Td#");     // disable tracking
    if (v.equals("Ts")) commandBlind(":TQ#");    // sidereal
    if (v.equals("Tl")) commandBlind(":TL#");    // lunar
    if (v.equals("Th")) commandBlind(":TS#");    // solar

    // quick
    if (v.equals("qq")) commandBlind(":Q#");     // stop goto/guide
    if (v.equals("qc")) commandBool(":SX99,1#"); // meridian flip, pause->continue
    if (v.equals("qr")) commandBlind(":hF#");    // home, reset
    if (v.equals("qh")) commandBlind(":hC#");    // home, find
    if (v.equals("pk")) commandBool(":hP#");     // park
    if (v.equals("pu")) commandBool(":hR#");     // un-park

    // GUIDE control direction
    if (v.equals("n1")) commandBlind(":Mn#");    // move n... s,e,w
    if (v.equals("s1")) commandBlind(":Ms#");
    if (v.equals("e1")) commandBlind(":Me#");
    if (v.equals("w1")) commandBlind(":Mw#");

    if (v.equals("n0")) commandBlind(":Qn#");    // quit n... s,e,w
    if (v.equals("s0")) commandBlind(":Qs#");
    if (v.equals("e0")) commandBlind(":Qe#");
    if (v.equals("w0")) commandBlind(":Qw#");

    if (v.equals("sy")) commandBool(":CS#");     // sync

    // GUIDE control rate
    if (v.equals("R0")) commandBlind(":R0#");    // guide rate 0..9
    if (v.equals("R1")) commandBlind(":R1#");
    if (v.equals("R2")) commandBlind(":R2#");
    if (v.equals("R3")) commandBlind(":R3#");
    if (v.equals("R4")) commandBlind(":R4#");
    if (v.equals("R5")) commandBlind(":R5#");
    if (v.equals("R6")) commandBlind(":R6#");
    if (v.equals("R7")) commandBlind(":R7#");
    if (v.equals("R8")) commandBlind(":R8#");
    if (v.equals("R9")) commandBlind(":R9#");

    // Rotate/De-Rotate
    if (v.equals("rB")) commandBlind(":r3#:rc#:r<#"); // rate 3, move ccw
    if (v.equals("rr")) commandBlind(":r1#:rc#:r<#"); // rate 1, move ccw
    if (v.equals("rf")) commandBlind(":r1#:rc#:r>#"); // rate 1, move cw
    if (v.equals("rF")) commandBlind(":r3#:rc#:r>#"); // rate 3, move cw
    if (v.equals("rH")) commandBlind(":rF#");         // reset rotator at home
    if (v.equals("rh")) commandBlind(":rC#");         // move rotator to home
    if (v.equals("r0")) commandBlind(":r-#");         // disable de-rotator
    if (v.equals("r1")) commandBlind(":r+#");         // enable de-rotator
    if (v.equals("rR")) commandBlind(":rR#");         // reverse rotator
    if (v.equals("rp")) commandBlind(":rP#");         // move rotator to parallactic angle
    if (v.equals("rq")) commandBlind(":rQ#");

    // Focuser
    if (v.equals("F1")) commandBool(":FA1#");      // select focuser 1
    if (v.equals("F2")) commandBool(":FA2#");      // select focuser 2
    if (v.equals("F3")) commandBool(":FA3#");      // select focuser 3
    if (v.equals("F4")) commandBool(":FA4#");      // select focuser 4
    if (v.equals("F5")) commandBool(":FA5#");      // select focuser 5
    if (v.equals("F6")) commandBool(":FA6#");      // select focuser 6
    if (v.equals("FH")) commandBlind(":FH#");      // reset focuser at home (half travel)
    if (v.equals("Fh")) commandBlind(":Fh#");      // move focuser to home (half travel)
    if (v.equals("FI")) commandBlind(":F4#:F-#");  // rate fast, move in
    if (v.equals("Fi")) commandBlind(":F2#:F-#");  // rate slow, move in
    if (v.equals("Fo")) commandBlind(":F2#:F+#");  // rate slow, move out
    if (v.equals("FO")) commandBlind(":F4#:F+#");  // rate fast, move out
    if (v.equals("Fq")) commandBlind(":FQ#");
  }

  // Set the rotator position
  v=server.arg("rs");
  if (!v.equals(EmptyStr)) {
    double f = v.toFloat();
    if (f >= -360.0 || f <= 360.0) {
      char temp[20], temp1[20];
      doubleToDms(temp1, f, true, true, PM_HIGH);
      sprintf(temp, ":rS%s#", temp1);
      commandBool(temp);
    }
  }

  // Set the focuser position
  v=server.arg("fs");
  if (!v.equals(EmptyStr)) {
    int p = v.toInt();
    if (p >= -500000 || p <= 500000) {
      char temp[20];
      sprintf(temp, ":FS%d#", p);
      commandBool(temp);
    }
  }

  // refine polar align
  v=server.arg("rp");
  if (!v.equals(EmptyStr)) {
    if (v.equals("a")) commandBool(":MP#");
  }

}
