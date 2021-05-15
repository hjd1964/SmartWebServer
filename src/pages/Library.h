// -----------------------------------------------------------------------------------
// Library
#pragma once

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

const char html_libStart[] PROGMEM =
"<form method='get' action='/lib.htm'>";

const char html_libCatalogSelect1[] PROGMEM =
L_CATALOGS " (<span id='libFree'>?"
"</span> " L_RECS_AVAIL ":)<br /><br />Number <select name='selBox' onchange=\"busy(); s('ci',this.value);\">"
"<option value='0' %s>-</option>"
"<option value='1' %s>1</option>"
"<option value='2' %s>2</option>"
"<option value='3' %s>3</option>"
"<option value='4' %s>4</option>"
"<option value='5' %s>5</option>";
const char html_libCatalogSelect2[] PROGMEM =
"<option value='6' %s>6</option>"
"<option value='7' %s>7</option>"
"<option value='8' %s>8</option>"
"<option value='9' %s>9</option>"
"<option value='10' %s>10</option>"
"<option value='11' %s>11</option>"
"<option value='12' %s>12</option>"
"<option value='13' %s>13</option>"
"<option value='14' %s>14</option>"
"<option value='15' %s>15</option>"
"</select>&nbsp;\n";

const char html_libSubmitCatalog[] PROGMEM =
"<button id='up' type='button' onclick=\"busy(); t('cu',document.getElementById('catData').value);\" disabled>" L_UPLOAD "</button> "
"<button id='down' type='button' onclick=\"busy(); s('cd',1);\" disabled>" L_DOWNLOAD "</button>&nbsp;&nbsp;&nbsp;"
"<button id='clr_btn' type='button' onclick=\"busy(); if (confirm('" L_ARE_YOU_SURE "?')) s('cc','clear')\">" L_CAT_CLEAR_LIB "</button><br /><br />\n";

const char html_libShowMessage[] PROGMEM =
"<div id='message' style='background-color: #222222; color: #aaaaaa; border: 1px solid #551111; width: 400px; padding: 2px;'>" L_NO_CATALOG ".</div><br />\n";

const char html_libEditCatalog[] PROGMEM =
L_CAT_DATA "<br />"
"<textarea id='catData' style='background-color: #222222; color: #ffffff; border: 1px solid #551111;' "
"rows='10' cols='40' oninput=\"document.getElementById('up').disabled=false;\"></textarea><br /><br />\n";

const char html_libCatalogForm1[] PROGMEM =
"Data format:<br /><pre>"
"  Object Name Cat    RA       Dec   <br />"
"  ccccccccccc,ccc,HH:MM:SS,sDD*MM:SS<br /></pre>"
"Sample data:<br /><pre>"
"<p style='color:white'>"
"  $PlaNeb1<br />"
"  M97        ,PN ,11:15:53,+54*55:24<br />"
"  M27        ,PN ,20:00:26,+22*46:29<br />"
"</p></pre>"
L_CAT_EXAMPLE1 L_CAT_EXAMPLE2 L_CAT_EXAMPLE3 L_CAT_EXAMPLE4 L_CAT_EXAMPLE5
"    UNK = Unknown<br />"
"    OC  = Open Cluster<br />"
"    GC  = Globular Cluster<br />";
const char html_libCatalogForm2[] PROGMEM =
"    PN  = Planetary Nebula<br />"
"    DN  = Dark Nebula<br />"
"    SG  = Spiral Galaxy<br />"
"    EG  = Elliptical Galaxy<br />"
"    IG  = Irregular Galaxy<br />"
"    KNT = Knot<br />"
"    SNR = Supernova Remnant<br />"
"    GAL = Galaxy<br />"
"    CN  = Cluster w/Nebula<br />"
"    STR = Star<br />"
"    PLA = Planet<br />"
"    CMT = Comet<br />"
"    AST = Asteroid<br /><br />"
L_CAT_EXAMPLE6 L_CAT_EXAMPLE7
"<br /></pre>\n";

const char html_libEnd[] PROGMEM =
"</form>\r\n";
