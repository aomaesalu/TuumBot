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
    std::string pattern = rtx::gC.getStr("Pattern.AllyGoal");
    if (pattern == std::string("Y")) {
      return blob->isYellow();
    } else {
      return blob->isBlue();
    }
  }

  bool Goal::isOpponent() const {
    std::string pattern = rtx::gC.getStr("Pattern.OpponentGoal");
    if (pattern == std::string("Y")) {
      return blob->isYellow();
    } else {
      return blob->isBlue();
    }
  }

}
