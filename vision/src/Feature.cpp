/**
 * @file Feature.hpp
 * Field feature seen on the camera.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Feature.hpp"


Feature::Feature(const Feature &other) {

}

Feature::Feature(const unsigned int &distance, const double &angle):
  distance{distance},
  angle{angle}
{
  // Nothing to do here
}

Feature::~Feature {
  // Nothing to do here
}

std::pair<unsigned int, double> getDeltaVector() const {
  return std::make_pair<unsigned int, double>(distance, angle);
}

unsigned int getDistance() const {
  return distance;
}

double getAngle() const {
  return angle;
}

void setDistance(const unsigned int &distance) {
  this->distance = distance;
}

void setAngle(const unsigned int &angle) {
  this->angle = angle;
}
