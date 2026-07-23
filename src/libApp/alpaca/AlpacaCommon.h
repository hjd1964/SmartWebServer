// -----------------------------------------------------------------------------------
// Shared ASCOM "common device" members (present, with identical semantics, on every
// Alpaca interface: Connected/Connecting/Connect()/Disconnect(), Name, Description,
// DriverInfo, DriverVersion, InterfaceVersion, SupportedActions/Action/Command*, and the
// DeviceState property (the IStateValue mechanism -- an array of {Name,Value} pairs)).
#pragma once

#include "../../Common.h"

#if ALPACA_SERVER == ON

#include "AlpacaJson.h"

// Attempts to handle `member` as one of the common members above.
// Returns true if it was recognized and `resp` has been fully populated -- the caller
// should stop. Returns false if `member` isn't a common member, so the caller should
// continue checking its own device-specific member list.
//
// `deviceStateJson` supplies the device-specific DeviceState entries as already-valid,
// comma-joined {"Name":...,"Value":...} JSON object fragments (no wrapping brackets).
bool alpacaHandleCommonMember(const String &member, int httpMethod, AlpacaResponse &resp,
                               const char *name, const char *description, int interfaceVersion,
                               bool connected, String (*deviceStateJson)());

#endif
