/** @file hal.cpp
 *  Harware abstraction layer system module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include "rtxmath.hpp"
#include "rtxhal.hpp"


namespace rtx { namespace hal {

  Hardware hw;

  Timer senseTimer;

  void setup() {
    hw.init();

    senseTimer.setPeriod(3000);
    senseTimer.start();
  }


  void process() {
    if(senseTimer.isTime()) {
      hw.getMainBoard()->senseBall();
      senseTimer.start();
    }
    hw.run();
  }

}}
