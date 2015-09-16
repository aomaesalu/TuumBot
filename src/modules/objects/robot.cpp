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

 * Robot class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "robot.h"

Robot::Robot(const Robot& other):
Object(other.getPosition()),
Circle(other.getRadius())
{}

Robot::Robot(const Point2D* position, const double& radius):
Object(position)
Circle(radius)
{}

Robot::Robot(const double& x, const double& y, const double& radius):
Object(new Point2D(x, y)),
Circle(radius)
{}
