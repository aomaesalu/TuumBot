/**
 * Tuum
 * Robotex 2015
 * University of Tartu
 * Team:
 *   Kristjan Kanarbik
 *   Meelik Kiik
 *   Ants-Oskar Mäesalu
 *   Mart Müllerbeck
 *   Kaur Viiklaid
 *   Indrek Virro
 *   Mihkel Väljaots

 * Field class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "field.h"

Field::Field(const Field& other):
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

Field::Field(Goal* friendlyGoal, Goal* enemyGoal, RobotSelf* robotSelf, const double& width, const double& length):
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

RobotSelf* Field::getRobotSelf() const {
  return robotSelf;
}
