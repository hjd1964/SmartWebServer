// -----------------------------------------------------------------------------------
// Display and process data from webpages

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

#if OPERATIONAL_MODE == WIFI
  void handleWifi();
#endif

void handleNotFound();
