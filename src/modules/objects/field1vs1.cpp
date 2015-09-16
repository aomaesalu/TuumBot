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

Field1vs1::Field1vs1(const Goal* friendlyGoal, const Goal* enemyGoal, const RobotSelf* self):
Field(friendlyGoal, enemyGoal, self)
{} // TODO: Decide whether to fill ball list with dummy values - probably not

Field1vs1::Field1vs1(const Goal* friendlyGoal, const Goal* enemyGoal, const RobotSelf* self, const std::vector<Ball*> balls):
Field(friendlyGoal, enemyGoal, self),
balls{balls}
{}

std::vector<Ball*> Field1vs1::getBalls() const {
  return balls;
}
