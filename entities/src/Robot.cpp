/** @file Robot.cpp
 *  Robot class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.1
 *  @date 26 November 2015
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

  Color Robot::getColor() const {
    return color;
  }

  bool Robot::isYellowBlue() const {
    return color == ROBOT_YELLOW_BLUE;
  }

  bool Robot::isBlueYellow() const {
    return color == ROBOT_BLUE_YELLOW;
  }

}
