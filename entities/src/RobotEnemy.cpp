/**
 * @file RobotEnemy.cpp
 * Enemy robot class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "RobotEnemy.hpp"


namespace rtx {

  RobotEnemy::RobotEnemy(const RobotEnemy &other):
    Robot(other.getPosition(), other.getRadius())
  {

  }

  RobotEnemy::RobotEnemy(const Point2D *position, const double &radius):
    Robot(position, radius)
  {

  }

  RobotEnemy::RobotEnemy(const double &x, const double &y,
                         const double &radius):
    Robot(x, y, radius)
  {

  }


};
