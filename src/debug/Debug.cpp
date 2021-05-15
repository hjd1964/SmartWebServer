// -----------------------------------------------------------------------------------
// Helper macros for debugging, with less typing

#include "Debug.h"

void debugPrint(const char* s) {
  char s1[255];
  strcpy(s1, s);
  for (unsigned int i = 0; i < strlen(s1); i++) if (s1[i] == ' ') s1[i] = '_';
  SERIAL_ONSTEP.print(s1);
}
