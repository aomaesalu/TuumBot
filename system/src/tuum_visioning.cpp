/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 11. November 2015
 */

#include "tuum_visioning.hpp"

using namespace rtx;

namespace rtx { namespace Visioning {

  FeatureSet features;
  BallSet balls;
  GoalSet goals;
  RobotSet robots;

  void setup() {
    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera();

    Vision::setup();

    printf("\033[1;32m");
    printf("[Visioning::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera(); // TODO: Use

    Frame frontFrame, backFrame;
    if (frontCamera)
      frontFrame = frontCamera->getFrame();
    if (backCamera)
      backFrame = backCamera->getFrame();

    Vision::process(frontCamera);
    Vision::process(backCamera);

    if (frontCamera) {
      featureDetection(frontCamera);
      ballDetection(frontCamera);
      goalDetection(frontCamera);
      robotDetection(frontCamera);
    }
  }

  void featureDetection(Camera *camera) {
    // TODO
  }

  void ballDetection(Camera *camera) {
    // TODO
  }

  void goalDetection(Camera *camera) {
    // TODO
  }

  void robotDetection(Camera *camera) {
    // TODO
  }

}}
