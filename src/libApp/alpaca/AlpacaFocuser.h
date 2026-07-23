// -----------------------------------------------------------------------------------
// ASCOM Alpaca IFocuserV4 implementation, gateway to OnStep via onStep.command()
// One instance covers all (up to 6) focusers -- see AlpacaFocuser.cpp for how device
// number maps to the underlying OnStep focuser/axis.
#pragma once

#include "../../Common.h"

#if ALPACA_SERVER == ON

#include "AlpacaJson.h"

class AlpacaFocuser {
  public:
    void handleRequest(int deviceNumber, const String &method, int httpMethod, AlpacaResponse &resp);
};

extern AlpacaFocuser alpacaFocuser;

#endif
