// -----------------------------------------------------------------------------------
// Coordinate basis tile
#pragma once

#include "../htmlHeaders.h"
#include "../htmlMessages.h"
#include "../htmlScripts.h"

#if DISPLAY_COORDINATE_ORIGIN == ON
  extern void coordinateTile(String &data);
  extern void coordinateTileGet();

  const char html_coordinateBasis[] PROGMEM =
    L_COORD_ORIGIN_MSG
    "<br /><br />"
    "<button onpointerdown=\"if (confirm('" L_ARE_YOU_SURE "?')) s('ob','set')\" type='button'>" L_COORD_ORIGIN "</button>"
    "<br />\n";
#endif
