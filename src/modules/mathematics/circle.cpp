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

 * Circle class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "circle.h"
#include "constants.h"

Circle::Circle(const Circle& other):
radius{other.getRadius()}
{}

Circle::Circle(const double& radius):
radius{radius}
{}

void Circle::setRadius(const double& radius) {
  this->radius = radius;
}

double Circle::getArea() const {
  return radius * radius;
}

double Circle::getPerimeter() const {
  return 2 * PI * radius;
}

double Circle::getRadius() const {
  return radius;
}
