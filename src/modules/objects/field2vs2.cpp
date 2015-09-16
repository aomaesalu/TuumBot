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

 * 2vs2 field class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "field2vs2.h"

Field2vs2::Field2vs2(const Field2vs2& other):
Field(other),
ball{other.getBall()},
friend{other.getFriend()},
enemies{other.getEnemies()}
{}

Field2vs2::Field2vs2(const Goal* friendlyGoal, const Goal* enemyGoal, const RobotSelf* self, const RobotFriend* friend, const RobotEnemy** enemies):
Field(friendlyGoal, enemyGoal, self),
friend{friend},
enemies{enemies}
{} // TODO: Decide whether to fill ball pointer with dummy value - probably not

Field2vs2::Field2vs2(const Goal* friendlyGoal, const Goal* enemyGoal, const RobotSelf* self, const RobotFriend* friend, const RobotEnemy** enemies, const Ball* ball):
Field(friendlyGoal, enemyGoal, self),
friend{friend},
enemies{enemies},
ball{ball}
{}

RobotFriend* Field2vs2::getFriend() const {
  return friend;
}

RobotEnemies** Field2vs2::getEnemies() const {
  return enemies;
}

Ball* Field2vs2::getBall() const {
  return ball;
}
