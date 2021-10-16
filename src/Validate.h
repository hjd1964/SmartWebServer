#pragma once

#if DISPLAY_RESET_CONTROLS == FWU
  #if SERIAL_SWAP != OFF
    #error "Configuration (Config.h): Setting DISPLAY_RESET_CONTROLS FWU must be used with SERIAL_SWAP OFF only"
  #endif
#endif
