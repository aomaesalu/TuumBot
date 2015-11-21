/** @file rtx_ctl.hpp
 *  Tuum Robotex logic controllers implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"
#include "tuum_navigation.hpp"
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
    Motion::setBehaviour(Motion::MOT_SCAN);
    Motion::start();
  }

  void LSBallLocate::run() {
    //TODO: try to search from probable previous ball areas
    switch(ctx.phase) {
      case CP_INIT:
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
    Motion::setBehaviour(Motion::MOT_CURVED);
    Motion::stop();

    ctx.phase = CP_INIT;

    targetUpdate.setPeriod(50);
    targetUpdate.start();
  }

  void LSBallRetrieve::run() {
    switch(ctx.phase) {
      case CP_INIT:
        targetBall = Navigation::getNearestBall();
	if(targetBall != nullptr) ctx.phase = CP_RUN;
	break;
      case CP_RUN:
      {
	//if(hw.isBallInDribbler()) break;

	if(targetBall->getHealth() < 5) {
          targetBall = nullptr;
	  ctx.phase = CP_INIT;
	  break;
	}
	
	//TODO: ( targetPosition = on (ball <-> gate) line & behind ball )
        if(targetUpdate.isTime()) {
	  if(targetBall != nullptr) {
	    Ball* b = targetBall;

	    Motion::setTarget(Navigation::calcBallPickupPos(b->getTransform()));
	    if(!Motion::isTargetAchieved()) Motion::start();
	  }

	  targetUpdate.start();
	}
        
	break;
      }
      case CP_DONE:
	break;
    }
  }

  bool LSBallRetrieve::isRunnable() {
    return Visioning::ballDetect.size() > 0;
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
