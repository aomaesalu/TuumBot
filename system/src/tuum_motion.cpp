
#include "rtxhal.hpp"
#include "MotorControl.hpp"

#include "tuum_localization.hpp"
#include "tuum_motion.hpp"


namespace rtx { namespace Motion {

  MotionType motionType;
  Vec3i motionGoal;
  Vec3<int, double, double> velocityData; // (v, direction, rotSpeed)

  double targetDistanceCondition = 10;
  double targetOrientationCondition = 0.1;

  Timer motorCmdTimer;

  void setup() {
    motionType = MOT_NAIVE;
    velocityData = {0, 0.0, 0.0};

    motorCmdTimer.setPeriod(1000);
    motorCmdTimer.start();

    printf("\033[1;32m");
    printf("[Motion::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    MotorControl* mco = hal::hw.getMotorControl();
    // mco->setMovement(speed, dir_vec, rotSpeed)

    bool dirty = false;

    // Updates velocity data
    switch(motionType) {
      case MOT_SCAN:
        velocityData.x = 0;
        velocityData.y = 0.0;
        break;
      case MOT_NAIVE:
        // Correct orientation => velocityData
        // or
        // Forward motion => velocityData
        break;
      case MOT_CURVED:
        // Curved motion => velocityData
        break;
      case MOT_STATIC:
        // Rotational motion => velocityData
        break;
      case MOT_COMPLEX_CURVED:
        // Complex rotational motion => velocityData
        break;
      default:
        velocityData = {0, 0.0, 0.0};
        dirty = true;
        break;
    }

    if(!targetAchieved() & (motorCmdTimer.isTime() || dirty)) {
      // mco->setMovement(velocityData.x, velocityData.y, velocityData.z)
      printf("[rtx::Motion]mco->setMovement(?)\n");
      motorCmdTimer.start();
    }
  }

  int setTarget(Vec3i target) {
    motionGoal = target;
  }

  void setSpeed(int v) {
    velocityData.x = v;
  }

  void setType(MotionType mt) {
    motionType = mt;
  }

  double targetDistance() {
    return Localization::getPosition().distanceTo(motionGoal.x, motionGoal.y);
  }

  double targetAngle() {
    return 0.0; // TODO: dot(Localization::getOrientation(), motionGoal);
  }

  bool targetAchieved() {
    return targetDistance() < targetDistanceCondition;
  }

  bool orientationAchieved() {
    return targetAngle() < targetOrientationCondition;
  }

}}
