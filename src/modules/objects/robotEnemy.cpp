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

#include "robotEnemy.h"

RobotEnemy::RobotEnemy(const RobotEnemy& other):
Robot(other.getPosition(), other.getRadius())
{}

RobotEnemy::RobotEnemy(const Point2D* position, const double& radius):
Robot(position, radius)
{}

RobotEnemy::RobotEnemy(const double& x, const double& y, const double& radius):
Robot(x, y, radius)
{}
