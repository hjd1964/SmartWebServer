// -----------------------------------------------------------------------------------
// Minimal hand-rolled JSON helpers for the Alpaca REST API responses.
// No parsing is needed: Alpaca requests are query-string/form-encoded, only
// responses are JSON. Kept deliberately small to match the codebase's existing
// manual string-building style (see src/pages/KeyValue.cpp) instead of pulling
// in a JSON library.
#pragma once

#include "../../Common.h"

// escape a string for safe embedding inside JSON double-quotes
String jsonEscape(const String &s);

// JSON value fragment builders (no wrapping key, caller assembles objects/arrays)
String jsonBool(bool v);
String jsonNumber(double v, uint8_t decimals = 6);
String jsonInt(long v);
String jsonString(const String &v);

// appends one {"Name":"<name>","Value":<valueJson>} entry (comma-separated) to an array
// under construction -- the IStateValue shape used by every device's DeviceState property.
// `first` tracks whether a leading comma is needed; caller wraps the result in "[" ... "]".
void jsonStateValueEntry(String &out, bool &first, const char *name, const String &valueJson);

// builds the standard Alpaca response envelope:
// {"Value":<value>,"ClientTransactionID":N,"ServerTransactionID":N,"ErrorNumber":N,"ErrorMessage":"..."}
// "Value" is present only when setValue() was called and no error was set (methods that
// return void, and property/method calls that errored, omit it entirely per the Alpaca spec).
class AlpacaResponse {
  public:
    void begin(uint32_t clientTransactionId);

    // value must already be a valid JSON fragment (number/bool/string/array/object)
    void setValue(const String &jsonValueFragment);

    void setError(int errorNumber, const String &message);

    String toJson();

  private:
    String value;
    bool hasValue = false;
    uint32_t clientTransactionId = 0;
    int errorNumber = 0;
    String errorMessage;
};
