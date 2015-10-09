/**
 * @file Field1vs1.cpp
 * 1vs1 football game field class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Field1vs1.hpp"


namespace rtx {

  Field1vs1::Field1vs1(const Field1vs1& other):
    Field(other),
    balls{other.getBalls()}
  {

  }

  Field1vs1::Field1vs1(RobotSelf* robotSelf, const double& width,
                       const double& length):
    Field(robotSelf, width, length)
  {
    // TODO: Decide whether to fill ball list with dummy values - probably not
  }

  Field1vs1::Field1vs1(const double& width, const double& length):
    Field(new RobotSelf(), width, length)
  {
    // TODO: Decide whether to fill ball list with dummy values - probably not
  }

  Field1vs1::Field1vs1(Goal* friendlyGoal, Goal* enemyGoal,
                       RobotSelf* robotSelf, const double& width,
                       const double& length):
    Field(friendlyGoal, enemyGoal, robotSelf, width, length)
  {
     // TODO: Decide whether to fill ball list with dummy values - probably not
  }

  Field1vs1::Field1vs1(RobotSelf* robotSelf, const std::vector<Ball*>& balls,
                       const double& width, const double& length):
    Field(robotSelf, width, length),
    balls{balls}
  {

  }

  Field1vs1::Field1vs1(Goal* friendlyGoal, Goal* enemyGoal,
                       RobotSelf* robotSelf, const std::vector<Ball*>& balls,
                       const double& width, const double& length):
    Field(friendlyGoal, enemyGoal, robotSelf, width, length),
    balls{balls}
  {

  }

  std::vector<Ball*> Field1vs1::getBalls() const {
    return balls;
  }

};
