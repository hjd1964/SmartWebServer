// Library general purpose macros
#pragma once

// conversion math
#define degToRad(x)                 ((x)/(double)RAD_DEG_RATIO)
#define degToRadF(x)                ((x)/(double)RAD_DEG_RATIO_F)
#define radToDeg(x)                 ((x)*(double)RAD_DEG_RATIO)
#define radToDegF(x)                ((x)*(double)RAD_DEG_RATIO_F)
#define hrsToRad(x)                 ((x)/(double)RAD_HOUR_RATIO)
#define radToHrs(x)                 ((x)*(double)RAD_HOUR_RATIO)
#define arcsecToRad(x)              ((x)/(double)206264.8062470963L)
#define radToArcsec(x)              ((x)*(double)206264.8062470963L)
#define siderealToRad(x)            ((x)/(double)13750.98708313975L)
#define siderealToRadF(x)           ((x)/13750.987F)
#define radToSidereal(x)            ((x)*(double)13750.98708313975L)
#define hzToSubMicros(x)            ((x)*(double)266666.666666667L)
#define hzToSidereal(x)             ((x)/(double)SIDEREAL_RATE_HZ)
#define siderealToHz(x)             ((x)*(double)SIDEREAL_RATE_HZ)
#define fequal(x,y)                 (fabs((x)-(y))<SmallestFloat)
#define fgt(x,y)                    ((x)-(y)>SmallestFloat)
#define flt(x,y)                    ((y)-(x)>SmallestFloat)
#define NormalizeAzimuth(x)         (x<0.0L?x+360.0L:x)

// strings
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#ifndef EmptyStr
  #define EmptyStr ""
#endif

// pins
#define DAC_PIN(v)                  ((v)+0x100)
#define GPIO_PIN(v)                 ((v)+0x200)

// sense
#define THLD(v)                     ((v)<<1)  // 10 bit analog threshold, bits 1 through 10
#define HYST(v)                     ((v)<<11) // 10 bit hysteresis, bits 11 through 20
#ifndef INPUT_PULLDOWN
  #define INPUT_PULLDOWN INPUT
#endif

// automatically use fast I/O if available
#ifndef digitalReadF
  #ifdef HAL_HAS_DIGITAL_FAST
    #define digitalReadF(pin)           ( digitalReadFast(pin) )
  #else
    #define digitalReadF(pin)           ( digitalRead(pin) )
  #endif
#endif
#ifndef digitalWriteF
  #ifdef HAL_HAS_DIGITAL_FAST
    #define digitalWriteF(pin,value)   { digitalWriteFast(pin,value); }
  #else
    #define digitalWriteF(pin,value)   { digitalWrite(pin,value); }
  #endif
#endif

// supress compiler warnings for unused parameters
#ifndef UNUSED
  #define UNUSED(x) (void)(x)
#endif
