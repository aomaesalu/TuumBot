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

#include "robotAlly.h"

RobotAlly::RobotAlly(const RobotAlly& other):
Robot(other.getPosition(), other.getRadius())
{}

RobotAlly::RobotAlly(const Point2D* position, const double& radius):
Robot(position, radius)
{}

RobotAlly::RobotAlly(const double& x, const double& y, const double& radius):
Robot(x, y, radius)
{}
