// -----------------------------------------------------------------------------------
// Library

#include "Library.h"

void processLibraryGet();

int currentCatalog = 0;
char currentCatName[12] = "";
bool catalogIndexChanged = false;
bool downloadCatalogData = false;
bool uploadCatalogData = false;
String showMessage = "";

void handleLibrary() {
  char temp[400] = "";
  char temp1[120] = "";

  currentCatalog = 0;
  SERIAL_ONSTEP.setTimeout(webTimeout);
  onStep.serialRecvFlush();

  processLibraryGet();

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
  data.concat(FPSTR(html_linksLibS));
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

  // Scripts
  sprintf_P(temp, html_ajaxScript, "libraryA.txt"); data.concat(temp);
  data.concat(FPSTR(html_libStatusScript));

  // active ajax page is: libraryAjax();
  data.concat("<script>var ajaxPage='library.txt';</script>\n");
  data.concat(FPSTR(html_ajax_active));
  data.concat("<script>auto2Rate=2;</script>");
  sendHtml(data);

  // OnStep wasn't found, show warning and info.
  if (!status.valid) { data.concat(FPSTR(html_bad_comms_message)); sendHtml(data); sendHtmlDone(); return; }

  data.concat(FPSTR(html_libCatalogSelect1));
  sendHtml(data);
  data.concat(FPSTR(html_libCatalogSelect2));
  sendHtml(data);
  data.concat(FPSTR(html_libSubmitCatalog));
  sendHtml(data);
  data.concat(FPSTR(html_libShowMessage));
  data.concat(FPSTR(html_libEditCatalog));
  sendHtml(data);
  data.concat(FPSTR(html_libCatalogForm1));
  sendHtml(data);
  data.concat(FPSTR(html_libCatalogForm2));

  data.concat("</div></body></html>");

  sendHtml(data);
  sendHtmlDone();
}

void libraryAjaxGet() {
  sendTextStart();
  processLibraryGet();
  sendTextDone();
}

void libraryAjax() {
  String data = "";

  sendTextStart();

  data.concat("libFree|"); data.concat(onStep.commandString(":L?#")); data+="\n";

  if (showMessage != "") {
    data.concat("message|" + showMessage + "\n");
    showMessage="";
  }
  
  if (catalogIndexChanged) {
    if (currentCatalog == 0) data.concat("clr_btn|" L_CAT_CLEAR_LIB "\n"); else data.concat("clr_btn|" L_CAT_CLEAR "\n");
    data.concat("up|disabled\n");
    strcpy(currentCatName,"");
    if (currentCatalog != 0) {
      if (onStep.commandBool(":L$#")) {
        char temp[80] = "";
        onStep.command(":LR#", temp);
        if (temp[0] == '$') {
          for (int i=1; i<10; i++) {
            currentCatName[i-1]=0; currentCatName[i]=0;
            if (temp[i] == ',' || temp[i] == 0 || temp[i] == '#') break;
            currentCatName[i-1]=temp[i];
          }
          if (currentCatName[0] == 0) {
            showMessage = F(L_CAT_NO_NAME);
          } else {
            data.concat("down|enabled\n");
            showMessage = L_CATALOG " "; showMessage += currentCatName; showMessage += " " L_SELECTED ".";
          }
        } else { data.concat("down|disabled\n"); showMessage = F(L_CAT_NO_NAME); }
      } else { data.concat("down|disabled\n"); showMessage = F(L_CAT_GET_NAME_FAIL); }
    } else { data.concat("down|disabled\n"); showMessage = F(L_CAT_NO_CAT); }
    data.concat("catData&\n");
    catalogIndexChanged=false;
  } else

  if (downloadCatalogData) {
    if (currentCatalog != 0) {
      data.concat("catData&");
      bool success = false;
      char temp[80] = "";
      sprintf(temp, ":Lo%ld#", (long)currentCatalog-1);
      if (onStep.commandBool(temp)) {
        data.concat("$");
        data.concat(currentCatName);
        data.concat("\r");

        while (true) {
          onStep.command(":LR#", temp);
          if (temp[0] == ',') { success=true; break; }
          if (temp[0] == 0)  break;

          // isolate the individual fields (and error check)
          int i = 0;
          String r = temp;
          String nameS = "";
          String catS = "";
          String raS = "";
          String decS = "";

          r = r + ",";
          i = r.indexOf(","); if (i >= 0) { nameS = r.substring(0, i); r = r.substring(i + 1); } else { success = false; break; }
          i = r.indexOf(","); if (i >= 0) { catS  = r.substring(0, i); r = r.substring(i + 1); } else { success = false; break; }
          i = r.indexOf(","); if (i >= 0) { raS   = r.substring(0, i); r = r.substring(i + 1); } else { success = false; break; }
          i = r.indexOf(","); if (i >= 0) { decS  = r.substring(0, i); r = r.substring(i + 1); } else { success = false; break; }

          // pad to the correct length (and error check)
          i = 11 - nameS.length(); if (i >= 0) { for (int j = 0; j < i; j++) nameS = nameS + " "; } else { success = false; break; }
          i = 3 - catS.length(); if (i >= 0) { for (int j = 0; j < i; j++) catS = catS + " "; } else { success = false; break; }

          r = nameS + "," + catS + "," + raS + "," + decS;

          data.concat(r);
          data.concat("\r");
        }
        if (!success) showMessage = F(L_CAT_DOWNLOAD_FAIL); else showMessage = F(L_CAT_DOWNLOAD_SUCCESS);
      } else showMessage = F(L_CAT_DOWNLOAD_INDEX_FAIL);
    } else showMessage = F(L_CAT_DOWNLOAD_INDEX_FAIL);
    data.concat("\n");

    downloadCatalogData = false;
  }

  sendText(data);
  sendTextDone();
}

void processLibraryGet() {
  String v;
  int i;
  char temp[40]="";

  // Catalog clear
  v = www.arg("cc");
  if (!v.equals(EmptyStr)) {
    if (currentCatalog >= 0 && currentCatalog < 16) {
      if (currentCatalog == 0) {
        // clear library
        onStep.commandBool(":Lo0#");
        onStep.commandBlind(":L!#");
      } else {
        // clear this catalog
        sprintf(temp,":Lo%ld#",(long)currentCatalog-1);
        onStep.commandBool(temp);
        onStep.commandBlind(":LL#");
      }
      catalogIndexChanged=true;
      showMessage=L_CAT_DATA_REMOVED ".";
    }
  }

  // Catalog download
  v = www.arg("cd");
  if (!v.equals(EmptyStr)) {
    if (currentCatalog > 0 && currentCatalog < 16) {
      sprintf(temp,":Lo%ld#",(long)currentCatalog-1);
      if (!onStep.commandBool(temp)) currentCatalog=0;
      downloadCatalogData=true;
    }
  }

  // Catalog upload
  // Object Name|Cat|---RA---|---Dec---
  // ccccccccccc,ccc,HH:MM:SS,sDD*MM:SS
  // NGC6813    ,DN ,19:41:08,+27*20:22
  v = www.arg("cu");
  if (!v.equals(EmptyStr)) {
    showMessage="";
    
    uploadCatalogData = true;
    if (currentCatalog > 0 && currentCatalog < 16) {
      sprintf(temp,":Lo%ld#",(long)currentCatalog-1);
      if (onStep.commandBool(temp)) {
        v.replace("_"," ");
        int lineNum = 0;
        if (v.equals("DELETE")) { v = ""; showMessage = L_CAT_DATA_REMOVED "."; }
        onStep.commandBlind(":LL#");  // clear this catalog
        while (v.length() > 0) { // any data left?
          lineNum++;
          String line,co,cat,ra,de;
          i=v.indexOf("\n"); if (i >= 0) { line = v.substring(0,i); v=v.substring(i+1); } else { line = v; v=""; }

          // catalog name?
          if (lineNum == 1) {
            line.trim();
            if (line.charAt(0) == '$') {
              co = line.substring(0);
              co.trim();
              if (co.length() < 2 || co.length() > 11) { showMessage=F(L_CAT_UPLOAD_FAIL); break; }
              if (!onStep.commandBool(":L$#")) { showMessage=F(L_CAT_UPLOAD_INDEX_FAIL); break; }
              if (!onStep.commandBool(":LD#")) { showMessage=F(L_CAT_DELETE_FAIL); break; }
              if (!onStep.commandBool((":LW"+co+"#").c_str())) { showMessage=F(L_CAT_WRITE_NAME_FAIL); break; }
              continue;
            } else { showMessage=L_CAT_UPLOAD_NO_NAME_FAIL; break; }
          }
          
          i = line.indexOf(","); if (i >= 0) { co  = line.substring(0,i); line=line.substring(i+1); } else { showMessage=F(L_CAT_BAD_FORM); showMessage+=String(lineNum); break; }
          i = line.indexOf(","); if (i >= 0) { cat = line.substring(0,i); line=line.substring(i+1); } else { showMessage=F(L_CAT_BAD_FORM); showMessage+=String(lineNum); break; }
          i = line.indexOf(","); if (i >= 0) { ra  = line.substring(0,i); line=line.substring(i+1); } else { showMessage=F(L_CAT_BAD_FORM); showMessage+=String(lineNum); break; }
          de = line;
          
          co.trim(); cat.trim(); ra.trim(); de.trim();
          if (co.length() < 1 || co.length() > 11) { showMessage=F(L_CAT_UPLOAD_BAD_OBJECT_NAME); showMessage+=String(lineNum); break; }
          if (cat != "UNK" && cat != "OC"  && cat != "GC"  && cat != "PN" && 
              cat != "DN"  && cat != "SG"  && cat != "EG"  && cat != "IG" &&
              cat != "KNT" && cat != "SNR" && cat != "GAL" && cat != "CN" &&
              cat != "STR" && cat != "PLA" && cat != "CMT" && cat != "AST") { showMessage=F(L_CAT_BAD_CATEGORY); showMessage+=String(lineNum); break; }

          if (!isDigit(ra.charAt(0)) || !isDigit(ra.charAt(1)) || 
              !isDigit(ra.charAt(3)) || !isDigit(ra.charAt(4)) ||
              !isDigit(ra.charAt(6)) || !isDigit(ra.charAt(7)) ||
              ra.charAt(2) != ':' || ra.charAt(5) != ':' || ra.length() != 8)  { showMessage=F(L_CAT_BAD_RA); showMessage+=String(lineNum); break; }
              
          if (!isDigit(de.charAt(1)) || !isDigit(de.charAt(2)) || 
              !isDigit(de.charAt(4)) || !isDigit(de.charAt(5)) ||
              !isDigit(de.charAt(7)) || !isDigit(de.charAt(8)) ||
              (de.charAt(0) != '+' && de.charAt(0) != '-') ||
              (de.charAt(3) != '*' && de.charAt(3) != ':') ||
              de.charAt(6) != ':' || de.length() != 9) { showMessage=F(L_CAT_BAD_DEC); showMessage+=String(lineNum); break; }

          if (!onStep.commandBool((":Sr"+ra+"#").c_str())) {
            showMessage=F(L_CAT_UPLOAD_RA_FAIL);
            showMessage+=String(lineNum);
            break;
          }

          if (!onStep.commandBool((":Sd"+de+"#").c_str())) {
            showMessage=F(L_CAT_UPLOAD_DEC_FAIL);
            showMessage+=String(lineNum);
            break;
          }

          if (!onStep.commandBool((":LW"+co+","+cat+"#").c_str())) {
            showMessage=F(L_CAT_UPLOAD_LINE_FAIL);
            showMessage+=String(lineNum);
            break;
          }
        }
        if (showMessage=="") showMessage=L_CAT_UPLOAD_SUCCESS ", "+String(lineNum)+" " L_CAT_UPLOAD_LINES_WRITTEN ".";
      } else showMessage=F(L_CAT_UPLOAD_SELECT_FAIL);
    } else showMessage=F(L_CAT_UPLOAD_NO_CAT);
  }

  // Catalog index (1-15)
  v = www.arg("ci");
  if (!v.equals(EmptyStr)) {
    currentCatalog=v.toInt();
    if (currentCatalog > 0 && currentCatalog < 16) {
      sprintf(temp,":Lo%ld#",(long)currentCatalog-1);
      if (!onStep.commandBool(temp)) currentCatalog=0;
    } else currentCatalog=0;
    catalogIndexChanged=true;
  }
}
