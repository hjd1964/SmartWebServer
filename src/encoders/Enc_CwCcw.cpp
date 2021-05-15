// ----------------------------------------------------------------------------------------------------------------
// Read CW/CCW type encoders

#include "Enc_CwCcw.h"

#if AXIS1_ENC == CWCCW || AXIS2_ENC == CWCCW

  #if AXIS1_ENC == CWCCW && AXIS1_ENC_RATE_CONTROL == ON
  
    #define MIN_ENC_PERIOD 0.2
    #define MAX_ENC_PERIOD 5.0

    extern float Axis1EncTicksPerDeg;
    extern volatile long Axis1EncStaSamples;
    extern volatile long Axis1EncLtaSamples;
    extern volatile int32_t Tsta;
    extern volatile int32_t Tlta;
    extern float arcSecondsPerTick;
    extern float usPerTick;

    unsigned long msPerTickMax = (arcSecondsPerTick/15.041)*1000.0*MAX_ENC_PERIOD;
    #if AXIS1_ENC_BIN_AVG > 0
      volatile uint32_t usPerBinTickMin = (double)usPerTick*(double)AXIS1_ENC_BIN_AVG*MIN_ENC_PERIOD;
      volatile uint32_t usPerBinTickMax = (double)usPerTick*(double)AXIS1_ENC_BIN_AVG*MAX_ENC_PERIOD;
    #endif
    #if defined(ESP8266) || defined(ESP32)
      volatile uint32_t clocksPerTickMin = (double)usPerTick*(double)ESP.getCpuFreqMHz()*MIN_ENC_PERIOD;
      volatile uint32_t clocksPerTickMax = (double)usPerTick*(double)ESP.getCpuFreqMHz()*MAX_ENC_PERIOD;
      #define GetClockCount ESP.getCycleCount()
      #define ClockCountToMicros ((uint32_t)ESP.getCpuFreqMHz())
    #elif defined(__MK20DX256__)
      volatile uint32_t clocksPerTickMin = (double)usPerTick*(double)(F_CPU/1000000L)*MIN_ENC_PERIOD;
      volatile uint32_t clocksPerTickMax = (double)usPerTick*(double)(F_CPU/1000000L)*MAX_ENC_PERIOD;
      #define GetClockCount ARM_DWT_CYCCNT
      #define ClockCountToMicros (F_CPU/1000000L)
    #else
      volatile uint32_t clocksPerTickMin = (double)usPerTick*MIN_ENC_PERIOD;
      volatile uint32_t clocksPerTickMax = (double)usPerTick*MAX_ENC_PERIOD;
      #define GetClockCount micros()
      #define ClockCountToMicros (1L)
    #endif

    volatile uint32_t TM0 = 0;
    volatile uint32_t TM1 = 0;
    volatile uint32_t Telapsed = 0;

    static unsigned long lastLogRate = 0;
    bool fastMotion() { return Telapsed < clocksPerTickMin; }
    bool slowMotion() { return (millis() - lastLogRate) > msPerTickMax; }
    
    // this ISR function times the arrival of pulses from the RA axis encoder
    void ICACHE_RAM_ATTR __logRate() {
      lastLogRate = millis();
      #if AXIS1_ENC_BIN_AVG > 0
        int i = abs(__p1)%AXIS1_ENC_BIN_AVG;
        uint32_t T0us = T0/ClockCountToMicros;
        uint32_t Te = T0us - T1Bins[i]; T1Bins[i] = T0us;
        if (Te > usPerBinTickMin && Te < usPerBinTickMax)
        {
          StaBins[i] = ((StaBins[i]*(Axis1EncStaSamples - 1)) + Te)/Axis1EncStaSamples;
          LtaBins[i] = ((LtaBins[i]*(Axis1EncLtaSamples - 1)) + Te)/Axis1EncLtaSamples;
        }
      #else
        uint32_t Te=Telapsed/ClockCountToMicros;
        Tsta=((Tsta*(Axis1EncStaSamples - 1)) + Te)/Axis1EncStaSamples;
        Tlta=((Tlta*(Axis1EncLtaSamples - 1)) + Te)/Axis1EncLtaSamples;
      #endif
    }
  #endif

  #if AXIS1_ENC == CWCCW
    volatile int32_t __p1;
    void ICACHE_RAM_ATTR __cw1() {
      __p1++;
      #if AXIS1_ENC_RATE_CONTROL == ON
        TM0 = GetClockCount; Telapsed = TM0 - TM1; TM1 = TM0;
        if (Telapsed > clocksPerTickMin) __logRate();
      #endif
    }

    void ICACHE_RAM_ATTR __ccw1() {
      __p1--;
      #if AXIS1_ENC_RATE_CONTROL == ON
        TM0 = GetClockCount; Telapsed = TM0 - TM1; TM1 = TM0;
        if (Telapsed > clocksPerTickMin) __logRate();
      #endif
    }
  #endif

  #if AXIS2_ENC == CWCCW
    volatile int32_t __p2;
    void ICACHE_RAM_ATTR __cw2() { __p2++; }
    void ICACHE_RAM_ATTR __ccw2() { __p2--; }
  #endif

  CwCcwEncoder::CwCcwEncoder(int16_t cwPin, int16_t ccwPin, int16_t axis) {
    _cwPin = cwPin;
    _ccwPin = ccwPin;
    _axis = axis;
    pinMode(_cwPin, INPUT_PULLUP);
    pinMode(_ccwPin, INPUT_PULLUP);
    #if AXIS1_ENC == CWCCW
      if (_axis == 1) {
        attachInterrupt(digitalPinToInterrupt(_cwPin), __cw1, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_ccwPin), __ccw1, CHANGE);
      }
    #endif
    #if AXIS2_ENC == CWCCW
      if (_axis == 2) {
        attachInterrupt(digitalPinToInterrupt(_cwPin), __cw2, CHANGE);
        attachInterrupt(digitalPinToInterrupt(_ccwPin), __ccw2, CHANGE);
      }
    #endif
  }

  int32_t CwCcwEncoder::read() {
    int32_t v = 0;
    #if AXIS1_ENC == CWCCW
      if (_axis == 1) { noInterrupts(); v = __p1; interrupts(); }
    #endif
    #if AXIS2_ENC == CWCCW
      if (_axis == 2) { noInterrupts(); v = __p2; interrupts(); }
    #endif
    return v;
  }
  
  void CwCcwEncoder::write(int32_t v) {
    #if AXIS1_ENC == CWCCW
      if (_axis == 1) { noInterrupts(); __p1 = v; interrupts(); }
    #endif
    #if AXIS2_ENC == CWCCW
      if (_axis == 2) { noInterrupts(); __p2 = v; interrupts(); }
    #endif
  }

  #if AXIS1_ENC == CWCCW
    CwCcwEncoder axis1Pos(AXIS1_ENC_A_PIN, AXIS1_ENC_B_PIN, 1);
  #endif

  #if AXIS2_ENC == CWCCW
    CwCcwEncoder axis2Pos(AXIS2_ENC_A_PIN, AXIS2_ENC_B_PIN, 2);
  #endif

#endif
