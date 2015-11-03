/** @file tuum_motion.cpp
 *  Motion system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 1. November 2015
 */
#include "rtxhal.hpp"
#include "MotorControl.hpp"

#include "tuum_localization.hpp"
#include "tuum_motion.hpp"


namespace rtx { namespace Motion {

  MotionType motionType;

  Vec3i motionGoal;
  bool motionInProgress;
  bool targetAchieved;

  struct MotionData {
    int baseVelocity;
    double vx; double vy;
    double angleDelta; // angleDelta = M_PI*db; 0 < db < 2;

    double getHeading() {
      if( vx == 0 && vy != 0) return M_PI / 2 * (vy / abs(vy));
      else if( vy == 0 && vx != 0) return M_PI * (vx / abs(vx));
      else if( vy == 0 && vx == 0) return 0.0;
      return atan(vy / vx);
    }

    int getVelocity() {
      return (int)(baseVelocity * getVelocityFactor());
    }

    int getRotationSpeed() {
      if(angleDelta == 0) return 0;
      double v = (pow(M_PI, 2) * baseVelocity / (4 * angleDelta));
      return (int)v;
    }

    double getVelocityFactor() {
      if(vx == 0.0 && vy == 0.0) return 0.0;
      double s = vx + vy;
      if( s > 1) return 1;
      else if( s < -1 ) return 1;
      return s;
    }

    void setDirectionVector(double x, double y) {
      vx = x; vy = y;
      normalizeTo(1.0);
    }

    void normalizeTo(double factor) {
      double s = (vx + vy) / factor;
      if(s == 0.0) {
        vx = 0.0; vy = 0.0;
      } else {
        vx /= s; vy /= s;
      }
    }

  } motionData;

  double targetDistanceCondition = 10;
  double targetOrientationCondition = 0.1;

  Timer motorCmdTimer;

  void setup() {
    motionType = MOT_NAIVE;
    motionInProgress = false;
    targetAchieved = false;

    motionData = {0, 0.0, 0.0, 0.0};

    motorCmdTimer.setPeriod(1000);
    motorCmdTimer.start();

    printf("\033[1;32m");
    printf("[Motion::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    MotorControl* mco = hal::hw.getMotorControl();

    bool dirty = false;
    switch(motionType) {
      case MOT_SCAN:
        if(!motionInProgress) {
          printf("MOT_SCAN: init\n");
          motionData.baseVelocity = 15;
          motionData.setDirectionVector(0.0, 0.0);
          motionData.angleDelta = 0.25;

          motionInProgress = true;
          targetAchieved = false;
          dirty = true;
        }
        break;
      case MOT_NAIVE:

        // Correct orientation => motionData
        // then
        // Forward motion => motionData
        break;
      case MOT_CURVED:
        // Curved motion => motionData
        break;
      case MOT_STATIC:
        // Rotational motion => motionData
        break;
      case MOT_COMPLEX_CURVED:
        // Complex rotational motion => motionData
        break;
      default:
        motionData = {0, 0.0, 0.0};
        motionInProgress = false;
        dirty = true;
        break;
    }

    if(motionInProgress || dirty) {
      if((!isTargetAchieved() && motorCmdTimer.isTime()) || dirty) {
        //mco->setMovement(motionData.x, motionData.y, motionData.z, motionData.a);
        printf("[rtx::Motion]mco->omniDrive(%i, %g, %i)\n", motionData.getVelocity(), motionData.getHeading(), motionData.getRotationSpeed());
        motorCmdTimer.start();
      }
    }

  }

  int setTarget(Vec3i target) {
    motionGoal = target;
  }

  void setSpeed(int v) {
    motionData.baseVelocity = v;
    // Recalculate velocity parameters?
  }

  void setBehaviour(MotionType mt) {
    motionType = mt;
    motionInProgress = false;
  }

  double targetDistance() {
    return Localization::getPosition().distanceTo(motionGoal.x, motionGoal.y);
  }

  double targetAngle() {
    return 0.0; // TODO: dot(Localization::getOrientation(), motionGoal);
  }

  bool isTargetAchieved() {
    return targetAchieved;
  }

  bool orientationAchieved() {
    return targetAngle() < targetOrientationCondition;
  }

}}
