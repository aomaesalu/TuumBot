/**
 * @file Line.cpp
 * Line class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Line.hpp"


namespace rtx {

  Line::Line(const Line &other):
    pointA{new Point2D(other.getPointA()->getX(), other.getPointB()->getY())},
    pointB{new Point2D(other.getPointB()->getX(), other.getPointB()->getY())}
  {

  }

  Line::Line(const Point2D *pointA, const Point2D *pointB):
    pointA{new Point2D(pointA->getX(), pointA->getY())},
    pointB{new Point2D(pointB->getX(), pointB->getY())}
  {

  }

  Line::Line(const double &pointAx, const double &pointAy, const double
             &pointBx, const double &pointBy):
    pointA{new Point2D(pointAx, pointAy)},
    pointB{new Point2D(pointBx, pointBy)}
  {

  }

  void Line::setPointA(const Point2D *pointA) {
    this->pointA = new Point2D(pointA->getX(), pointA->getY());
  }

  void Line::setPointA(const double &x, const double &y) {
    pointA = new Point2D(x, y);
  }

  void Line::setPointB(const Point2D *pointB) {
    this->pointB = new Point2D(pointB->getX(), pointB->getY());
  }

  void Line::setPointB(const double &x, const double &y) {
    pointB = new Point2D(x, y);
  }

  double Line::getLength() const {
    return pointA->distanceFrom(pointB);
  }

  Point2D* Line::getPointA() const {
    return pointA;
  }

  Point2D* Line::getPointB() const {
    return pointB;
  }

};
