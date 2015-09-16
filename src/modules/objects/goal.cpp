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

 * Goal class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "goal.h"

Goal::Goal(const Goal& other):
Object(other.getPosition()),
Rectangle(other.getWidth(), other.getLength())
{}

Goal::Goal(const Point2D* position, const double& width, const double& length):
Object(position),
Rectangle(width, length)
{}

Goal::Goal(const double& x, const double& y, const double& width, const double& length):
Object(new Point2D(x, y)),
Rectangle(width, length)
{}

void Goal::swapColor() {
  // TODO
}
