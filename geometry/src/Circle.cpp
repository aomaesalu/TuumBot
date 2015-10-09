/**
 * @file Circle.cpp
 * Circle class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

//#include "rtxc.h"
#include "mathematicalConstants.h"

#include "Circle.hpp"


namespace rtx {

  Circle::Circle(const Circle &other):
    Shape2D(),
    radius{other.getRadius()}
  {

  }

  Circle::Circle(const double &radius):
    Shape2D(),
    radius{radius}
  {

  }

  void Circle::setRadius(const double &radius) {
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

};
