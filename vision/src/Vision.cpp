/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "vision.hpp"


void emptyVector(std::vector<Feature*> &vector) {
  for (std::vector<Feature*>::iterator i = vector.begin(); i != vector.end();
       ++i) {
    delete *i;
  }
  vector.clear();
}

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
    return staticFeatures;
  }

  std::vector<Feature*> Vision::getMovingFeatures() const {
    return movingFeatures;
  }

  std::vector<Feature*> Vision::getAllFeatures() const {
    return allFeatures;
  }

  void Vision::process() {
    lineDetection();
    cornerDetection();
    blobDetection();
    ballDetection();
    goalDetection();
    robotDetection();
    updateFeatures();
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

  void updateStaticFeatures() {
    emptyVector(staticFeatures);
    staticFeatures.insert(features.end(), corners.begin(), corners.end());
    staticFeatures.insert(features.end, goals.begin(), goals.end());
  }

  void updateMovingFeatures() {
    emptyVector(movingFeatures);
    staticFeatures.insert(features.end(), balls.begin(), balls.end());
    staticFeatures.insert(features.end, robots.begin(), robots.end());
  }

  void updateAllFeatures() {
    emptyVector(allFeatures);
    staticFeatures.insert(features.end(), staticFeatures.begin(),
                          staticFeatures.end());
    staticFeatures.insert(features.end, movingFeatures.begin(),
                          movingFeatures.end());
  }

  void updateFeatures() {
    updateStaticFeatures();
    updateMovingFeatures();
    updateAllFeatures();
  }

};
