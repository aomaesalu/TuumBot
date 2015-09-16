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

#include "robotFriend.h"

RobotFriend::RobotFriend(const RobotFriend& other):
Robot(other.getPosition(), other.getRadius())
{}

RobotFriend::RobotFriend(const Point2D* position, const double& radius):
Robot(position, radius)
{}

RobotFriend::RobotFriend(const double& x, const double& y, const double& radius):
Robot(x, y, radius)
{}
