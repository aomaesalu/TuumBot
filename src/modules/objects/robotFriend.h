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

#ifndef ROBOT_FRIEND_H
#define ROBOT_FRIEND_H

#include "robot.h"

class RobotFriend: public Robot {
private:

public:
  RobotFriend(const RobotFriend&);
  RobotFriend(const Point2D*, const double&); // TODO: Add default robot radius
  RobotFriend(const double&, const double&, const double&); // TODO: Add default robot radius
  // TODO: Add communication and strategy methods
};

#endif // ROBOT_FRIEND_H
