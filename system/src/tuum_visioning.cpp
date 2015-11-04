/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 1. November 2015
 */

#include "rtxhal.hpp"
#include "tuum_visioning.hpp"

using namespace rtx::hal;

namespace rtx { namespace Visioning {

  FeatureSet features;
  BallSet balls;
  RobotSet robots;
  GoalSet goals;

  void setup() {
    CameraDevice* cam = hal::hw.getFrontCamera();

    printf("\033[1;32m");
    printf("[Visioning::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    CameraDevice* cam = hal::hw.getFrontCamera();

    if(cam != nullptr) {
      featureDetection(cam);
      ballsDetection(cam);
      robotsDetection(cam);
      goalDetection(cam);
    }
  }

  void featureDetection(CameraDevice* cam) {

  }

  void ballsDetection(CameraDevice* cam) {

  }

  void robotsDetection(CameraDevice* cam) {

  }

  void goalDetection(CameraDevice* cam) {

  }

}}
