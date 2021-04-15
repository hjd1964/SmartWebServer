// -----------------------------------------------------------------------------------
// Error not found, 404

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
#include "../ethernetServers/WebServer.h"

#include "htmlHeaders.h"
#include "htmlMessages.h"
#include "htmlScripts.h"

#if OPERATIONAL_MODE == ETHERNET_W5100 || OPERATIONAL_MODE == ETHERNET_W5500
  void handleNotFound(EthernetClient *client) {
    String message = "File Not Found\n\n";
    client->print(message);
    VLF("File not found");
  }
#else
  void handleNotFound() {
    String message = "File Not Found\n\n";
    message.concat("URI: ");
    message.concat(server.uri());
    message.concat("\nMethod: ");
    message.concat((server.method() == HTTP_GET)?"GET":"POST");
    message.concat("\nArguments: ");
    message.concat(server.args());
    message.concat("\n");
    for (uint8_t i = 0; i < server.args(); i++){
      message.concat(" " + server.argName(i) + ": " + server.arg(i) + "\n");
    }
    server.send(404, "text/plain", message);
  }
#endif
