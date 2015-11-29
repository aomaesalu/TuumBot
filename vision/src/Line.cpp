/**
 *  @file Line.cpp
 *  Line seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#include "Line.hpp"


namespace rtx {

  Line::Line(const Line &other):
    distance(other.getDistance()),
    angle(other.getAngle())
  {
    // Nothing to do here
  }

  Line::Line(const double &distance, const double &angle):
    distance(distance),
    angle(angle)
  {
    // Nothing to do here
  }

  Line::Line(const std::vector<std::pair<double, double>> &points) {
    // TODO: Regression?
  }

  Line::~Line() {
    // Nothing to do here
  }

  double Line::getDistance() const {
    return distance;
  }

  double Line::getAngle() const {
    return angle;
  }

  void Line::setDistance(const double &distance) {
    this->distance = distance;
  }

  void Line::setAngle(const double &angle) {
    this->angle = angle;
  }

}
