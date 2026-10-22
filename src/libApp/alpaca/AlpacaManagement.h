// -----------------------------------------------------------------------------------
// Alpaca Management API: /management/apiversions, /management/v1/description,
// /management/v1/configureddevices
#pragma once

#include "../../Common.h"

#if ALPACA_SERVER == ON

#include "AlpacaJson.h"

void alpacaManagementApiVersions(AlpacaResponse &resp);
void alpacaManagementDescription(AlpacaResponse &resp);
void alpacaManagementConfiguredDevices(AlpacaResponse &resp);

#endif
