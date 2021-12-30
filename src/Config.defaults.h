// -----------------------------------------------------------------------------------
// Configuration defaults
#pragma once

// Wifi related
#define STA_AP_FALLBACK                true // activate SoftAP if station fails to connect

// onstep reset control related
#ifndef RESET_PIN
  #define RESET_PIN                    OFF
#endif

#ifndef RESET_PIN_STATE
  #define RESET_PIN_STATE              LOW
#endif
