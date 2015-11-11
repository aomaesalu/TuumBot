/**
 * @file Line.cpp
 * Line class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Line2D.hpp"


namespace rtx {

  Line2D::Line2D(const Line2D &other):
    pointA{new Point2D(other.getPointA()->getX(), other.getPointB()->getY())},
    pointB{new Point2D(other.getPointB()->getX(), other.getPointB()->getY())}
  {

  }

  Line2D::Line2D(const Point2D *pointA, const Point2D *pointB):
    pointA{new Point2D(pointA->getX(), pointA->getY())},
    pointB{new Point2D(pointB->getX(), pointB->getY())}
  {

  }

  Line2D::Line2D(const double &pointAx, const double &pointAy,
             const double &pointBx, const double &pointBy):
    pointA{new Point2D(pointAx, pointAy)},
    pointB{new Point2D(pointBx, pointBy)}
  {

  }

  void Line2D::setPointA(const Point2D *pointA) {
    this->pointA = new Point2D(pointA->getX(), pointA->getY());
  }

  void Line2D::setPointA(const double &x, const double &y) {
    pointA = new Point2D(x, y);
  }

  void Line2D::setPointB(const Point2D *pointB) {
    this->pointB = new Point2D(pointB->getX(), pointB->getY());
  }

  void Line2D::setPointB(const double &x, const double &y) {
    pointB = new Point2D(x, y);
  }

  double Line2D::getLength() const {
    return pointA->distanceFrom(pointB);
  }

  Point2D* Line2D::getPointA() const {
    return pointA;
  }

  Point2D* Line2D::getPointB() const {
    return pointB;
  }

};
