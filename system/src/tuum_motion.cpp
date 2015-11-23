/** @file tuum_motion.cpp
 *  Motion system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 1. November 2015
 */

#include <iostream>

#include "rtxhal.hpp"

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
    Vec2i positionTarget;
    Vec2i aimTarget;

    bool posTargetSet = false;
    bool aimTargetSet = false;

    void clear() {
      posTargetSet = false;
      aimTargetSet = false;
      positionTarget = {0, 0};
      aimTarget = {0, 0};
    }

    void setPosTarget(Vec2i vec) {
      positionTarget = vec;
      posTargetSet = true;
    }

    void setAimTarget(Vec2i vec) {
      aimTarget = vec;
      aimTargetSet = true;
    }

    void setTransformTarget(Transform t) {
      positionTarget = t.getPosition();
      std::cout << "CREATE AIM TARGET TO: " << t.getOrientation() << std::endl;
      aimTarget = positionTarget + Vec2i::fromOrientation(t.getOrientation());
      std::cout << positionTarget.toString()
	        << "; " << aimTarget.toString()
		<< "; AIM ORI: " << (aimTarget - positionTarget).getOrientation()
		<< std::endl;
      posTargetSet = true;
      aimTargetSet = true;
    }

    Vec2i getTargetPosition() {
      if(posTargetSet)
        return positionTarget;
      else
        return Localization::getTransform()->getPosition();
    }

    double getTargetOrientation() {
      if(aimTargetSet)
        return (aimTarget - positionTarget).getOrientation();
      else
        return Localization::getTransform()->getOrientation();
    }

    // Omni drive parameter calculation structure
    int baseVelocity;

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

      if(posTargetSet)
        _speed = (int)(baseVelocity);
      else
        _speed = 0;

      if(aimTargetSet)
        _r_speed = (int)(baseVelocity*0.6*getOVF());
      else
        _r_speed = 0;

      //Vec2f _dV = dV;
      //_dV.rotate(-orientDelta);
      _heading = 0; //(positionTarget - Localization::getTransform()->getPosition()).getOrientation();
    }

    void applyFactors() {
      _speed *= getVF();
      //_r_speed *= getOVF();
    }

    void clamp() {
      if(fabs(_r_speed) < MIN_ROT_SPEED && _r_speed != 0) {
        _r_speed = MIN_ROT_SPEED * (_r_speed < 0 ? -1 : 1);
      }
    }

    double getVF() {
      double mag = (positionTarget - Localization::getTransform()->getPosition()).getMagnitude();

      if(mag >= CLOSE_RANGE) return 1;
      return mag / CLOSE_RANGE;
    }

    double getOVF() {
      double orientDelta = (aimTarget - positionTarget).getOrientation();
      double oD = fabs(orientDelta);

      int sign = orientDelta < 0 ? -1 : 1;

      if(oD <= MN_ROT_STEP) return 0;
      if(oD > 0.50) return sign;

      double v = (oD - MN_ROT_STEP) * sign / (0.5 - MN_ROT_STEP);
      return v;
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

    motionData.clear();

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

            targetAchieved = false;
            motionCtx.phase = MOP_RUN;
            break;
        }
        break;
      case MOT_COMPLEX:
        switch(motionCtx.phase) {
          case MOP_INIT:
            motionData.baseVelocity = 30;

            motionData.calc();
            motionData.applyFactors();
            motionData.clamp();

            targetAchieved = false;
            motionCtx.phase = MOP_RUN;

            break;
        }
        break;
    }

    switch(motionCtx.phase) {
      case MOP_RUN:
        if(!isTargetAchieved()) {
          if(motorCmdTimer.isTime()) {
            //printf("[rtx::Motion]mco->omniDrive(%i, %g, %i)\n", motionData.getSpeed(), motionData.getHeading(), motionData.getRotationSpeed());
            mco->OmniDrive(motionData.getSpeed(), motionData.getHeading(), motionData.getRotationSpeed());
            motorCmdTimer.start();
          }
        } else {
	  stop();
          printf("[Motion]Target achieved.\n");
          motionCtx.phase = MOP_DONE;
        }

        break;
      default:
        break;
    }

  }

  // Target API
  void _setTarget() {
    motionCtx.phase = MOP_STANDBY;
    targetAchieved = false;
  }

  void setPositionTarget(Vec2i pos) {
    motionData.setPosTarget(pos);
    _setTarget();
  }

  void setAimTarget(Vec2i pos) {
    double orientDelta = pos.getOrientation();
    motionData.setAimTarget(pos);
    _setTarget();
  }

  void setTarget(Transform target) {
    Transform t = getTargetTransform() - target;
    double mag = t.getPosition().getMagnitude();
    if(mag < MN_DIST_STEP && t.o < MN_ROT_STEP) return;

    motionData.setTransformTarget(target);
    std::cout << "[Motion]Set target: "
              << motionData.getTargetPosition().toString()
	      << std::endl;

    _setTarget();
  }

  // == State control API
  void start() {
    if(Motion::isRunning()) return;
    motionCtx.phase = MOP_INIT;
  }

  void stop() {
    hal::hw.getMotorControl()->OmniDrive(0, 0, 0);
    motionData.clear();
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


  // == State response API
  bool isRunning() {
    return motionCtx.phase == MOP_RUN && targetAchieved == false;
  }

  Vec2i getTargetPosition() {
    return motionData.getTargetPosition();
  }

  double getTargetOrientation() {
    return motionData.getTargetOrientation();
  }

  Transform getTargetTransform() {
    return Transform({motionData.getTargetPosition(), motionData.getTargetOrientation()});
  }



  //FIXME:
  double stateProbability(Transform* t, MotionData* data) {
    //double px = gaussian_probability(t1->getX(), 30, t2->getX());
    //double py = gaussian_probability(t1->getY(), 30, t2->getY());
    //double c  = 3.14/180;
    //double po = gaussian_probability(t1->getOrientation()*c, 40, t2->getOrientation()*c);

    /*std::cout << "px=" << px
	      << ", py=" << py
	<< ", po= " << po
	<< std::endl;
    */

    const int d = 65;

    Vec2i v = data->getTargetPosition();
    double O = data->getTargetOrientation();

    if(data->posTargetSet) {
      if(fabs(t->getX() - v.x) > d ||
	 fabs(t->getY() - v.y) > d) return 0.0;
    }

    if(data->aimTargetSet) {
      if(fabs(t->o - O) > 0.08) return 0.0;
    }

    return 1.0;
  }

  bool isTargetAchieved() {
    if(!targetAchieved) {
      // (transform - target) <= uncertainty

      Transform* t = Localization::getTransform();
      double p = stateProbability(t, &motionData);

      //std::cout << "P = " << p << std::endl;

      if(p > 0.3) targetAchieved = true;
    }

    return targetAchieved;
  }

  bool isOrientationAchieved() {
    return getDeltaOrientation() < targetOrientationCondition;
  }


  // State offset getters
  double getDeltaDistance() {
    return Localization::getTransform()->getPosition().distanceTo(motionGoal.getPosition());
  }

  double getDeltaOrientation() {
    return motionData.getTargetOrientation() - Localization::getTransform()->o;
  }

  Transform getDeltaTransform() {
    return getTargetTransform() - *Localization::getTransform();
  }

  double getOrientError() {
    return fabs(getDeltaOrientation());
  }

  int getTargetRange() {
    double mag = getDeltaTransform().getPosition().getMagnitude();
    if(mag <= PROXIMITY) return PROXIMITY;
    else if(mag <= CLOSE_RANGE) return CLOSE_RANGE;
    else if(mag <= MID_RANGE) return MID_RANGE;
    else return LONG_RANGE;
  }

}}





/*

case MOT_AIM:
  switch(motionCtx.phase) {
    case MOP_INIT:
      deltaT = motionGoal - (*Localization::getTransform());

      motionData.baseVelocity = 25;
      motionData.setDirectionVector(0.0, 0.0);
      motionData.orientDelta = deltaT.o;

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

case MOT_COMPLEX_CURVED:
  // Complex rotational motion => motionData
  break;
*/
