/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Vision.hpp"


namespace rtx {

  void emptyVector(std::vector<Feature*> &vector) {
    for (std::vector<Feature*>::iterator i = vector.begin(); i != vector.end();
         ++i) {
      delete *i;
    }
    vector.clear();
  }

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

  std::vector<Feature*> Vision::getMovableFeatures() const {
    return movableFeatures;
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

  void Vision::updateStaticFeatures() {
    emptyVector(staticFeatures);
    staticFeatures.insert(staticFeatures.end(), corners.begin(), corners.end());
    staticFeatures.insert(staticFeatures.end(), goals.begin(), goals.end());
  }

  void Vision::updateMovableFeatures() {
    emptyVector(movableFeatures);
    movableFeatures.insert(movableFeatures.end(), balls.begin(), balls.end());
    movableFeatures.insert(movableFeatures.end(), robots.begin(), robots.end());
  }

  void Vision::updateAllFeatures() {
    emptyVector(allFeatures);
    allFeatures.insert(allFeatures.end(), staticFeatures.begin(),
                          staticFeatures.end());
    allFeatures.insert(allFeatures.end(), movableFeatures.begin(),
                          movableFeatures.end());
  }

  void Vision::updateFeatures() {
    updateStaticFeatures();
    updateMovableFeatures();
    updateAllFeatures();
  }

};
