// -----------------------------------------------------------------------------------
// Minimal hand-rolled JSON helpers for the Alpaca REST API responses.
#include "AlpacaJson.h"

String jsonEscape(const String &s) {
  String out;
  out.reserve(s.length() + 4);
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    switch (c) {
      case '"':  out += "\\\""; break;
      case '\\': out += "\\\\"; break;
      case '\n': out += "\\n";  break;
      case '\r': out += "\\r";  break;
      case '\t': out += "\\t";  break;
      default:
        if ((uint8_t)c < 0x20) { char b[7]; snprintf(b, sizeof(b), "\\u%04x", c); out += b; }
        else out += c;
    }
  }
  return out;
}

String jsonBool(bool v) {
  return v ? "true" : "false";
}

String jsonNumber(double v, uint8_t decimals) {
  if (isnan(v) || isinf(v)) return "null";
  char buf[32];
  dtostrf(v, 0, decimals, buf);
  return String(buf);
}

String jsonInt(long v) {
  return String(v);
}

String jsonString(const String &v) {
  String out = "\"";
  out += jsonEscape(v);
  out += "\"";
  return out;
}

void jsonStateValueEntry(String &out, bool &first, const char *name, const String &valueJson) {
  if (!first) out += ",";
  out += "{\"Name\":\"" + String(name) + "\",\"Value\":" + valueJson + "}";
  first = false;
}

// server-wide monotonically increasing transaction id, per the Alpaca spec
static uint32_t serverTransactionCounter = 0;

void AlpacaResponse::begin(uint32_t clientTransactionId) {
  this->clientTransactionId = clientTransactionId;
  serverTransactionCounter++;
  value = "";
  hasValue = false;
  errorNumber = 0;
  errorMessage = "";
}

void AlpacaResponse::setValue(const String &jsonValueFragment) {
  value = jsonValueFragment;
  hasValue = true;
}

void AlpacaResponse::setError(int errorNumber, const String &message) {
  this->errorNumber = errorNumber;
  this->errorMessage = message;
}

String AlpacaResponse::toJson() {
  String out = "{";
  if (hasValue && errorNumber == 0) {
    out += "\"Value\":";
    out += value;
    out += ",";
  }
  out += "\"ClientTransactionID\":" + String(clientTransactionId) + ",";
  out += "\"ServerTransactionID\":" + String(serverTransactionCounter) + ",";
  out += "\"ErrorNumber\":" + String(errorNumber) + ",";
  out += "\"ErrorMessage\":" + jsonString(errorMessage);
  out += "}";
  return out;
}
