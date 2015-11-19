/** @file Robot.cpp
 *  Robot class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.1
 */

#include "Robot.hpp"


namespace rtx {

  Robot::Robot() {

  }

  Robot::Robot(const Robot& robot) {
    (*this) = robot;
  }

  Robot::Robot(const Transform transform, const double &radius):
    Entity(transform), Circle(radius)
  {

  }

};
