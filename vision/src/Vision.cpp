/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "vision.hpp"


namespace rtx {

  Vision::Vision() {
    // TODO
  }

  Vision::~Vision() {
    // TODO
  }

  std::vector<Feature*> Vision::getBalls() const {
    return balls;
  }

  std::vector<Feature*> Vision::getGoals() const {
    return goals;
  }

  std::vector<Feature*> Vision::getCorners() const {
    return corners;
  }

  std::vector<Feature*> Vision::getRobots() const {
    return robots;
  }

  std::vector<Feature*> Vision::getStaticFeatures() const {
    std::vector<Feature*> features;
    features.insert(features.end(), corners.begin(), corners.end());
    features.insert(features.end(), goals.begin(), goals.end());
    return features;
  }

  std::vector<Feature*> Vision::getMovingFeatures() const {
    std::vector<Feature*> features;
    features.insert(features.end(), balls.begin(), balls.end());
    features.insert(features.end, robots.begin(), robots.end());
    return features;
  }

  void Vision::process() {
    lineDetection();
    cornerDetection();
    blobDetection();
    ballDetection();
    goalDetection();
    robotDetection();
  }

  void Vision::lineDetection() {
    // TODO
  }

  void Vision::blobDetection() {
    // TODO
  }

  void Vision::ballDetection() {
    // TODO
  }

  void Vision::goalDetection() {
    // TODO
  }

  void Vision::cornerDetection() {
    // TODO
  }

  void Vision::robotDetection() {
    // TODO
  }

};
