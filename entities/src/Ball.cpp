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
    Entity(*other.getTransform(), *other.getBlob()),
    Circle(other.getRadius())
  {
    initialiseAttributes();
  }

  Ball::Ball(const Transform transform, Vision::Blob *blob, const double &radius):
    Entity(transform, blob),
    Circle(radius)
  {
    initialiseAttributes();
  }

  void Ball::initialiseAttributes() {
    kicked = false;
    inGoal = false;
    inBounds = true;
  }

  bool Ball::isInGoal() const {
    return inGoal;
  }

  bool Ball::isOutOfGoal() const {
    return !isInGoal();
  }

  bool Ball::isInBounds() const {
    return inBounds;
  }

  bool Ball::isOutOfBounds() const {
    return !isInBounds();
  }

  bool Ball::isKicked() const {
    return kicked;
  }

  bool Ball::isNotKicked() const {
    return !isKicked();
  }

  bool Ball::isValid() const {
    return isInBounds() && isOutOfGoal() && isNotKicked();
  }

  bool Ball::isNotValid() const {
    return !isValid();
  }

  void Ball::setKicked(const bool &value) {
    kicked = value;
  }

  void Ball::setInGoal(const bool &value) {
    inGoal = value;
  }

  void Ball::setInBounds(const bool &value) {
    inBounds = value;
  }

}
