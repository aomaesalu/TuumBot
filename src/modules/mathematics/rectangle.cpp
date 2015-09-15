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

 * Rectangle class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "rectangle.h"

Rectangle::Rectangle(const Rectangle& other):
x{other.getX()},
y{other.getY()}
{}

Rectangle::Rectangle(const double& x, const double& y):
x{x},
y{y}
{}

void Rectangle::setX(const double& x) {
  this->x = x;
}

void Rectangle::setY(const double& y) {
  this->y = y;
}

double Rectangle::getArea() const {
  return x * y;
}

double Rectangle::getPerimeter() const {
  return 2 * (a + b);
}

double Rectangle::getX() const {
  return x;
}

double Rectangle::getY() const {
  return y;
}
