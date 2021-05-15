// -----------------------------------------------------------------------------------
// Display and process data from webpages

#include <Arduino.h>
#include "../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"

#include "EncoderControl.h"

#if OPERATIONAL_MODE == WIFI
  void handleRoot();

  void handleConfiguration();
  void configurationAjaxGet();

  void handleSettings();
  void settingsAjaxGet();
  void settingsAjax();

  #if ENCODERS == ON
    void handleEncoders();
    void encAjaxGet();
    void encAjax();
  #endif

  void handleLibrary();
  void libraryAjaxGet();
  void libraryAjax();

  void handleControl();
  void controlAjaxGet();
  void controlAjax();

  void handleAux();
  void auxAjaxGet();
  void auxAjax();

  void handlePec();
  void pecAjax();

  void handleNetwork();

  void handleNotFound();
#else
  void handleRoot(EthernetClient *client);

  void handleConfiguration(EthernetClient *client);
  void configurationAjaxGet(EthernetClient *client);

  void handleSettings(EthernetClient *client);
  void settingsAjaxGet(EthernetClient *client);
  void settingsAjax(EthernetClient *client);

  #if ENCODERS == ON
    void handleEncoders(EthernetClient *client);
    void encAjaxGet(EthernetClient *client);
    void encAjax(EthernetClient *client);
  #endif

  void handleLibrary(EthernetClient *client);
  void libraryAjaxGet(EthernetClient *client);
  void libraryAjax(EthernetClient *client);

  void handleControl(EthernetClient *client);
  void controlAjaxGet(EthernetClient *client);
  void controlAjax(EthernetClient *client);

  void handleAux(EthernetClient *client);
  void auxAjaxGet(EthernetClient *client);
  void auxAjax(EthernetClient *client);

  void handlePec(EthernetClient *client);
  void pecAjax(EthernetClient *client);

  void handleNetwork(EthernetClient *client);

  void handleNotFound(EthernetClient *client);
#endif
