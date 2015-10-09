/**
 * @file RobotSelf.cpp
 * Robot self class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "RobotSelf.hpp"


namespace rtx {

  RobotSelf::RobotSelf(const RobotSelf &other):
    Robot(other.getPosition(), other.getRadius())
  {

  }

  RobotSelf::RobotSelf(const Point2D *position, const double &radius):
    Robot(position, radius)
  {

  }

  RobotSelf::RobotSelf(const double &x, const double &y, const double &radius):
    Robot(x, y, radius)
  {

  }


};
