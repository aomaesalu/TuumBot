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

  std::vector<Point2D*> Vision::getBalls() const {
    return balls;
  }

  std::vector<Point2D*> Vision::getGoals() const {
    return goals;
  }

  std::vector<Point2D*> Vision::getCorners() const {
    return corners;
  }

  std::vector<Point2D*> Vision::getRobots() const {
    return robots;
  }

  void Vision::analyse() {
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
