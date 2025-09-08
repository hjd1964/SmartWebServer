// -----------------------------------------------------------------------------------
// Misc functions to help with commands, etc.

#include "Misc.h"
#include "../../locales/Locale.h"
#include "../status/Status.h"

int hexToInt(String s) {
  int i0;
  int i1;
  if (s.length() != 2) return -1;
  char c0 = s.charAt(0);
  char c1 = s.charAt(1);
  if ( ((c0 >= '0' && c0 <= '9') || (c0 >= 'A' && c0 <= 'F')) && ((c1 >= '0' && c1 <= '9') || (c1 >= 'A' && c1 <= 'F')) ) {
    if (c0 >= '0' && c0 <= '9') { i0 = c0 - '0'; } else { i0 = (c0 - 'A') + 10; }
    if (c1 >= '0' && c1 <= '9') { i1 = c1 - '0'; } else { i1 = (c1 - 'A') + 10; }
    return i0*16 + i1;
  } else return -1;
}

uint8_t timeToByte(float t) {
  float v = 10;                         // default is 1 second
  if (t <= 0.0162) v=0; else            // 0.0156 (1/64 second)        (0)
  if (t <= 0.0313) v=1; else            // 0.0313 (1/32 second)        (1)
  if (t <= 0.0625) v=2; else            // 0.0625 (1/16 second)        (2)
  if (t <= 1.0) v=2.0+t*8.0; else       // 0.125 seconds to 1 seconds  (2 to 10)
  if (t <= 10.0) v=6.0+t*4.0; else      // 0.25 seconds to 10 seconds  (10 to 46)
  if (t <= 30.0) v=26.0+t*2.0; else     // 0.5 seconds to 30 seconds   (46 to 86)
  if (t <= 120.0) v=56.0+t; else        // 1 second to 120 seconds     (86 to 176)
  if (t <= 600.0) v=168.0+t/15.0; else  // 15 seconds to 300 seconds   (176 to 208)
  if (t <= 3360.0) v=198.0+t/60.0; else // 1 minute to 56 minutes      (208 to 254)
  if (t <= 3600.0) v=255;               // 1 hour                      (255)
  if (v < 0) v=0;
  if (v > 255) v=255;
  return lround(v);
}

float byteToTime(uint8_t b) {
  float v = 1.0;                        // default is 1 second
  if (b == 0) v=0.016125; else          // 0.0156 (1/64 second)        (0)
  if (b == 1) v=0.03125; else           // 0.0313 (1/32 second)        (1)
  if (b == 2) v=0.0625; else            // 0.0625 (1/16 second)        (2)
  if (b <= 10) v=(b-2.0)/8.0; else      // 0.125 seconds to 1 seconds  (2 to 10)
  if (b <= 46) v=(b-6.0)/4.0; else      // 0.25 seconds to 10 seconds  (10 to 46)
  if (b <= 86) v=(b-26.0)/2.0; else     // 0.5 seconds to 30 seconds   (46 to 86)
  if (b <= 176) v=(b-56.0); else        // 1 second to 120 seconds     (86 to 176)
  if (b <= 208) v=(b-168.0)*15.0; else  // 15 seconds to 300 seconds   (176 to 208)
  if (b <= 254) v=(b-198.0)*60.0; else  // 1 minute to 56 minutes      (208 to 254)
  if (b == 255) v=3600.0;               // 1 hour                      (255)
  return v;
}

void localeTemperature(char* temperatureStr) {
  float t = atof(temperatureStr);
  if (DISPLAY_UNITS == IMPERIAL) {
    t = t*(9.0/5.0) + 32.0;
    dtostrf(t, 3, 1, temperatureStr);
    strcat(temperatureStr, "&deg;F");
  } else strcat(temperatureStr, "&deg;C");
  if (isnan(t)) strcpy(temperatureStr, "?");
}

void localePressure(char* pressureStr) {
  float p = atof(pressureStr);
  if (DISPLAY_UNITS == IMPERIAL) {
    p = p/33.864;
    dtostrf(p, 4, 2, pressureStr);
    strcat(pressureStr, " inHg");
  } else strcat(pressureStr, " mb");
  if (isnan(p)) strcpy(pressureStr, "?");
}

void localeHumidity(char* humidityStr) {
  float h = atof(humidityStr);
  strcat(humidityStr, "%");
  if (isnan(h)) strcpy(humidityStr, "?");
}
