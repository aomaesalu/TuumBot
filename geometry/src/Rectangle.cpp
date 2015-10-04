/**
 * @file Rectangle.cpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#include "Rectangle.hpp"


namespace rtx {

  Rectangle::Rectangle(const Rectangle& other):
    Shape2D(),
    width{other.getWidth()},
    length{other.getLength()}
  {

  }

  Rectangle::Rectangle(const double& width, const double& length):
    Shape2D(),
    width{width},
    length{length}
  {

  }

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

};
