#pragma once

#include "../Common.h"

// encoder polling rate in milli-seconds
#define STATE_POLLING_RATE_MS 100

class State {
  public:
    void init();
    void poll();

    DynamicJsonDocument *doc;
    char date[10];
    char time[10];
    char last[10];

  private:
};

extern State state;
