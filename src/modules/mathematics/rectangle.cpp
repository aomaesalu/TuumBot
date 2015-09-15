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
width{other.getWidth()},
length{other.getLength()}
{}

Rectangle::Rectangle(const double& width, const double& length):
width{width},
length{length}
{}

void Rectangle::setWidth(const double& width) {
  this->width = width;
}

void Rectangle::setLength(const double& length) {
  this->length = length;
}

double Rectangle::getArea() const {
  return width * length;
}

double Rectangle::getPerimeter() const {
  return 2 * (width + length);
}

double Rectangle::getWidth() const {
  return width;
}

double Rectangle::getLength() const {
  return length;
}
