/** @file hal.hpp
 *  Consolidated harware abstraction layer header.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include "hal.hpp"

#include "MainBoard.hpp"

#include "Hardware.hpp"

#ifndef RTX_HAL_H
#define RTX_HAL_H

namespace rtx { namespace hal {

  extern Hardware hw;

  void setup();
  void process();

}}

#endif // RTX_HAL_H
