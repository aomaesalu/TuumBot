/**
 * @file Feature.hpp
 * Field feature seen on the camera.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Feature.hpp"


namespace rtx {

  Feature::Feature(const Feature &other):
    distance{other.getDistance()},
    angle{other.getAngle()},
    type{other.getType()}
  {
    // Nothing to do here
  }

  Feature::Feature(const unsigned int &distance, const double &angle,
                   const FeatureType &type):
    distance{distance},
    angle{angle},
    type{type}
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

  FeatureType getType() const {
    return type;
  }

  void setDistance(const unsigned int &distance) {
    this->distance = distance;
  }

  void setAngle(const unsigned int &angle) {
    this->angle = angle;
  }

  void setType(const FeatureType &type) {
    this->type = type;
  }

  bool isCorner() const {
    return type == CORNER;
  }

  bool isGoal() const {
    return type == GOAL;
  }

  bool isBall() const {
    return type == BALL;
  }

  bool isRobot() const {
    return type == ROBOT;
  }

  bool isStatic() const {
    return type == CORNER || type == GOAL;
  }

  bool isMoving() const {
    return type == BALL || type == ROBOT;
  }

}
