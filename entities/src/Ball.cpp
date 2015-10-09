/**
 * @file Ball.hpp
 * Ball class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Ball.hpp"


namespace rtx {

  Ball::Ball(const Ball &other):
    Entity(other.getPosition()),
    Circle(other.getRadius())
  {

  }

  Ball::Ball(const Point2D *position, const double &radius):
    Entity(position),
    Circle(radius)
  {

  }

  Ball::Ball(const double &x, const double &y, const double &radius):
    Entity(new Point2D(x, y)),
    Circle(radius)
  {

  }

};
