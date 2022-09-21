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

// unused ajax page update feature
#define AJAX_PAGE_UPDATE_RATE_FAST_MS 200     // fast update rate
#define AJAX_PAGE_UPDATE_FAST_SHED_MS 5000    // time before return to normal update rate

// various auxillary features
#define SWITCH                       1
#define ANALOG_OUTPUT                2
#define DEW_HEATER                   3
#define INTERVALOMETER               4

// The settings below are for initialization only, afterward they are stored and recalled from EEPROM and must
// be changed in the web interface OR with a reset (for initialization again) as described in the Config.h comments
#define TIMEOUT_WEB                  200
#define TIMEOUT_CMD                  200

// task manager
#define TASKS_MAX                    32      // up to 32 tasks
#define TASKS_SKIP_MISSED

// EEPROM contents
#define INIT_NV_KEY                  698623872UL

#define NV_TIMEOUT_CMD               5       // 2 bytes
#define NV_TIMEOUT_WEB               7       // 2 bytes
#define NV_ETHERNET_SETTINGS_BASE    100     // up to 200 bytes (128)
#define NV_WIFI_SETTINGS_BASE        300     // up to 500 bytes (451)
#define NV_ENCODER_SETTINGS_BASE     800     // up to 100 bytes (88)
#define NV_ENCODER_A1_ZERO           900     // up to 8 bytes
#define NV_ENCODER_A2_ZERO           908     // up to 8 bytes
