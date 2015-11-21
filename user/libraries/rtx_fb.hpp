/** @file rtx_fb.hpp
 *  Robotex 1vs1 football logic module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#ifndef RTX_TFB_LOGIC_H
#define RTX_TFB_LOGIC_H

#include "application.hpp"
#include "STM.hpp"

namespace rtx { namespace Logic {

  extern STM stm;

  void setup();
  void process();

}}

#endif
