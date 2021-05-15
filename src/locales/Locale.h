// -----------------------------------------------------------------------------------
// Locale
#pragma once

#include "../Constants.h"
#include "../../Config.h"

#include "Locales.h"

// see Strings_xx.h for individual locale translations

// ISO639-1 language codes (these control the inclusion of individual Strings_xx.h files as specified in Config.h)
#if DISPLAY_LANGUAGE == L_cn
  #include "Strings_cn.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_de
  #include "Strings_de.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_en
  #include "Strings_en.h"
  #define UNITS METRIC
#endif
#if DISPLAY_LANGUAGE == L_us // not ISO639-1 but might be useful
  #include "Strings_en.h"
  #define UNITS IMPERIAL
#endif

// misc. locale support functions
#ifndef DISPLAY_UNITS
  #define DISPLAY_UNITS UNITS
#else
  #if DISPLAY_UNITS == LOCALE_DEFAULT
    #undef DISPLAY_UNITS
    #define DISPLAY_UNITS UNITS
  #endif
#endif
