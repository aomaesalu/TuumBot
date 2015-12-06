/**
 *  @file Feature.cpp
 *  Field feature seen on the camera.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 6 December 2015
 */

#include "Feature.hpp"

#include <cmath>


namespace rtx { namespace Vision {

  Feature::Feature(const Feature &other):
    distance(other.getDistance()),
    angle(other.getAngle()),
    cameraID(other.getCameraID())
  {
    // Nothing to do here
  }

  Feature::Feature(const unsigned int &distance, const double &angle):
    distance(distance),
    angle(angle),
    cameraID(cameraID)
  {
    // Nothing to do here
  }

  Feature::~Feature() {
    // Nothing to do here
  }

  unsigned int Feature::getDistance() const {
    return distance;
  }

  double Feature::getAngle() const {
    return angle;
  }

  unsigned int Feature::getCameraID() const {
    return cameraID;
  }

  std::pair<double, double> Feature::getRelativePoint() const {
    return std::pair<double, double>(distance * sin(angle), distance * cos(angle));
  }

  void Feature::setDistance(const unsigned int &distance) {
    this->distance = distance;
  }

  void Feature::setAngle(const double &angle) {
    this->angle = angle;
  }

}}
