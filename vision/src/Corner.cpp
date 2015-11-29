/**
 *  @file Corner.cpp
 *  Corner seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#include "Corner.hpp"

#include <cmath>


namespace rtx {

  Corner::Corner(const Corner &other):
    distance(other.getDistance()),
    angle(other.getAngle())
  {
    // Nothing to do here
  }

  Corner::Corner(const double &distance, const double &angle):
    distance(distance),
    angle(angle)
  {
    // Nothing to do here
  }

  Corner::Corner(const std::pair<double, double> &point) {
    distance = sqrt(point.second * point.second + point.first + point.first);
    double angle = atan2(point.first, point.second);
  }

  Corner::~Corner() {
    // Nothing to do here
  }

  double Corner::getDistance() const {
    return distance;
  }

  double Corner::getAngle() const {
    return angle;
  }

  std::pair<double, double> Corner::getRelativePoint() const {
    return std::pair<double, double>(distance * sin(angle), distance * cos(angle));
  }

  void Corner::setDistance(const double &distance) {
    this->distance = distance;
  }

  void Corner::setAngle(const double &angle) {
    this->angle = angle;
  }

}
