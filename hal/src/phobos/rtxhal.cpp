/** @file hal.cpp
 *  Harware abstraction layer system module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include "rtxhal.hpp"


namespace rtx { namespace hal {

  Hardware hw;

  void setup() {
    hw.init();
  }


  void process() {

  }

}}
