/** @file tfb_logic.hpp
 *  Team football logic module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#ifndef RTX_TFB_LOGIC_H
#define RTX_TFB_LOGIC_H

#include "application.hpp"

namespace rtx { namespace Logic {

  enum StrategyConstant {
    NONE,
    MAINSTREAM
  };

  enum LogicState {
    LS_INIT,
    LS_PASSIVE,
    LS_BALL_LOCATE,
    LS_BALL_RETRIEVE,
    LS_GOAL_SCAN,
    LS_GOAL_SHOOT
  };

  void setup();
  void process();

  void printSystemState();

}}

#endif
