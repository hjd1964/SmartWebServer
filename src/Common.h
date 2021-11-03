// Common includes
#pragma once

// enable lib support for the command and web servers
#define WEB_SERVER ON

#include <Arduino.h>
#include "libApp/status/Version.h"
#include "Constants.h"
#include "lib/Constants.h"
#include "../Config.h"
#include "../Extended.config.h"
#include "Config.defaults.h"
#include "HAL/HAL.h"
#include "lib/Macros.h"
#include "pinmaps/Models.h"
#include "lib/debug/Debug.h"
#include "lib/nv/NV.h"
extern NVS nv;
