// -----------------------------------------------------------------------------------
// Configuration defaults
#pragma once

// use the HAL specified default NV driver
#ifndef NV_DRIVER
#define NV_DRIVER                     NV_DEFAULT
#endif

// Wifi related
#define STA_AP_FALLBACK               true    // activate SoftAP if station fails to connect

// onstep reset control related
#ifndef RESET_PIN
#define RESET_PIN                     OFF
#endif

#ifndef RESET_PIN_STATE
#define RESET_PIN_STATE               LOW
#endif

// background ajax page update timing (in milliseconds, 50ms minimum)
#ifndef AJAX_PAGE_UPDATE_RATE_MS
#define AJAX_PAGE_UPDATE_RATE_MS      500     // normal ajax page update update rate
#endif

#ifndef STATE_POLLING_RATE_MS
#define STATE_POLLING_RATE_MS         500     // time between updates for most OnStep state information
#endif

// background scanning of OnStep state (in milliseconds, 200ms minimum)
#ifndef STATE_SLOW_POLLING_RATE_MS
#define STATE_SLOW_POLLING_RATE_MS    1000    // as above except for less critical items
#endif

#ifndef STATE_FAST_POLLING_RATE_MS
#define STATE_FAST_POLLING_RATE_MS    500     // as above except for more critical items
#endif

#ifndef STATE_GPIO_POLLING_RATE_MS
#define STATE_GPIO_POLLING_RATE_MS    100     // time between updates of OnStep GPIO pin state
#endif
