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

namespace rtx {


  namespace Motion {

    void TwitchScan::_init() {
      wait_for_vision = true;

      motionTimer.setPeriod(160);
      visionTimer.setPeriod(400);
    }

    void TwitchScan::init() {
      _init();
    }

    void TwitchScan::init(int sp_vision, int sp_scan) {
      m_spv = sp_vision;
      m_sps = sp_scan;
      _init();
    }

    void TwitchScan::run() {
      if(!wait_for_vision) {
	if(motionTimer.isTime()) {
          Motion::setAimTarget(Vec2i({0, 1}));
	  motionData.manualRotGear = {m_spv, 3.14};
	  Motion::start();
	  wait_for_vision = true;
	  visionTimer.start();
	}
      } else {
	if(visionTimer.isTime()) {
	  Motion::setAimTarget(Vec2i({0, 1}));
	  motionData.manualRotGear = {m_sps, 3.14};
	  Motion::start();
	  wait_for_vision = false;
	  motionTimer.start();
	}
      }
    }

  }

}

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
    twitchScanner.init(5, 30);
    mb->stopDribbler();
  }

  int LSBallLocate::run() {
    if(Visioning::ballDetect.size() > 0) {
      mb->startDribbler();
      Motion::stop();
      return 0;
    } else {
      twitchScanner.run();
    }

    return 0;
  }

  bool LSBallLocate::isRunnable() {
    return true;
  }


  // Navigate to ball
  void LSBallNavigator::init() {
    Motion::stop();

    Ball* b = Navigation::getNearestBall(); 
    if(b != nullptr)
      std::cout << "Navigate to " << b->toString() << std::endl;
  }

  int LSBallNavigator::run() {
    Ball* b = nullptr; 
    if(mb->getBallSensorState()) goto OK;
    if(Visioning::ballDetect.size() <= 0) goto ERR;

    b = Navigation::getNearestBall();

    if(b != nullptr) {
      Vec2i pos = Navigation::calcBallPickupPos(b->getTransform()).getPosition();

      Motion::setPositionTarget(pos);
      Motion::setAimTarget(b->getTransform()->getPosition());
      mb->startDribbler();

      //std::cout << d << std::endl;
      //if(d < 250) mb->startDribbler();
      //else mb->stopDribbler();

      if(!Motion::isTargetAchieved()) {
        if(!Motion::isRunning()) Motion::start();
      } else {
        goto OK;
      }
    } else {
      Motion::stop();
    }

    return 0;
OK:
    Motion::stop();
    return 1;
ERR:
    Motion::stop();
    return -1;
  }

  bool LSBallNavigator::isRunnable() {
    return Visioning::ballDetect.size() > 0 || mb->getBallSensorState();
  }


  // Ball pickup
  void LSBallPicker::init() {
    Motion::stop();
    mb->startDribbler();
  }

  int LSBallPicker::run() {
    Ball* b = nullptr; 
    if(mb->getBallSensorState()) goto OK;
    if(Visioning::ballDetect.size() <= 0) goto ERR;

    b = Navigation::getNearestBall();

    if(b != nullptr) {
      double dD = Motion::VLS_DIST.low;
      Transform* t = b->getTransform();
      Transform* me = Localization::getTransform();
      Vec2f avf = (t->getPosition() - me->getPosition()).getNormalized();
      Motion::setPositionTarget(t->getPosition() + (avf*dD).toInt());
      Motion::setAimTarget(t->getPosition() + (avf*1.1*dD).toInt());

      if(me->getPosition().distanceTo(t->getPosition()) > dD) return -1;

      mb->startDribbler();
      if(!Motion::isRunning()) Motion::start();
    } else {
      Motion::stop();
    }

    return 0;
OK:
    Motion::stop();
    return 1;
ERR:
    Motion::stop();
    mb->stopDribbler();
    return -1;
  }

  bool LSBallPicker::isRunnable() {
    Ball* b = Navigation::getNearestBall();
    if(b == nullptr) return false;

    Transform* t = Localization::getTransform();
    double d = t->distanceTo(b->getTransform()->getPosition());
    if(d > (Motion::VLS_DIST.mn + Motion::GRS_MOV.mn.step) ) return false;

    return true;
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
    ctx.phase = CP_INIT;
    twitchScanner.init(10, 30);
    mb->startDribbler();
  }

  int LSGoalLocate::run() {
    if(!mb->getBallSensorState()) goto ERR;
    if(Navigation::getOpponentGoal() != nullptr) goto OK;

    twitchScanner.run();

    return 0;
OK:
    Motion::stop();
    return 1;
ERR:
    Motion::stop();
    return -1;
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
;
    //if(fabs(Motion::getDeltaOrientation()) < 0.030) mb->doCoilKick();

    if(!Motion::isTargetAchieved()) {
      if(!Motion::isRunning()) Motion::start();
    } else {
      Motion::stop();
      if(mb->getBallSensorState()) mb->doCoilKick();
    }

    return 0;
  }

  bool LSGoalShoot::isRunnable() {
     
    return Navigation::getOpponentGoal() != nullptr && mb->getBallSensorState();
  }

  // Defend goal
  void LSGoalee::init() {
    Motion::stop();
    Motion::setBehaviour(Motion::MOT_COMPLEX);
  }

  int LSGoalee::run() {
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
    } else {
      Motion::stop();
    }

    return 0;
  }

}}
