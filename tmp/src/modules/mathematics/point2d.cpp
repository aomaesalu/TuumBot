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

 * 2-dimensional point class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "point2d.h"
#include <cmath>

Point2D::Point2D(const Point2D& other):
x{other.getX()},
y{other.getY()}
{}

Point2D::Point2D(const double& x, const double& y):
x{x},
y{y}
{}

void Point2D::setX(const double& x) {
  this->x = x;
}

void Point2D::setY(const double& y) {
  this->y = y;
}

double Point2D::distanceFrom(const Point2D* other) {
  return sqrt(x * other->getX() + y * other->getY()); // TODO: Optimise
}

double Point2D::getX() const {
  return x;
}

double Point2D::getY() const {
  return y;
}
