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

#include "robot.h"

class RobotAlly: public Robot {
private:

public:
  RobotAlly(const RobotAlly&);
  RobotAlly(const Point2D*, const double&); // TODO: Add default robot radius
  RobotAlly(const double&, const double&, const double&); // TODO: Add default robot radius
  // TODO: Add communication and strategy methods
};

#endif // ROBOT_ALLY_H
