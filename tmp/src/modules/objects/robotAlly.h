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

 * Friendly robot class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef ROBOT_ALLY_H
#define ROBOT_ALLY_H

#include "constants.h"
#include "robot.h"

class RobotAlly: public Robot {
private:

public:
  RobotAlly(const RobotAlly&);
  RobotAlly(const Point2D*, const double& = ROBOT_RADIUS);
  RobotAlly(const double& = ROBOT_INITIAL_POSITION_X, const double& = ROBOT_INITIAL_POSITION_Y, const double& = ROBOT_RADIUS);
  // TODO: Add communication and strategy methods
};

#endif // ROBOT_ALLY_H
