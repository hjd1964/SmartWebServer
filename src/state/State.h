#pragma once

#include "../Common.h"

// encoder polling rate in milli-seconds
#define STATE_POLLING_RATE_MS 100

class State {
  public:
    void init();
    void poll();
    DynamicJsonDocument *doc;
  private:
};

extern State state;
