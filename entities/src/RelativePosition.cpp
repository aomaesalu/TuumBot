/**
 * @file RelativePosition.hpp
 * Relative position class.
 * TODO: Implement elsewhere.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "RelativePosition.hpp"


namespace rtx {

  RelativePosition::RelativePosition(const RelativePosition &other):
    distance{other.getDistance()},
    angle{other.getAngle()}
  {
    // Nothing to do here
  }

  RelativePosition::RelativePosition(const unsigned int &distance, const double &angle):
    distance{distance},
    angle{angle}
  {
    // Nothing to do here
  }

  RelativePosition::~RelativePosition() {
    // Nothing to do here
  }

  std::pair<unsigned int, double> RelativePosition::getDeltaVector() const {
    return std::pair<unsigned int, double>(distance, angle);
  }

  unsigned int RelativePosition::getDistance() const {
    return distance;
  }

  double RelativePosition::getAngle() const {
    return angle;
  }

  void RelativePosition::setDistance(const unsigned int &distance) {
    this->distance = distance;
  }

  void RelativePosition::setAngle(const double &angle) {
    this->angle = angle;
  }

};
