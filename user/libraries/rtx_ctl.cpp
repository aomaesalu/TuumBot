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

#include "rtxhal.hpp"

#include "rtx_ctl.hpp"

namespace rtx { namespace ctl {

  hal::MainBoard* mb = hal::hw.getMainBoard();

  // Warmup
  bool LSInit::isRunnable() {
    return true;
  }

  int LSInit::run() {
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

  bool LSInit::isInterruptable() {
    return ctx.phase == CP_DONE;
  }


  // Ball search
  void LSBallLocate::init() {
    Motion::stop();
	    Motion::setBehaviour(Motion::MOT_COMPLEX);
	    Motion::setSpeed(30);
	    mb->stopDribbler();
	  }

	  int LSBallLocate::run() {
	    if(Visioning::ballDetect.probableSize() > 0) {
	      Motion::stop();
	      return 0;
	    } else {
	      Motion::setAimTarget(Vec2i({1, 1}));
	      if(!Motion::isRunning()) Motion::start();
	    }

	    return 0;
	  }

	  bool LSBallLocate::isRunnable() {
	    return true;
  }


  // Ball retrieval
  void LSBallRetrieve::init() {
    Motion::stop();
    Motion::setSpeed(80);
  }

  int LSBallRetrieve::run() {
    if(mb->getBallSensorState()) return 1;
    if(Visioning::ballDetect.size() <= 0) return -1;

    Ball* b = Navigation::getNearestBall();

    if(b != nullptr) {
      Vec2i pos = Navigation::calcBallPickupPos(b->getTransform()).getPosition();

      Motion::setPositionTarget(pos);
      Motion::setAimTarget(b->getTransform()->getPosition());
      mb->startDribbler();

      Transform* t = Localization::getTransform();
      double d = t->distanceTo(b->getTransform()->getPosition());

      //std::cout << d << std::endl;
      //if(d < 250) mb->startDribbler();
      //else mb->stopDribbler();

      if(!Motion::isTargetAchieved()) {
        if(!Motion::isRunning()) Motion::start();
      }
    }

    return 0;
  }

  bool LSBallRetrieve::isRunnable() {
    return Visioning::ballDetect.size() > 0 || mb->getBallSensorState();
  }


  // Opposing goal search
  bool LSGoalLocate::isRunnable() {
    if(mb->getBallSensorState()) {
      mb->startDribbler();
      return true;
    }
    return false;
  }

  void LSGoalLocate::init() {
    Motion::stop();
    Motion::setSpeed(50);
    ctx.phase = CP_INIT;
    mb->startDribbler();
  }

  int LSGoalLocate::run() {
    if(!mb->getBallSensorState()) return -1;
    if(Navigation::getOpponentGoal() != nullptr) return 1;

    Motion::setAimTarget(Vec2i({1, -1}));
    if(!Motion::isRunning()) Motion::start();
    return 0;
  }


  // Shoot to opposing goal
  void LSGoalShoot::init() {
    Motion::stop();
  }

  int LSGoalShoot::run() {
    if(!mb->getBallSensorState()) return -1;

    Goal* g = Navigation::getOpponentGoal();
    if(g == nullptr) return -1;

    //Motion::setPositionTarget(Navigation::getGoalShootPosition(g));
    Motion::setAimTarget(g->getTransform()->getPosition());
    //std::cout << g->getTransform()->getPosition().toString() << std::endl;;

    if(!Motion::isTargetAchieved()) {
      if(!Motion::isRunning()) Motion::start();
    } else {
      mb->doCoilKick();
    }

    return 0;
  }

  bool LSGoalShoot::isRunnable() {
    return mb->getBallSensorState() && Navigation::getOpponentGoal() != nullptr;
  }

  // Defend goal
  void LSGoalee::init() {
    Motion::stop();
    Motion::setBehaviour(Motion::MOT_COMPLEX);
    Motion::setSpeed(50);
  }

  int LSGoalee::run() {
    if(Visioning::ballDetect.size() <= 0) return -1;

    Ball* b = Navigation::getNearestBall();

    if(b != nullptr) {
      Vec2i pos = Navigation::calcBallPickupPos(b->getTransform()).getPosition();

      Motion::setPositionTarget(pos);
      Motion::setAimTarget(b->getTransform()->getPosition());

      Transform* t = Localization::getTransform();
      double d = t->distanceTo(b->getTransform()->getPosition());

      //std::cout << d << std::endl;
      //if(d < 250) mb->startDribbler();
      //else mb->stopDribbler();

      if(!Motion::isTargetAchieved()) {
        if(!Motion::isRunning()) Motion::start();
      }
    }

    return 0;
  }

}}
