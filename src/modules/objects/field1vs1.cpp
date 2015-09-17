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

 * 1vs1 field class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "field1vs1.h"

Field1vs1::Field1vs1(const Field1vs1& other):
Field(other),
balls{other.getBalls()}
{}

Field1vs1::Field1vs1(Goal* friendlyGoal, Goal* enemyGoal, RobotSelf* robotSelf, const double& width, const double& length):
Field(friendlyGoal, enemyGoal, robotSelf, width, length)
{} // TODO: Decide whether to fill ball list with dummy values - probably not

Field1vs1::Field1vs1(Goal* friendlyGoal, Goal* enemyGoal, RobotSelf* robotSelf, const std::vector<Ball*>& balls, const double& width, const double& length):
Field(friendlyGoal, enemyGoal, robotSelf, width, length),
balls{balls}
{}

std::vector<Ball*> Field1vs1::getBalls() const {
  return balls;
}
