/** @file __future__.hpp
 *  Contains funcionalities that are yet to be
 *  properly implemented in a module but are required.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 24. October 2015
 */

#include <random>
#include <unistd.h>

#include "rtxc.h"

#ifndef RTX_FUTURE_H
#define RTX_FUTURE_H

// TODO: these belong in loc module? or create dedicated system module?
namespace rtx {

  unsigned int millis();

  struct MotionVec {
    int distance;
    double orientationDelta;
  };

  long double gaussian_probability(double mu, double sig, double x);
  long double random_gaussian(double mu, double sig);


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
};

namespace vis {

  class Feature {
    private:
      int m_placeholder;
    public:
      Feature() {

      }
  };

};

#endif // RTX_FUTURE_H
