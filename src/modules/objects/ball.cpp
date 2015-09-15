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

 * Ball class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "ball.h"

Ball::Ball(const Ball& other):
Object(other.getPosition()),
Circle(other.getRadius())
{}

Ball::Ball(const Point2D* position, const double& radius):
Object(position)
Circle(radius)
{}

Ball::Ball(const double& x, const double& y, const double& radius):
Object(new Point2D(x, y)),
Circle(radius)
{}
