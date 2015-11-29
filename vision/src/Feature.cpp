/**
 *  @file Feature.cpp
 *  Field feature seen on the camera.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#include "Feature.hpp"


namespace rtx {

  Feature::Feature(const Feature &other):
    distance(other.getDistance()),
    angle(other.getAngle())
  {
    // Nothing to do here
  }

  Feature::Feature(const unsigned int &distance, const double &angle):
    distance(distance),
    angle(angle)
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

  void Feature::setDistance(const unsigned int &distance) {
    this->distance = distance;
  }

  void Feature::setAngle(const double &angle) {
    this->angle = angle;
  }

}
