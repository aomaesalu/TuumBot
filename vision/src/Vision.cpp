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

  std::vector<Blob*> Vision::getBlobs() const {
    return blobs;
  }

  std::vector<Feature*> Vision::getLines() const {
    return lines;
  }

  std::vector<Feature*> Vision::getCorners() const {
    return corners;
  }

  void Vision::process() {
    blobDetection();
    lineDetection();
    cornerDetection();
  }

  void Vision::lineDetection() {
    // TODO
  }

  void Vision::cornerDetection() {
    // TODO
  }

  void Vision::blobDetection() {
    // TODO
  }
};
