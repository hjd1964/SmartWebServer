// ----------------------------------------------------------------------------------------------------------------
// Read Broadcom BISS-C type encoders

#include <Arduino.h>
#include "../../Constants.h"
#include "../../Config.h"
#include "../../ConfigX.h"
#include "../pinmaps/Models.h"

// tested and known to work with Broadcom AS37-H39B-B
// designed according protocol description found in as38-H39e-b-an100.pdf

#if AXIS1_ENC == BC_BISSC || AXIS2_ENC == BC_BISSC
  #ifdef EEPROM_DISABLED
    #error "Absolute encoders require NV/EEPROM which isn't supported on this platform"
  #endif

  // similar broadcom devices have range of 10MHz to 80 KHz
  // the fastest supported rate for this library is 500 KHz
  // rounding makes rates of about 500, 250, 166.66, 125, 100, 83.33 KHz
  #define CLOCK_RATE_KHZ 250

  #define ENC_HAS_ABSOLUTE
  
  class BiSSC_Encoder {
    public:
      BiSSC_Encoder(int16_t maPin, int16_t sloPin, int16_t axis);
      int32_t read();
      void write(int32_t v);
      void setZero();

    private:
      uint32_t _position = 0;
      int32_t _offset = 0;
      int16_t _clkPin;
      int16_t _sloPin;
      int16_t _axis;
      
      bool readEnc(uint32_t &encPos);
  };

  #if AXIS1_ENC == BC_BISSC
    #define ENC_HAS_ABSOLUTE_AXIS1
    extern BiSSC_Encoder axis1Pos;
  #endif

  #if AXIS2_ENC == BC_BISSC
    #define ENC_HAS_ABSOLUTE_AXIS2
    extern BiSSC_Encoder axis2Pos;
  #endif

#endif
