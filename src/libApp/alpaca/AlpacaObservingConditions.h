// -----------------------------------------------------------------------------------
// ASCOM Alpaca IObservingConditionsV2 implementation, gateway to OnStep via onStep.command()
#pragma once

#include "../../Common.h"

#if ALPACA_SERVER == ON

#include "AlpacaJson.h"

class AlpacaObservingConditions {
  public:
    void handleRequest(int deviceNumber, const String &method, int httpMethod, AlpacaResponse &resp);
};

extern AlpacaObservingConditions alpacaObservingConditions;

#endif
