/**
 * @file Point2D.cpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#include <cmath>
#include "Point2D.hpp"

namespace rtx {

  /** Placeholder
   *
   */
  Point2D::Point2D(const Point2D& other):
    x{other.getX()}, y{other.getY()}
  {

  }

  Point2D::Point2D(const double& x, const double& y):
    x{x}, y{y}
  {

  }

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


};
