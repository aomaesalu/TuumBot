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


namespace rtx { namespace Vision {

  Corner::Corner(const Corner &other):
    Feature(other)
  {
    // Nothing to do here
  }

  Corner::Corner(const double &distance, const double &angle, const unsigned int &cameraID):
    Feature(distance, angle, cameraID)
  {
    // Nothing to do here
  }

  Corner::Corner(const std::pair<double, double> &point, const unsigned int &cameraID):
    Feature(0, 0, cameraID)
  {
    distance = sqrt(point.second * point.second + point.first + point.first);
    angle = -atan2(point.first, point.second);
  }

  Corner::~Corner() {
    // Nothing to do here
  }

}}
