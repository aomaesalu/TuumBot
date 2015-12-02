/**
 *  @file Ball.cpp
 *  Ball class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 2 December 2015
 */

#include "Ball.hpp"


namespace rtx {

  Ball::Ball(Ball &other):
    Entity(*other.getTransform()),
    Circle(other.getRadius())
  {
    // Nothing to do here
  }

  Ball::Ball(const Transform transform, const double &radius):
    Entity(transform),
    Circle(radius)
  {
    // Nothing to do here
  }

  bool Ball::isInGoal() {
    return inGoal;
  }

  bool Ball::isOutOfGoal() {
    return !isInGoal();
  }

  bool Ball::isInBounds() {
    return inBounds;
  }

  bool Ball::isOutOfBounds() {
    return !isInBounds();
  }

  bool Ball::isKicked() {
    return kicked;
  }

  bool Ball::isNotKicked() {
    return !isKicked();
  }

  bool Ball::isValid() {
    return isInBounds() && isOutOfGoal() && isNotKicked();
  }

  bool Ball::isNotValid() {
    return !isValid();
  }

  bool Ball::setKicked(const bool &value) {
    kicked = value;
  }

  bool Ball::setInGoal(const bool &value) {
    inGoal = value;
  }

  bool Ball::setInBounds(const bool &value) {
    inBounds = value;
  }

}
