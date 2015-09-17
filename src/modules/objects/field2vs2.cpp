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
ally{other.getAlly()},
enemies{other.getEnemies()}
{}

Field2vs2::Field2vs2(Goal* friendlyGoal, Goal* enemyGoal, RobotSelf* robotSelf, RobotAlly* ally, RobotEnemy** enemies, const double& width, const double& length):
Field(friendlyGoal, enemyGoal, robotSelf, width, length),
ally{ally},
enemies{enemies}
{} // TODO: Decide whether to fill ball pointer with dummy value - probably not

Field2vs2::Field2vs2(Goal* friendlyGoal, Goal* enemyGoal, RobotSelf* robotSelf, RobotAlly* ally, RobotEnemy** enemies, Ball* ball, const double& width, const double& length):
Field(friendlyGoal, enemyGoal, robotSelf, width, length),
ally{ally},
enemies{enemies},
ball{ball}
{}

RobotAlly* Field2vs2::getAlly() const {
  return ally;
}

RobotEnemy** Field2vs2::getEnemies() const {
  return enemies;
}

Ball* Field2vs2::getBall() const {
  return ball;
}
