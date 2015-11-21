/** @file tuum_motion.cpp
 *  Motion system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 1. November 2015
 */

#include <iostream>

#include "rtxhal.hpp"
#include "MotorControl.hpp"

#include "tuum_localization.hpp"
#include "tuum_motion.hpp"


//TODO: Implement state machine & controllers
//TODO: Refactor MotionData structure to separate source
//TODO: Refactor code in overall
namespace rtx { namespace Motion {

  enum MotionPhase {
    MOP_STANDBY,
    MOP_INIT,
    MOP_RUN,
    MOP_DONE
  };

  struct MotionContext {
    MotionPhase phase;
  } motionCtx;

  MotionType motionType;

  Transform motionGoal; // (x, y, orient)
  bool motionInProgress;
  bool targetAchieved;

  struct MotionData {
    int baseVelocity;
    Vec2f dV;
    double orientDelta;

    double _speed;
    double _r_speed;
    double _heading;

    double getHeading() {
      return _heading;
    }

    int getSpeed() {
      return _speed;
    }

    int getRotationSpeed() {
      return _r_speed;
    }

    void calc() {
      _speed = (int)(baseVelocity);
      _r_speed = (int)(baseVelocity*0.6*getOVF());

      //Vec2f _dV = dV;
      //_dV.rotate(-orientDelta);
      _heading = atan2(dV.y, dV.x);
    }

    void applyFactors() {
      _speed *= getVF();
      _r_speed *= getOVF();
    }

    void clamp() {
      if(fabs(_r_speed) < MIN_ROT_SPEED && _r_speed != 0) {
	_r_speed = MIN_ROT_SPEED * (_r_speed < 0 ? -1 : 1);
      }
    }

    double getVF() {
      double mag = dV.getMagnitude();

      if(mag >= CLOSE_RANGE) return 1;
      return mag / CLOSE_RANGE;
    }

    double getOVF() {
      double oD = fabs(orientDelta);

      int sign = 1;
      if(orientDelta < 0) sign = -1;

      if(oD <= 0.08) return 0;
      if(oD > 0.50) return sign;

      double v = (oD - MN_ROT_STEP) * sign / (0.5 - MN_ROT_STEP);
      
      return v;
    }

    void setDirectionVector(double x, double y) {
      dV.x = x; dV.y = y;
    }

    void setDirectionVector(Vec2i vec) {
      dV.x = vec.x; dV.y = vec.y;
    }

  } motionData;

  // Motion Controllers
  class MotionController {
  private:

  public:
    virtual void init() {};
    virtual void run() {};

  };

  class MCOrientScan : public MotionController {
  private:


  public:
    void init() {

    }

    void run() {

    }

  };

  // Avg orient, orient offset
  // Set target orient: orient - offset
  // 
  // while:
  //   if target achieved:
  //     target orient : orient (+/-) offset

  MotionController* MCO;

  bool motionActive = false;

  double targetDistanceCondition = 10;
  double targetOrientationCondition = 0.1;

  Timer motorCmdTimer;
  Timer timer0;

  void setup() {
    motionType = MOT_NAIVE;
    motionInProgress = false;
    targetAchieved = false;

    motionData = {0, 0.0, 0.0, 0.0};

    motorCmdTimer.setPeriod(100);
    motorCmdTimer.start();

    printf("\033[1;32m");
    printf("[Motion::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    hal::MotorControl* mco = hal::hw.getMotorControl();

    Transform deltaT;

    switch(motionType) {
      case MOT_SCAN:
	switch(motionCtx.phase) {
	  case MOP_INIT:
	    motionData.baseVelocity = 0;
	    motionData.setDirectionVector(0.0, 0.0);
	    motionData.orientDelta = M_PI;

	    targetAchieved = false;
	    motionCtx.phase = MOP_RUN;
	    break;
	}
        break;
      case MOT_NAIVE:
        // Correct orientation => motionData
        // then
        // Forward motion => motionData
        break;
      case MOT_BLIND:
        switch(motionCtx.phase) {
	  case MOP_INIT:
	    deltaT = motionGoal - (*Localization::getTransform());

	    motionData.baseVelocity = 35;
	    motionData.orientDelta = deltaT.o;
	    motionData.setDirectionVector(deltaT.getPosition());

	    motionData.calc();
	    motionData.clamp();

	    targetAchieved = false;
            motionCtx.phase = MOP_RUN;

            printf("[rtx::Motion]mco->omniDrive(%i, %g, %i)\n", motionData.getSpeed(), motionData.getHeading(), motionData.getRotationSpeed());
	    break;
	  case MOP_RUN:
	    break;
	  case MOP_DONE:
	    break;
        }
      case MOT_CURVED:
        switch(motionCtx.phase) {
	  case MOP_INIT:
	    deltaT = motionGoal - (*Localization::getTransform());

	    motionData.baseVelocity = 25;
	    motionData.orientDelta = deltaT.o;
	    motionData.setDirectionVector(deltaT.getPosition());

	    motionData.calc();
	    motionData.applyFactors();
	    motionData.clamp();

	    targetAchieved = false;
            motionCtx.phase = MOP_RUN;

            printf("[rtx::Motion]mco->omniDrive(%i, %g, %i)\n", motionData.getSpeed(), motionData.getHeading(), motionData.getRotationSpeed());

	    break;
	  case MOP_RUN:
	    break;
	  case MOP_DONE:
	    break;
        }
        break;
      case MOT_SCAN2:
        switch(motionCtx.phase) {
	  case MOP_INIT:
	    motionData.baseVelocity = 30;
	    motionData.orientDelta = -1.57;
	    motionData.setDirectionVector(Vec2i({0, 0}));

	    motionData.calc();
	    motionData.applyFactors();
	    motionData.clamp();

	    timer0.setPeriod(2000);
	    timer0.start();

	    targetAchieved = false;
            motionCtx.phase = MOP_RUN;

            printf("[rtx::Motion]mco->omniDrive(%i, %g, %i)\n", motionData.getSpeed(), motionData.getHeading(), motionData.getRotationSpeed());
	    break;
	  case MOP_RUN:
	    if(timer0.isTime()) {
              motionData.orientDelta *= motionData.orientDelta < 0 ? 1 : -1;
	      timer0.start();
	    }
	    break;
	  case MOP_DONE:
	    break;
        }
	break;
      case MOT_STATIC:
        // Rotational motion => motionData
        break;
      case MOT_COMPLEX_CURVED:
        // Complex rotational motion => motionData
        break;
    }

    switch(motionCtx.phase) {
      case MOP_RUN:
	if(!isTargetAchieved()) {
	  if(motorCmdTimer.isTime()) {
	    //mco->OmniDrive(motionData.getSpeed(), motionData.getHeading(), motionData.getRotationSpeed());
	    motorCmdTimer.start();
	  }
	} else {
	  printf("[Motion]Target achieved.\n");
	  std::cout << "Target: " << motionGoal.toString() << std::endl;
	  motionCtx.phase = MOP_DONE;
	}

	break;
      default:
	break;
    }

  }

  void setTarget(Transform target) {
    Transform t = motionGoal - target;
    double mag = t.getPosition().getMagnitude();
    if(mag < MN_DIST_STEP && t.o < MN_ROT_STEP) return;

    printf("[Motion::setTarget]dT: %g, %g\n", t.getPosition().getMagnitude(), t.o);
    printf("[Motion::setTarget]T: %i, %i, %g\n", target.getX(), target.getY(), target.o);

    motionCtx.phase = MOP_STANDBY;
    motionGoal = target;
    targetAchieved = false;
  }

  bool isRunning() {
    return motionCtx.phase == MOP_RUN && targetAchieved == false;
  }

  void start() {
    motionCtx.phase = MOP_INIT;
  }

  Transform getDeltaTransform() {
    return motionGoal - *Localization::getTransform();
  }

  double getOrientError() {
    return fabs(motionGoal.o - Localization::getTransform()->o);
  }

  int getTargetRange() {
    double mag = getDeltaTransform().getPosition().getMagnitude();
    if(mag <= PROXIMITY) return PROXIMITY;
    else if(mag <= CLOSE_RANGE) return CLOSE_RANGE;
    else if(mag <= MID_RANGE) return MID_RANGE;
    else return LONG_RANGE;
  }

  void stop() {
    hal::hw.getMotorControl()->OmniDrive(0, 0, 0);
    motionCtx.phase = MOP_STANDBY;
  }

  void setSpeed(int v) {
    motionData.baseVelocity = v;
    // Recalculate velocity parameters?
  }

  void setBehaviour(MotionType mt) {
    stop();
    motionType = mt;
    motionActive = false;
  }

  double targetDistance() {
    return Localization::getTransform()->getPosition().distanceTo(motionGoal.getPosition());
  }

  double targetAngle() {
    return 0.0; // TODO: dot(Localization::getOrientation(), motionGoal);
  }

  //FIXME:
  double stateProbability(Transform* t1, Transform* t2) {
    //double px = gaussian_probability(t1->getX(), 30, t2->getX());
    //double py = gaussian_probability(t1->getY(), 30, t2->getY());
    //double c  = 3.14/180;
    //double po = gaussian_probability(t1->getOrientation()*c, 40, t2->getOrientation()*c);

    /*std::cout << "px=" << px
              << ", py=" << py
	      << ", po= " << po
	      << std::endl;
    */

    double po;
    const int d = 65;
    if(fabs(t1->getX() - t2->getX()) < d &&
	fabs(t1->getY() - t2->getY()) < d &&
	fabs(t1->o - t2->o) < 0.09) {
      po = 1.0;
    } else {
      po = 0.0;
    }
    return po;
  }

  bool isTargetAchieved() {
    if(!targetAchieved) {
      // (transform - target) <= uncertainty

      Transform* t = Localization::getTransform();
      double p = stateProbability(t, &motionGoal);

      //std::cout << "P = " << p << std::endl;

      if(p > 0.3) targetAchieved = true;
    }

    return targetAchieved;
  }

  bool orientationAchieved() {
    return targetAngle() < targetOrientationCondition;
  }

}}
