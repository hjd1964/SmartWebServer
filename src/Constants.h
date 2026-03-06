// -----------------------------------------------------------------------------------
// Constants
#pragma once

// misc.
#define PROD_ABV                     "SWS"

#define METRIC                       ON
#define IMPERIAL                     -200
#define LOCALE_DEFAULT               -201
#define AUTO_ON                      -202
#define AUTO_OFF                     -203
#define FWU                          -204

// ajax page update feature
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 500     // fast update rate
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 5000    // time before return to normal update rate
#define AJAX_PAGE_LAZY_GET_MS         1000    // wait time for lazy get

// various auxillary features
#define SWITCH                       1
#define ANALOG_OUTPUT                2
#define DEW_HEATER                   3
#define INTERVALOMETER               4
#define COVER_SWITCH                 7

// The settings below are for initialization only, afterward they are stored and recalled from EEPROM and must
// be changed in the web interface OR with a reset (for initialization again) as described in the Config.h comments
#define TIMEOUT_WEB                  200
#define TIMEOUT_CMD                  200

// task manager
#define TASKS_MAX                    40      // up to 40 tasks
#define TASKS_SKIP_MISSED

// NV -------------------------------------------------------------------------------------------------------------------

// unique volume signature for this volume/partition layout
#define NV_VOLUME_SIGNATURE         0x0001u
