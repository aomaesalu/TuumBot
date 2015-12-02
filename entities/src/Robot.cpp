/** @file Robot.cpp
 *  Robot class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 29 November 2015
 */

#include "Robot.hpp"

#include "tuum_platform.hpp"


namespace rtx {

  Robot::Robot() {

  }

  Robot::Robot(const Robot& robot) {
    (*this) = robot;
  }

  Robot::Robot(const Transform transform, const Blob*, const double &radius):
    Entity(transform, blob),
    Circle(radius)
  {
    // Nothing to do here
  }

  bool Robot::isAlly() const {
    std::string pattern = rtx::gC.getStr("Pattern.Ally");
    if (pattern == std::string("YB")) {
      return blob->isYellowBlue();
    } else {
      return blob->isBlueYellow();
    }
  }

  bool Robot::isOpponent() const {
    std::string pattern = rtx::gC.getStr("Pattern.Opponent");
    if (pattern == std::string("YB")) {
      return blob->isYellowBlue();
    } else {
      return blob->isBlueYellow();
    }
  }

}
