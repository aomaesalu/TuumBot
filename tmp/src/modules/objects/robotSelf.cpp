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

 * Robot self class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "robotSelf.h"

RobotSelf::RobotSelf(const RobotSelf& other):
Robot(other.getPosition(), other.getRadius())
{}

RobotSelf::RobotSelf(const Point2D* position, const double& radius):
Robot(position, radius)
{}

RobotSelf::RobotSelf(const double& x, const double& y, const double& radius):
Robot(x, y, radius)
{}
