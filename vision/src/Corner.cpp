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
    Feature(other)
  {
    // Nothing to do here
  }

  Corner::Corner(const double &distance, const double &angle):
    Feature(distance, angle)
  {
    // Nothing to do here
  }

  Corner::Corner(const std::pair<double, double> &point):
    Feature(0, 0)
  {
    distance = sqrt(point.second * point.second + point.first + point.first);
    angle = atan2(point.first, point.second);
  }

  Corner::~Corner() {
    // Nothing to do here
  }

  std::pair<double, double> Corner::getRelativePoint() const {
    return std::pair<double, double>(distance * sin(angle), distance * cos(angle));
  }

}
