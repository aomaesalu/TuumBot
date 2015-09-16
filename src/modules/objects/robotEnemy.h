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

 * Enemy robot class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef ROBOT_ENEMY_H
#define ROBOT_ENEMY_H

#include "robot.h"

class RobotEnemy: public Robot {
private:

public:
  RobotEnemy(const RobotEnemy&);
  RobotEnemy(const Point2D*, const double&); // TODO: Add default maximum robot radius; can be changed midgame
  RobotEnemy(const double&, const double&, const double&); // TODO: Add default maximum robot radius; can be changed midgame
  // TODO: Add size, movement and strategy analysing methods
}

#endif // ROBOT_ENEMY_H
