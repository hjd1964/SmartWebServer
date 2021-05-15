#pragma once

const char html_bad_comms_message[] PROGMEM =
"<br /><bigger><font class=\"y\">" L_DOWN_TITLE "</font></bigger><br /><br />"
L_DOWN_MESSAGE1 L_DOWN_MESSAGE2
" <li>" L_DOWN_MESSAGE3 "</li><br />"
" <li>" L_DOWN_MESSAGE4 "</li><br />"
" <li>" L_DOWN_MESSAGE5 "</li><br />"
" <li>" L_DOWN_MESSAGE6 "</li><br />"
" <li>" L_DOWN_MESSAGE7 "</li><br />"
#if DISPLAY_RESET_CONTROLS != OFF
  "<br /><form method='get' action='/configuration.htm'>"
  "<button name='advanced' type='submit' value='reset' onclick=\"return confirm('" L_ARE_YOU_SURE "?');\" >" L_RESET "</button></form>\r\n"
#endif
"</ul></div><br class=\"clear\" />\r\n"
"</div></body></html>";

const char html_onstep_header1[] PROGMEM = "<div class='t'><table width='100%%'><tr><td><b><font size='5'>";
const char html_onstep_header2[] PROGMEM = "</font></b></td><td align='right'><b>Smart Web Server ";
const char html_onstep_header3[] PROGMEM = ")</b></td></tr></table>";
const char html_onstep_header4[] PROGMEM = "</div><div class='b'>\r\n";

const char html_linksStatS[] PROGMEM = "<a href='/index.htm' style='background-color: #552222;'>" L_PAGE_STATUS "</a>";
const char html_linksStatN[] PROGMEM = "<a href='/index.htm'>" L_PAGE_STATUS "</a>";
const char html_linksCtrlS[] PROGMEM = "<a href='/control.htm' style='background-color: #552222;'>" L_PAGE_CONTROL "</a>";
const char html_linksCtrlN[] PROGMEM = "<a href='/control.htm'>" L_PAGE_CONTROL "</a>";
const char html_linksAuxS[] PROGMEM = "<a href='/auxiliary.htm' style='background-color: #552222;'>" L_PAGE_AUX "</a>";
const char html_linksAuxN[] PROGMEM = "<a href='/auxiliary.htm'>" L_PAGE_AUX "</a>";
const char html_linksLibS[] PROGMEM = "<a href='/library.htm' style='background-color: #552222;'>" L_PAGE_LIBRARY "</a>";
const char html_linksLibN[] PROGMEM = "<a href='/library.htm'>" L_PAGE_LIBRARY "</a>";
const char html_linksEncS[] PROGMEM = "<a href='/enc.htm' style='background-color: #552222;'>" L_PAGE_ENCODERS "</a>";
const char html_linksEncN[] PROGMEM = "<a href='/enc.htm'>" L_PAGE_ENCODERS "</a>";
const char html_linksPecS[] PROGMEM = "<a href='/pec.htm' style='background-color: #552222;'>" L_PAGE_PEC "</a>";
const char html_linksPecN[] PROGMEM = "<a href='/pec.htm'>" L_PAGE_PEC "</a>";
const char html_linksSetS[] PROGMEM = "<a href='/settings.htm' style='background-color: #552222;'>" L_PAGE_SETTINGS "</a>";
const char html_linksSetN[] PROGMEM = "<a href='/settings.htm'>" L_PAGE_SETTINGS "</a>";
const char html_linksCfgS[] PROGMEM = "<a href='/configuration.htm' style='background-color: #552222;'>" L_PAGE_CONFIG "</a>";
const char html_linksCfgN[] PROGMEM = "<a href='/configuration.htm'>" L_PAGE_CONFIG "</a>";
const char html_linksSetupS[] PROGMEM = "<a href='/net.htm' style='background-color: #552222;'>" L_PAGE_SETUP "</a>";
const char html_linksSetupN[] PROGMEM = "<a href='/net.htm'>" L_PAGE_SETUP "</a>";
