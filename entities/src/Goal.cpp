/**
 *  @file Goal.cpp
 *  Goal class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 2 December 2015
 */

#include "Goal.hpp"


namespace rtx {

  Goal::Goal(Goal &other):
    Entity(*other.getTransform(), other.getBlob()),
    Rectangle(other.getWidth(), other.getLength()),
    blob(other.getBlob())
  {

  }

  Goal::Goal(const Transform transform, const Blob *blob, const double &width, const double &length):
    Entity(transform, blob),
    Rectangle(width, length)
  {

  }

  bool Goal::isBlue() const {
    return blob->isBlue();
  }

  bool Goal::isYellow() const {
    return blob->isYellow();
  }

  bool Goal::isAlly() const {
    // TODO
  }

  bool Goal::isOpponent() const {
    // TODO
  }

}
