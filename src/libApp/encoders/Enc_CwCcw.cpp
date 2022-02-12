// ----------------------------------------------------------------------------------------------------------------
// Read CW/CCW type encoders

#include "Enc_CwCcw.h"
#include "Encoders.h"

#if AXIS1_ENC == CWCCW || AXIS2_ENC == CWCCW

  #if AXIS1_ENC == CWCCW
    volatile int32_t p1;
    void ICACHE_RAM_ATTR cw1() { p1++; }
    void ICACHE_RAM_ATTR ccw1() { p1--; }
  #endif
  #if AXIS2_ENC == CWCCW
    volatile int32_t p2;
    void ICACHE_RAM_ATTR cw2() { p2++; }
    void ICACHE_RAM_ATTR ccw2() { p2--; }
  #endif

  CwCcwEncoder::CwCcwEncoder(int16_t cwPin, int16_t ccwPin, int16_t axis) {
    this->axis = axis;
    pinMode(cwPin, INPUT_PULLUP);
    pinMode(ccwPin, INPUT_PULLUP);
    #if AXIS1_ENC == CWCCW
      if (axis == 1) {
        attachInterrupt(digitalPinToInterrupt(cwPin), cw1, CHANGE);
        attachInterrupt(digitalPinToInterrupt(ccwPin), ccw1, CHANGE);
      }
    #endif
    #if AXIS2_ENC == CWCCW
      if (axis == 2) {
        attachInterrupt(digitalPinToInterrupt(cwPin), cw2, CHANGE);
        attachInterrupt(digitalPinToInterrupt(ccwPin), ccw2, CHANGE);
      }
    #endif
  }

  int32_t CwCcwEncoder::read() {
    int32_t v = 0;
    #if AXIS1_ENC == CWCCW
      if (axis == 1) { noInterrupts(); v = p1; interrupts(); }
    #endif
    #if AXIS2_ENC == CWCCW
      if (axis == 2) { noInterrupts(); v = p2; interrupts(); }
    #endif
    return v;
  }
  
  void CwCcwEncoder::write(int32_t v) {
    #if AXIS1_ENC == CWCCW
      if (axis == 1) { noInterrupts(); p1 = v; interrupts(); }
    #endif
    #if AXIS2_ENC == CWCCW
      if (axis == 2) { noInterrupts(); p2 = v; interrupts(); }
    #endif
  }

  #if AXIS1_ENC == CWCCW
    CwCcwEncoder axis1Pos(AXIS1_ENC_A_PIN, AXIS1_ENC_B_PIN, 1);
  #endif
  #if AXIS2_ENC == CWCCW
    CwCcwEncoder axis2Pos(AXIS2_ENC_A_PIN, AXIS2_ENC_B_PIN, 2);
  #endif

#endif
