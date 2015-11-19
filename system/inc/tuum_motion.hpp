
#ifndef RTX_MOTION_H
#define RTX_MOTION_H

#include "rtxmath.hpp"
#include "__future__.hpp"

namespace rtx { namespace Motion {

  enum MotionType {
    MOT_SCAN,   // In-place rotation
    MOT_NAIVE,  // Turn and move
    MOT_CURVED, // Drive to target in 1 motion

    // Moves without orientation change
    MOT_STATIC,

    // Variables curve to achieve given end orientation
    MOT_COMPLEX_CURVED,

  };

  void setup();

  void process();

  int setTarget(Transform target);
  void setSpeed(int v);

  void start();
  void stop();

  void setBehaviour(MotionType mt);

  double targetDistance();
  double targetAngle();

  bool isTargetAchieved();
  bool orientationAchieved();

}}

#endif // RTX_MOTION_H
