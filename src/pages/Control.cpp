// -----------------------------------------------------------------------------------
// Telescope control related functions 

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../debug/Debug.h"

#include "../locales/Locale.h"
#include "../misc/Misc.h"
#include "../commands/Commands.h"
#include "../status/MountStatus.h"
#include "../wifiServers/WifiServers.h"
#include "../ethernetServers/EthernetServers.h"
#include "../encoders/Encoders.h"

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#include "Control.h"

void processControlGet();

boolean Focuser1;
boolean Focuser2;
boolean Rotate;
boolean DeRotate;

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500
void handleControl(EthernetClient *client) {
#else
void handleControl() {
#endif
  char temp[240] = "";

  Ser.setTimeout(webTimeout);
  serialRecvFlush();

  mountStatus.update(true);

  processControlGet();

  sendHtmlStart();
  
  String data=FPSTR(html_headB);
  data += FPSTR(html_main_cssB);
  data += FPSTR(html_main_css1);
  data += FPSTR(html_main_css2);
  data += FPSTR(html_main_css3);
  data += FPSTR(html_main_css4);
  data += FPSTR(html_main_css5);
  data += FPSTR(html_main_css6);
  sendHtml(data);
  data += FPSTR(html_main_css7);
  data += FPSTR(html_main_css8);
  data += FPSTR(html_main_css_control1);
  data += FPSTR(html_main_css_control2);
  data += FPSTR(html_main_css_control3);
  data += FPSTR(html_main_css_btns1);
  sendHtml(data);
  data += FPSTR(html_main_css_btns2);
  data += FPSTR(html_main_css_btns3);
  data += FPSTR(html_main_css_collapse1);
  data += FPSTR(html_main_css_collapse2);
  data += FPSTR(html_main_cssE);
  data += FPSTR(html_headE);
  data += FPSTR(html_bodyB);
  sendHtml(data);

  // finish the standard http response header
  data += FPSTR(html_onstep_header1); data += "OnStep";
  data += FPSTR(html_onstep_header2);
  if (mountStatus.getVersionStr(temp)) data += temp; else data += "?";
  data += FPSTR(html_onstep_header3);
  data += FPSTR(html_linksStatN);
  data += FPSTR(html_linksCtrlS);
  if (mountStatus.featureFound()) data += FPSTR(html_linksAuxN);
  data += FPSTR(html_linksLibN);
  #if ENCODERS == ON
    data += FPSTR(html_linksEncN);
  #endif
  sendHtml(data);
  data += FPSTR(html_linksPecN);
  data += FPSTR(html_linksSetN);
  data += FPSTR(html_linksCfgN);
  data += FPSTR(html_linksSetupN);
  data += FPSTR(html_onstep_header4);
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!mountStatus.valid()) { data+= FPSTR(html_bad_comms_message); sendHtml(data); sendHtmlDone(data); return; }

  // scripts
  sprintf_P(temp, html_ajaxScript, "controlA.txt"); data += temp;
  data += FPSTR(html_ajaxScriptShort);
  data += FPSTR(html_dateTimeScriptA);
  data += FPSTR(html_dateTimeScriptB);
  data += FPSTR(html_dateTimeScriptC);

  // active ajax page is: controlAjax();
  data +="<script>var ajaxPage='control.txt';</script>\n";
  data +=FPSTR(html_ajax_active);
  sendHtml(data);

  // Quick controls ------------------------------------------
  data += FPSTR(html_controlQuick1);
  data += FPSTR(html_controlQuick1a);
  if (mountStatus.mountType()==MT_GEM) data += FPSTR(html_controlQuick2);
  data += FPSTR(html_controlQuick3);
  data += FPSTR(html_controlQuick4);
  data += FPSTR(html_controlQuick5);
  sendHtml(data);

  // Tracking control ----------------------------------------
  data += FPSTR(html_controlTrack1);
  data += FPSTR(html_controlTrack2);
  data += FPSTR(html_controlTrack3);
  data += FPSTR(html_controlTrack4);
  sendHtml(data);

  // Get the align mode --------------------------------------
  data += FPSTR(html_controlAlign1);
  byte sc[3];
  int n=1;
  if (mountStatus.alignMaxStars()<3) { n=1; sc[0]=1; } else
  if (mountStatus.alignMaxStars()<4) { n=3; sc[0]=1; sc[1]=2; sc[2]=3; } else
  if (mountStatus.alignMaxStars()<6) { n=3; sc[0]=1; sc[1]=3; sc[2]=4; } else
  if (mountStatus.alignMaxStars()<8) { n=3; sc[0]=1; sc[1]=3; sc[2]=6; } else
                                     { n=3; sc[0]=1; sc[1]=3; sc[2]=9; }
  for (int i=0; i<n; i++) { char temp2[120]=""; sprintf_P(temp2,html_controlAlign2,i+1,sc[i],sc[i],SIDEREAL_CH); data+=temp2; }
  data += FPSTR(html_controlAlign3);
  if (mountStatus.mountType() != MT_ALTAZM) {
    data += FPSTR(html_controlAlign4);
  }
  sendHtml(data);
  
  // Tracking ------------------------------------------------
  data += FPSTR(html_controlTrack5);

  // Guiding -------------------------------------------------
  data += FPSTR(html_controlGuide1);
  data += FPSTR(html_controlGuide2);
  data += FPSTR(html_controlGuide3);
  data += FPSTR(html_controlGuide4);
  sendHtml(data);
  data += FPSTR(html_controlGuide5);
  data += FPSTR(html_controlGuide6);
  data += FPSTR(html_controlGuide7);
  sendHtml(data);

  // Focusing ------------------------------------------------
  if (commandBool(":FA#")) Focuser1=true; else Focuser1=false;
  if (commandBool(":fA#")) Focuser2=true; else Focuser2=false;
  if (Focuser1) {
    data += FPSTR(html_controlFocus1);
    data += "<div style='float: left;'>" L_FOCUSER ":</div><div style='float: right; text-align: right;' id='focuserpos'>?</div><br />";
    if (Focuser2) data += FPSTR(html_controlFocus2);
    data += FPSTR(html_controlFocus3);
    data += FPSTR(html_controlFocus4);
    data += FPSTR(html_controlFocus5);
    data += FPSTR(html_controlFocus6);
    sendHtml(data);
  }

  // Rotate/De-Rotate ----------------------------------------
  Rotate=false;
  DeRotate=false;
  if (command(":GX98#", temp)) {
    if (temp[0] == 'R') Rotate = true;
    if (temp[0] == 'D') { Rotate = true; DeRotate = true; }
  }
  if (Rotate) {
    data += FPSTR(html_controlRotate0);
    data += "<div style='float: left;'>" L_ROTATOR ":</div><div style='float: right; text-align: right;' id='rotatorpos'>?</div><br />";
    data += FPSTR(html_controlRotate1);
    data += FPSTR(html_controlRotate2);
    data += FPSTR(html_controlRotate3);
    sendHtml(data);
  }
  if (DeRotate) {
    data += FPSTR(html_controlDeRotate1);
    data += FPSTR(html_controlDeRotate2);
    sendHtml(data);
  }
  if (Rotate) {
    data += FPSTR(html_controlRotate4);
    sendHtml(data);
  }

  data += FPSTR(html_controlEnd);

  // collapsible script
  data += FPSTR(html_collapseScript);
  
  data += "</div></body></html>";

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
  String data="";
  char temp[120]="";

  mountStatus.update();
  if (mountStatus.valid()) {
    if (mountStatus.tracking()) {
      data += "trk_on|";  data+="disabled"; data+="\n";
      data += "trk_off|"; data+="enabled"; data+="\n";
    } else {
      data += "trk_on|";  data+="enabled"; data+="\n";
      data += "trk_off|"; data+="disabled"; data+="\n";
    }
    if (mountStatus.atHome() || mountStatus.parked()) {
      data += "park|";   data+="disabled"; data+="\n";
      data += "unpark|"; data+="enabled"; data+="\n";
    } else {
      data += "park|";   data+="enabled"; data+="\n";
      data += "unpark|"; data+="disabled"; data+="\n";
    }
    if (mountStatus.slewing()) {
      data += "c_goto|"; data+="enabled"; data+="\n";
    } else {
      data += "c_goto|"; data+="disabled"; data+="\n";
    }
    if (mountStatus.tracking()) {
      data += "alg1|"; data+="disabled"; data+="\n";
      data += "alg2|"; data+="disabled"; data+="\n";
      data += "alg3|"; data+="disabled"; data+="\n";
      if (mountStatus.aligning()) {
        data += "alga|"; data+="enabled"; data+="\n";
      } else {
        data += "alga|"; data+="disabled"; data+="\n";
      }
    } else {
      if (!mountStatus.parked() && mountStatus.atHome()) {
        data += "alg1|"; data+="enabled"; data+="\n";
        data += "alg2|"; data+="enabled"; data+="\n";
        data += "alg3|"; data+="enabled"; data+="\n";
        data += "alga|"; data+="disabled"; data+="\n";
      } else {
        data += "alg1|"; data+="disabled"; data+="\n";
        data += "alg2|"; data+="disabled"; data+="\n";
        data += "alg3|"; data+="disabled"; data+="\n";
        data += "alga|"; data+="disabled"; data+="\n";
      }
    }
    if (mountStatus.tracking() && !mountStatus.aligning()) {
      data += "rpa|"; data+="enabled"; data+="\n";
    } else {
      data += "rpa|"; data+="disabled"; data+="\n";
    }
  } else {
    data += "trk_on|";  data+="disabled"; data+="\n";
    data += "trk_off|"; data+="disabled"; data+="\n";
    data += "park|";    data+="disabled"; data+="\n";
    data += "unpark|";  data+="disabled"; data+="\n";
    data += "c_goto|";  data+="disabled"; data+="\n";
    data += "alg1|";    data+="disabled"; data+="\n";
    data += "alg2|";    data+="disabled"; data+="\n";
    data += "alg3|";    data+="disabled"; data+="\n";
    data += "alga|";    data+="disabled"; data+="\n";
    data += "rpa|";     data+="disabled"; data+="\n";
  }

  if (Focuser1) {
    data += "focuserpos|";
    if (command(":FG#",temp)) { data += temp; data += " microns\n"; } else { data += "?\n"; }
  }

  if (Rotate) {
    data += "rotatorpos|";
    if (command(":rG#",temp)) { temp[9]=temp[5]; temp[10]=temp[6]; temp[11]=0; temp[4]='&'; temp[5]='d'; temp[6]='e'; temp[7]='g'; temp[8]=';'; data += temp; data += "&#39;\n"; } else { data += "?\n"; }
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
    if (v=="q") commandBlind(":Q#");       // stop goto/guide
    if (v=="co") commandBool(":SX99,1#");  // meridian flip, pause->continue
    if (v=="hf") commandBlind(":hF#");     // home, reset
    if (v=="hc") commandBlind(":hC#");     // home, find
    if (v=="pk") commandBool(":hP#");      // park
    if (v=="pu") commandBool(":hR#");      // un-park
  }

  // Align
  v=server.arg("al");
  if (!v.equals(EmptyStr)) {
    if (v=="1") commandBool(":A1#");
    if (v=="2") commandBool(":A2#");
    if (v=="3") commandBool(":A3#");
    if (v=="4") commandBool(":A4#");
    if (v=="5") commandBool(":A5#");
    if (v=="6") commandBool(":A6#");
    if (v=="7") commandBool(":A7#");
    if (v=="8") commandBool(":A8#");
    if (v=="9") commandBool(":A9#");
    if (v=="n") commandBool(":A+#");
    if (v=="q") commandBlind(":Q#");
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
    if (v=="T1") commandBool(":Te#");     // enable tracking
    if (v=="T0") commandBool(":Td#");     // disable tracking
    if (v=="Ts") commandBlind(":TQ#");    // sidereal
    if (v=="Tl") commandBlind(":TL#");    // lunar
    if (v=="Th") commandBlind(":TS#");    // solar

    // quick
    if (v=="qq") commandBlind(":Q#");     // stop goto/guide
    if (v=="qc") commandBool(":SX99,1#"); // meridian flip, pause->continue
    if (v=="qr") commandBlind(":hF#");    // home, reset
    if (v=="qh") commandBlind(":hC#");    // home, find
    if (v=="pk") commandBool(":hP#");     // park
    if (v=="pu") commandBool(":hR#");     // un-park

    // GUIDE control direction
    if (v=="n1") commandBlind(":Mn#");    // move n... s,e,w
    if (v=="s1") commandBlind(":Ms#");
    if (v=="e1") commandBlind(":Me#");
    if (v=="w1") commandBlind(":Mw#");

    if (v=="n0") commandBlind(":Qn#");    // quit n... s,e,w
    if (v=="s0") commandBlind(":Qs#");
    if (v=="e0") commandBlind(":Qe#");
    if (v=="w0") commandBlind(":Qw#");

    if (v=="sy") commandBool(":CS#");     // sync

    // GUIDE control rate
    if (v=="R0") commandBlind(":R0#");    // guide rate 0..9
    if (v=="R1") commandBlind(":R1#");
    if (v=="R2") commandBlind(":R2#");
    if (v=="R3") commandBlind(":R3#");
    if (v=="R4") commandBlind(":R4#");
    if (v=="R5") commandBlind(":R5#");
    if (v=="R6") commandBlind(":R6#");
    if (v=="R7") commandBlind(":R7#");
    if (v=="R8") commandBlind(":R8#");
    if (v=="R9") commandBlind(":R9#");

    // Focuser
    if (v=="F1") commandBool(":FA1#");      // select focuser 1
    if (v=="F2") commandBool(":FA2#");      // select focuser 2
    if (v=="FH") commandBlind(":FH#");      // reset focuser at home (half travel)
    if (v=="Fh") commandBlind(":Fh#");      // move focuser to home (half travel)
    if (v=="FI") commandBlind(":FF#:F-#");  // rate fast, move in
    if (v=="Fi") commandBlind(":FS#:F-#");  // rate slow, move in
    if (v=="Fo") commandBlind(":FS#:F+#");  // rate fast, move out
    if (v=="FO") commandBlind(":FF#:F+#");  // rate slow, move out
    if (v=="Fq") commandBlind(":FQ#");
   
    // Rotate/De-Rotate
    if (v=="rB") commandBlind(":r3#:rc#:r<#"); // rate 3, move ccw
    if (v=="rr") commandBlind(":r1#:rc#:r<#"); // rate 1, move ccw
    if (v=="rf") commandBlind(":r1#:rc#:r>#"); // rate 1, move cw
    if (v=="rF") commandBlind(":r3#:rc#:r>#"); // rate 3, move cw
    if (v=="rH") commandBlind(":rF#");         // reset rotator at home
    if (v=="rh") commandBlind(":rC#");         // move rotator to home
    if (v=="r0") commandBlind(":r-#");         // disable de-rotator
    if (v=="r1") commandBlind(":r+#");         // enable de-rotator
    if (v=="rR") commandBlind(":rR#");         // reverse rotator
    if (v=="rp") commandBlind(":rP#");         // move rotator to parallactic angle
    if (v=="rq") commandBlind(":rQ#");
  }
  // refine polar align
  v=server.arg("rp");
  if (!v.equals(EmptyStr)) {
    if (v=="a") commandBool(":MP#");
  }

}
