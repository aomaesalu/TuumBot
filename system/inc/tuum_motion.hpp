
#ifndef RTX_MOTION_H
#define RTX_MOTION_H

#include "rtxmath.hpp"
#include "__future__.hpp"

namespace rtx { namespace Motion {

  const int LONG_RANGE = 600;
  const int MID_RANGE = 300;
  const int CLOSE_RANGE = 150;
  const int PROXIMITY = 40;

  const int MIN_SPEED = 5;
  const int MIN_ROT_SPEED = 5;

  const int MN_DIST_STEP = 15;
  const double MN_ROT_STEP = 0.05;

  enum MotionType {
    MOT_SCAN,   // In-place rotation
    MOT_NAIVE,  // Turn and move
    MOT_CURVED, // Drive to target in 1 motion

    // Moves without orientation change
    MOT_STATIC,

    // Variables curve to achieve given end orientation
    MOT_COMPLEX_CURVED,

    MOT_BLIND,
    MOT_SCAN2,
  };

  void setup();
  void process();

  void setTarget(Transform target);

  int getTargetRange();
  double getOrientError();

  void setSpeed(int v);

  void start();
  void stop();

  bool isRunning();

  void setBehaviour(MotionType mt);

  double targetDistance();
  double targetAngle();

  bool isTargetAchieved();
  bool orientationAchieved();


}}

#endif // RTX_MOTION_H
