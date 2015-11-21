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
    Motion::stop();

    //hal::hw.getMainBoard()->stopDribbler();

    ctx.phase = CP_INIT;

    targetUpdate.setPeriod(50);
    targetUpdate.start();

    ballPickupTimeout.setPeriod(2000);
    m_catchingBall = false;
  }

  void LSBallRetrieve::run() {
    switch(ctx.phase) {
      case CP_INIT:
        Motion::setBehaviour(Motion::MOT_CURVED);
	hal::hw.getMainBoard()->stopDribbler();
        targetBall = Navigation::getNearestBall();
	if(targetBall != nullptr) ctx.phase = CP_RUN;
	break;
      case CP_RUN:
      {
	if(hal::hw.getMainBoard()->getBallSensorState()) break;

	if(!m_catchingBall) {
	  if(targetBall->getHealth() < 5) {
	    targetBall = nullptr;
	    ctx.phase = CP_INIT;
	    break;
	  }
	}
	
	//TODO: ( targetPosition = on (ball <-> gate) line & behind ball )

	if(targetBall != nullptr) {
	  Ball* b = targetBall;
	  Motion::setTarget(Navigation::calcBallPickupPos(b->getTransform()));
	  if(!Motion::isRunning()) Motion::start();

	  // Motion::getTargetRange() == Motion::PROXIMITY && Motion::getOrientError() < 0.08
	  if(Motion::isTargetAchieved()) {
	    std::cout << "PROXIMITY" << std::endl;

	    Motion::stop();
	    Motion::setBehaviour(Motion::MOT_BLIND);
	    Motion::setTarget(Transform({{100, 0}, 0.0}));

	    hal::hw.getMainBoard()->startDribbler();
	    if(!Motion::isRunning()) Motion::start();

	    ballPickupTimeout.start();
	    ctx.phase = CP_DONE;
	  }
	}
	break;
      }
      case CP_DONE:
	if(ballPickupTimeout.isTime()) {
	  m_catchingBall = false;
	  std::cout << "CATCH TIMEOUT" << std::endl;
	  ctx.phase = CP_INIT;
	}
	break;
    }
  }

  bool LSBallRetrieve::isRunnable() {
    return Visioning::ballDetect.size() > 0 || m_catchingBall == true;;
  }


  // Opposing goal search
  bool LSGoalLocate::isRunnable() {
    return true;
    hal::MainBoard* mb = hal::hw.getMainBoard();
    if(mb->getBallSensorState()) {
      return false;
    }

    return false;
  }

  void LSGoalLocate::init() {
    Motion::stop();
    ctx.phase = CP_INIT;
  }

  void LSGoalLocate::run() {
   switch(ctx.phase) {
     case CP_INIT:
       Motion::setBehaviour(Motion::MOT_SCAN2);
       ctx.phase = CP_RUN;
       break;
     case CP_RUN:
       // While opposing goal found
       break;
     case CP_DONE:
       break;
   }
  }

  void LSGoalShoot::init() {
    Motion::stop();
  }

  // Shoot to opposing goal
  void LSGoalShoot::run() {
    //TODO
  }

  bool LSGoalShoot::isRunnable() {
    std::cout << "LSGoalShoot - " << (Visioning::blueGoal == nullptr) << std::endl;
    return Visioning::blueGoal != nullptr;
  }

}}
