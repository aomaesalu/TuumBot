/** @file rtx_ctl.hpp
 *  Tuum Robotex logic controllers implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"
#include "tuum_motion.hpp"

#include "rtx_ctl.hpp"

namespace rtx { namespace ctl {

  // Warmup
  void LSInit::run() {
    switch(ctx.phase) {
      case CP_INIT:
        //Motion::setBehaviour(Motion::MOT_CURVED);
      	//Motion::setTarget(Transform({{0, 15}, M_PI/4}));

        ctx.phase = CP_RUN;
        break;
      case CP_RUN:
        // TODO: timer/motion timeout/finish check
        ctx.phase = CP_DONE;
        break;
      case CP_DONE:
        break;
      default:
        break;
    }
  }

  bool LSInit::isRunnable() {
    return true;
  }

  bool LSInit::isInterruptable() {
    return ctx.phase == CP_DONE;
  }


  // Ball search
  void LSBallLocate::run() {
    //TODO: Spin to win
  }

  bool LSBallLocate::isRunnable() {
    return true;
  }


  // Ball retrieval
  void LSBallRetrieve::run() {
    //TODO
  }

  bool LSBallRetrieve::isRunnable() {
    if(Visioning::balls.size() > 0) {
      // Detected balls are reachable?
    }

    return true;
  }


  // Opposing goal search
  void LSGoalLocate::run() {
    //TODO
  }

  bool LSGoalLocate::isRunnable() {
    if(hal::hw.isBallInDribbler()) {

    }

    return true;
  }


  // Shoot to opposing goal
  void LSGoalShoot::run() {
    //TODO
  }

  bool LSGoalShoot::isRunnable() {
    //TODO

    return true;
  }

}}
