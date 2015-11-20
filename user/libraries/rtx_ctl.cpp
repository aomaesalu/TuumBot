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
    Motion::setBehaviour(Motion::MOT_SCAN);
    Motion::start();
  }

  void LSBallLocate::run() {
    switch(ctx.phase) {
      case CP_INIT:
        std::cout << "Locate init" << std::endl;

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

    targetUpdate.setPeriod(1000);
    targetUpdate.start();
  }

  void LSBallRetrieve::run() {
    if(targetUpdate.isTime()) {
      std::cout << "LSBallRetrieve: " << Visioning::ballDetect.size() << " balls." << std::endl;
      targetUpdate.start();

      if(targetBall != nullptr) {
        Ball* b = targetBall;
	Transform* t = b->getTransform();
        std::cout << "Target: <Ball hp=" << b->getHealth() << ", x=" << t->getX() << ", y=" << t->getY() << ">" << std::endl;
      }
    }

    switch(ctx.phase) {
      case CP_INIT:
      {
	// Select ball
	targetBall = nullptr;
	Transform* t = Localization::getTransform();
	double d = 0.0, _d;
	for(auto b : *Visioning::ballDetect.getEntities()) {
          _d = t->distanceTo(b->getTransform()->getPosition());
	  if(d < _d) {
	    d = _d;
	    targetBall = b;
	  }
	}
	if(targetBall != nullptr) ctx.phase = CP_RUN;
	break;
      }
      case CP_RUN:
	// Check if ball valid
	
	
	// Calculate target position
	// ( targetPosition = on (ball <-> gate) line & behind ball )
	break;
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
