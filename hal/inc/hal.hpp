
#ifndef HAL_H
#define HAL_H

#include <unistd.h>
#include <stdint.h>

unsigned int millis();

struct Timer {
  uint32_t _start = 0;
  uint32_t period;
  uint32_t _end = 0;

  void start() {
    _start = millis();
    _end = _start + period;
  }

  bool isTime() {
    return millis() > _end;
  }

  void setPeriod(uint32_t T) {
    period = T;
  }

};


#endif
