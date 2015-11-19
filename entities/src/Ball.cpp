/**
 * @file Ball.cpp
 * Ball class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Ball.hpp"


namespace rtx {

  Ball::Ball(Ball &other):
    Entity(*other.getTransform()),
    Circle(other.getRadius())
  {

  }

  Ball::Ball(const Transform transform, const double &radius):
    Entity(transform),
    Circle(radius)
  {

  }

};
