/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 1. November 2015
 */

#include "rtxhal.hpp"
#include "tuum_visioning.hpp"

using namespace rtx;

namespace rtx { namespace Visioning {

  FeatureSet features;
  BallSet balls;
  GoalSet goals;
  RobotSet robots;

  void setup() {
    CameraDevice *frontCamera = hal::hw.getFrontCamera();
    CameraDevice *backCamera = hal::hw.getBackCamera();

    Vision::setup();

    printf("\033[1;32m");
    printf("[Visioning::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    CameraDevice *frontCamera = hal::hw.getFrontCamera();
    CameraDevice *backCamera = hal::hw.getBackCamera(); // TODO: Use

    Vision::process(frontCamera);
    Vision::process(backCamera);

    if (frontCamera) {
      featureDetection(frontCamera);
      ballDetection(frontCamera);
      goalDetection(frontCamera);
      robotDetection(frontCamera);
    }
  }

  void featureDetection(CameraDevice *camera) {
    // TODO
  }

  void ballDetection(CameraDevice *camera) {
    // TODO
  }

  void goalDetection(CameraDevice *camera) {
    // TODO
  }

  void robotDetection(CameraDevice *camera) {
    // TODO
  }

}}
