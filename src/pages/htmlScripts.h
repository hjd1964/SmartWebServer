#pragma once

// Javascript for Ajax return
const char html_ajaxScript[] PROGMEM =
"<script>\n"
"function s(key,v1) {"
  "var xhttp = new XMLHttpRequest();"
  "xhttp.open('GET','%s?'+key+'='+encodeURIComponent(v1)+'&x='+new Date().getTime(), true);"
  "xhttp.send();"
"}</script>\n";

const char html_ajaxScriptShort[] PROGMEM =
"<script>\n"
"function g(v1){s('dr',v1);}"
"function gf(v1){s('dr',v1);autoFastRun();}"
"function sf(key,v1){s(key,v1);autoFastRun();}\n"
"</script>\n";

// Javascript for Date/Time return
const char html_dateTimeScriptA[] PROGMEM =
"<script>\r\n"
"function SetDateTime() {"
"var d1 = new Date();"
"var jan = new Date(d1.getFullYear(), 0, 1);"
"var d = new Date(d1.getTime()-(jan.getTimezoneOffset()-d1.getTimezoneOffset())*60*1000);";
const char html_dateTimeScriptB[] PROGMEM =
"document.getElementById('dd').value = d.getDate();"
"document.getElementById('dm').value = d.getMonth();"
"document.getElementById('dy').value = d.getFullYear();";
const char html_dateTimeScriptC[] PROGMEM =
"document.getElementById('th').value = d.getHours();"
"document.getElementById('tm').value = d.getMinutes();"
"document.getElementById('ts').value = d.getSeconds();"
"}\r\n"
"</script>\r\n";

// Javascript for library status
const char html_libStatusScript[] PROGMEM =
"<script>\n"
"function t(key,v) { if (v.length==0) v='DELETE'; s(key,v.replace(/ /g, '_')); }\n"
"function busy() {\n"
  "document.getElementById('message').innerHTML='Working...';"
  "document.getElementById('up').disabled=true;"
  "document.getElementById('down').disabled=true;"
"}\n"
"</script>\n";

// Javascript for Collapsibles
const char html_collapseScript[] PROGMEM =
"<script>"
"var cc = document.getElementsByClassName('collapsible');"
"var i;"
"for (i = 0; i < cc.length; i++) {"
  "cc[i].addEventListener('click', function() {"
    "this.classList.toggle('active');"
    "var ct = this.nextElementSibling;"
    "if (ct.style.display === 'block') { ct.style.display = 'none'; } else { ct.style.display = 'block'; }"
  "});"
"}"
"</script>\n";

// Javascript for Ajax active refresh of controls
// be sure to define "var ajaxPage='control.txt';" etc.
const char html_ajax_active[] PROGMEM =
"<script>\n"
"function pad(num, size) { var s = '000000000' + num; return s.substr(s.length-size); }\n"
"function update_date_time() {\n"
  "var now = new Date();\n"
  "document.getElementById('datetime').innerHTML = \n"
  "(now.getUTCMonth()+1)+'/'+pad(now.getUTCDate().toString(),2)+'/'+pad(now.getUTCFullYear().toString().substr(-2),2)+' '+\n"
  "pad(now.getUTCHours().toString(),2)+':'+pad(now.getUTCMinutes().toString(),2)+':'+pad(now.getUTCSeconds().toString(),2);\n"
"}\n"
"var auto1Tick=-1;\n"
"var auto2Tick=0;\n"
"var auto2Rate=" DEFAULT_AJAX_RATE "*10;\n"
"var auto1=setInterval(autoRun,100);\n"
"function autoFastRun() {\n"
  "auto2Rate=" DEFAULT_FAST_AJAX_RATE "*10\n"
  "auto2Tick=" DEFAULT_AJAX_SHED_TIME "*10;\n"
"}\n"
"function autoRun() {\n"
  "auto1Tick++;\n"
  "var i;\n"
  "if (auto2Tick>=0) auto2Tick--;\n"
  "if (auto2Tick==0) auto2Rate=" DEFAULT_AJAX_RATE "*10;\n"
  "if (auto1Tick%auto2Rate==0) {\n"
    "nocache='?nocache='+Math.random()*1000000;\n"
    "var request = new XMLHttpRequest();\n"
    "request.onreadystatechange = pageReady(ajaxPage);\n"
    "request.open('GET',ajaxPage.toLowerCase()+nocache,true); request.send(null);\n"
  "}"
"}\n"
"function pageReady(aPage) {\n"
  "return function() {\n"
    "if ((this.readyState==4)&&(this.status==200)) {\n"
      "lines=this.responseText.split('\\n');\n"
      "for (var i=0; i<lines.length; i++) {\n"
        "j=lines[i].indexOf('|');m=0;\n"
        "if (j==-1) {j=lines[i].indexOf('&');m=1;}\n"
        "v=lines[i].slice(j+1);\n"
        "k=lines[i].slice(0,j);\n"
        "if (k=='call' && v=='update_date_time') {\n"
          "update_date_time();\n"
        "} else {\n"
          "if (k!=''&&document.getElementById(k)!=null) {\n"
          " if (m==1) document.getElementById(k).value=v; else "
          " if (v=='disabled') document.getElementById(k).disabled=true; else"
          " if (v=='enabled') document.getElementById(k).disabled=false; else"
          " document.getElementById(k).innerHTML=v;"
          "}\n"
        "}\n"
      "}\n"
    "}"
  "}"
"}\n"
"</script>\n";
