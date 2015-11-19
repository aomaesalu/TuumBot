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
        Motion::setBehaviour(Motion::MOT_CURVED);
        Motion::setTarget(Transform(10, 10, 0));

        ctx.phase = CP_RUN;
        break;
      case CP_RUN:
        // TODO: timer/motion timeout/finish check
        ctx.phase = CP_DONE;
        break;
      case CP_DONE:
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
  void LSBallLocate::init() {
    ctx.phase = CP_INIT;
  }

  void LSBallLocate::run() {
    switch(ctx.phase) {
      case CP_INIT:
	Motion::setBehaviour(Motion::MOT_SCAN);
	Motion::start();
	ctx.phase = CP_RUN;
	break;
      case CP_RUN:
	break;
      case CP_DONE:
	break;
    }
  }

  bool LSBallLocate::isRunnable() {
    return true;
  }


  // Ball retrieval
  void LSBallRetrieve::init() {
    Motion::stop();

    targetUpdate.setPeriod(1000);
    targetUpdate.start();
  }

  void LSBallRetrieve::run() {
    if(targetUpdate.isTime()) {
      std::cout << "LSBallRetrieve: " << Visioning::balls.size() << " balls." << std::endl;
      targetUpdate.start();
    }
  }

  bool LSBallRetrieve::isRunnable() {
    return Visioning::balls.size() > 0;
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
