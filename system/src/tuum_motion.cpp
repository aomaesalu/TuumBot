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

  MotionType motionType;
  Transform motionGoal;
  MotionContext motionCtx;
  MotionData motionData;

  bool targetAchieved = false;
  bool motionActive = false;

  Timer motorCmdTimer;

  void setup() {
    motionType = MOT_COMPLEX;
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
      case MOT_COMPLEX:
        switch(motionCtx.phase) {
          case MOP_INIT:
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
    //double orientDelta = pos.getOrientation();
    if(motionData.setAimTarget(pos) < 0) return;
    _setTarget();
  }

  void setTarget(Transform target) {
    Transform t = getTargetTransform() - target;
    double mag = t.getPosition().getMagnitude();

    // If offset is lower than lowest gear's motion resolution skip target update.
    if(mag < GRS_MOV.low.step && t.o < GRS_ROT.low.step) return;

    motionData.setTransformTarget(target);
    std::cout << "[Motion]Set target: "
              << motionData.getTargetPos().toString()
	      << std::endl;

    _setTarget();
  }

  // == State control API
  void start() {
    if(Motion::isRunning()) return;
    motionCtx.phase = MOP_INIT;
  }

  void stop() {
    if(!Motion::isRunning()) return;
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


  // State response API
  bool isRunning() {
    return motionCtx.phase == MOP_RUN && targetAchieved == false;
  }

  Vec2i getTargetPosition() {
    return motionData.getTargetPos();
  }

  double getTargetOrientation() {
    return motionData.getTargetOrient();
  }

  Transform getTargetTransform() {
    return motionData.getTargetTransform();
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

    //FIXME: These should be taken from gear step
    const int d = 50;
    const double d_o = 0.08;

    //TODO: Get relative target vectors
    Vec2i v = data->getTargetPos();
    double O = data->getTargetOrient();

    if(data->posTargetSet) {
      if(fabs(t->getX() - v.x) > d ||
	 fabs(t->getY() - v.y) > d) return 0.0;
    }

    if(data->aimTargetSet) {
      if(fabs(t->o - O) > d_o) return 0.0;
    }

    return 1.0;
  }

  bool isTargetAchieved() {
    if(!targetAchieved) {

      Transform* t = Localization::getTransform();
      double p = stateProbability(t, &motionData);

      if(p > 0.3) targetAchieved = true;
    }

    return targetAchieved;
  }

  bool isOrientationAchieved() {
    return getDeltaOrientation() < targetOrientationCondition;
  }


  // State offset getters
  double getDeltaDistance() {
    return motionData.getDeltaDist();
  }

  double getDeltaOrientation() {
    return motionData.getDeltaOrient();
  }

  Transform getDeltaTransform() {
    return getTargetTransform() - *Localization::getTransform();
  }

  double getOrientError() {
    return fabs(getDeltaOrientation());
  }

  int getTargetRange() {
    double mag = motionData.getDeltaDist();
    if(mag <= VLS_DIST.mn) return VLS_DIST.mn; // Proximity
    else if(mag <= VLS_DIST.low) return VLS_DIST.low;
    else if(mag <= VLS_DIST.med) return VLS_DIST.med;
    else if(mag <= VLS_DIST.high) return VLS_DIST.high;
    else return VLS_DIST.mx;
  }

}}
