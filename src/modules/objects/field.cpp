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
currentGoal{other.getCurrentGoal()},
self{new RobotSelf(*(other.getSelf()))}
{
  goals = new Goal*[2];
  if (other.getCurrentGoal() == 0) {
    goals[0] = new Goal(*(other.getFriendlyGoal()));
  } else {
    goals[1] = new Goal(*(other.getEnemyGoal()));
  }
}

Field::Field(const Goal* friendlyGoal, const Goal* enemyGoal, const RobotSelf* self):
currentGoal{0},
self{new RobotSelf(*self)}
{
  goals = new Goal*[2];
  goals[0] = new Goal(*friendlyGoal);
  goals[1] = new Goal(*enemyGoal);
}

void Field::swapGoals() {
  currentGoal = 1 - currentGoal;
}

int Field::getCurrentGoal() const {
  return currentGoal;
}

Goal* Field::getFriendlyGoal() const {
  return goal[currentGoal];
}

Goal* Field::getEnemyGoal() const {
  return goal[1 - currentGoal];
}

RobotSelf* Field::getSelf() const {
  return self;
}
