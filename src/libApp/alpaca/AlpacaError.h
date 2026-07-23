// -----------------------------------------------------------------------------------
// ASCOM Alpaca standard error/exception numbers ("Exception")
// see https://ascom-standards.org/newdocs/alpaca-errors.html
#pragma once

#include "../../Common.h"

#define ALPACA_OK                          0x0
#define ALPACA_ERR_NOT_IMPLEMENTED         0x400
#define ALPACA_ERR_INVALID_VALUE           0x401
#define ALPACA_ERR_VALUE_NOT_SET           0x402
#define ALPACA_ERR_NOT_CONNECTED           0x407
#define ALPACA_ERR_PARKED                  0x408
#define ALPACA_ERR_SLAVED                  0x409
#define ALPACA_ERR_INVALID_OPERATION       0x40B
#define ALPACA_ERR_ACTION_NOT_IMPLEMENTED  0x40C

// driver-specific error range, add a small offset (0..0x7FF) for our own conditions
#define ALPACA_ERR_DRIVER_BASE             0x500
#define ALPACA_ERR_DRIVER_COMMS_FAILURE    (ALPACA_ERR_DRIVER_BASE + 1) // OnStep serial command failed/timed out
