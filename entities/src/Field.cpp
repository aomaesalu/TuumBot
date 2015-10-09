/**
 * @file Field.cpp
 * Field class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Field.hpp"


namespace rtx {

  Field::Field(const Field &other):
    Rectangle(other.getWidth(), other.getLength()),
    currentGoal{other.getCurrentGoal()},
    robotSelf{other.getRobotSelf()}
  {
    goals = new Goal*[2];
    if (other.getCurrentGoal() == 0) {
      goals[0] = other.getFriendlyGoal();
    } else {
      goals[1] = other.getEnemyGoal();
    }
  }

  Field::Field(Robot *robotSelf, const double &width, const double &length):
    Rectangle(width, length),
    currentGoal{0},
    robotSelf{robotSelf}
  {
    goals = new Goal*[2];
    goals[0] = new Goal(BLUE_GOAL_POSITION_X, BLUE_GOAL_POSITION_Y); // TODO: Add goal color information
    goals[1] = new Goal(YELLOW_GOAL_POSITION_X, YELLOW_GOAL_POSITION_Y); // TODO: Add goal color information
  }

  Field::Field(Goal *friendlyGoal, Goal *enemyGoal, Robot *robotSelf, const double &width, const double &length):
    Rectangle(width, length),
    currentGoal{0},
    robotSelf{robotSelf}
  {
    goals = new Goal*[2];
    goals[0] = friendlyGoal;
    goals[1] = enemyGoal;
  }

  void Field::swapGoals() {
    currentGoal = 1 - currentGoal;
  }

  int Field::getCurrentGoal() const {
    return currentGoal;
  }

  Goal* Field::getFriendlyGoal() const {
    return goals[currentGoal];
  }

  Goal* Field::getEnemyGoal() const {
    return goals[1 - currentGoal];
  }

  Robot* Field::getRobotSelf() const {
    return robotSelf;
  }

};
