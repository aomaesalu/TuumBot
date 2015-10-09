/**
 * @file Robot.cpp
 * Description placeholder.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Robot.hpp"


namespace rtx {

  Robot::Robot():
    Entity(0, 0), Circle(0)
  {

  }

  Robot::Robot(const Robot& other):
    Entity(other.getPosition()), Circle(other.getRadius())
  {

  }

  Robot::Robot(const Point2D* position, const double& radius):
    Entity(position), Circle(radius)
  {

  }

  Robot::Robot(const double& x, const double& y, const double& radius):
    Entity(new Point2D(x, y)), Circle(radius)
  {

  }

};
