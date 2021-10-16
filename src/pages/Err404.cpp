// -----------------------------------------------------------------------------------
// Error not found, 404

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500
  void handleNotFound() {
    String message = "File Not Found\n\n";
    sendHtmlDone(message);
    VLF("File not found");
  }
#else
  void handleNotFound() {
    String message = "File Not Found\n\n";
    message.concat("URI: ");
    message.concat(www.uri());
    message.concat("\nMethod: ");
    message.concat((www.method() == HTTP_GET)?"GET":"POST");
    message.concat("\nArguments: ");
    message.concat(www.args());
    message.concat("\n");
    for (uint8_t i = 0; i < www.args(); i++){
      message.concat(" " + www.argName(i) + ": " + www.arg(i) + "\n");
    }
    www.send(404, "text/plain", message);
  }
#endif
