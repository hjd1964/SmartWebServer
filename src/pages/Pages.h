// -----------------------------------------------------------------------------------
// Display and process data from webpages

#include <Arduino.h>
#include "../Constants.h"
#include "../../Config.h"
#include "../../Extended.config.h"

#include "EncoderControl.h"

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
