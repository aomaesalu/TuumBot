/**
 * @file RobotAlly.cpp
 * Robot ally (friendly robot) class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "RobotAlly.hpp"


namespace rtx {

  RobotAlly::RobotAlly(const RobotAlly &other):
    Robot(other.getPosition(), other.getRadius())
  {

  }

  RobotAlly::RobotAlly(const Point2D *position, const double &radius):
    Robot(position, radius)
  {

  }

  RobotAlly::RobotAlly(const double &x, const double &y, const double &radius):
    Robot(x, y, radius)
  {

  }


};
