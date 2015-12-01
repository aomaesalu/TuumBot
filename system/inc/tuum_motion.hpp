
#ifndef RTX_MOTIONING_H
#define RTX_MOTIONING_H

#include "rtxmath.hpp"

#include "syscore/MotionData.hpp"

#include "tuum_platform.hpp"

namespace rtx { namespace Motion {

  enum MotionType {
    MOT_SCAN,   // In-place rotation
    MOT_NAIVE,  // Turn and move
    MOT_CURVED, // Drive to target in 1 motion
    MOT_COMPLEX,

    // Variables curve to achieve given end orientation
    MOT_COMPLEX_CURVED,

    MOT_BLIND,
    MOT_SCAN2,
    MOT_AIM,
  };

  enum MotionPhase {
    MOP_STANDBY,
    MOP_INIT,
    MOP_RUN,
    MOP_DONE
  };

  struct MotionContext {
    MotionPhase phase;
  };

  extern MotionData motionData;

  const double targetDistanceCondition = GRS_MOV.low.step;
  const double targetOrientationCondition = GRS_ROT.low.step;

  void setup();
  void process();

  // Motion target API
  void setPositionTarget(Vec2i);
  void setAimTarget(Vec2i);
  void setTarget(Transform);


  // State control API
  void start();
  void stop();

  void setSpeed(int v);
  void setBehaviour(MotionType mt);


  // State response API
  bool isRunning();

  Vec2i getTargetPosition();
  double getTargetOrientation();
  Transform getTargetTransform();

  bool isTargetAchieved();
  bool isOrientationAchieved();


  // State offset getters
  double getDeltaDistance();
  double getDeltaOrientation();

  Transform getDeltaTransform();
  double getOrientError();

  int getTargetRange();

}}

#endif // RTX_MOTIONING_H
