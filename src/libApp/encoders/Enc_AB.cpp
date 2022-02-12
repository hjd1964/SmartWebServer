// ----------------------------------------------------------------------------------------------------------------
// Read Quadrature A/B type encoders

#include "Enc_AB.h"
#include "Encoders.h"

#if AXIS1_ENC == AB
  Encoder axis1Pos(AXIS1_ENC_A_PIN, AXIS1_ENC_B_PIN);
#endif

#if AXIS2_ENC == AB
  Encoder axis2Pos(AXIS2_ENC_A_PIN, AXIS2_ENC_B_PIN);
#endif
