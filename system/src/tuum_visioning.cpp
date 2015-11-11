/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 11. November 2015
 */

#include "tuum_visioning.hpp"
#include "mathematicalConstants.hpp"

#include <fstream>

using namespace rtx;

namespace rtx { namespace Visioning {

  std::string filter;

  FeatureSet features;
  BallSet balls;
  GoalSet goals;
  RobotSet robots;

  void setup() {
    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera();

    readFilterFromFile("../data/colors/1.txt");

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

    Vision::process(frontFrame, filter);
    Vision::process(backFrame, filter);

    if (frontCamera) {
      featureDetection(frontFrame);
      ballDetection(frontFrame);
      goalDetection(frontFrame);
      robotDetection(frontFrame);
    }

    // TODO: Add back camera frame processing
  }

  void readFilterFromFile(const std::string &fileName) {
    std::ifstream inputFile(fileName);
    inputFile >> filter;
    inputFile.close();
  }

  void featureDetection(const Frame &frame) {
    features.clear();
    // TODO
  }

  void ballDetection(const Frame &frame) {
    balls.clear();
    for (unsigned int i = 0; i < Vision::blobs.size(); ++i) {
      if (Vision::blobs[i]->getColor() == BALL) {
        // TODO: Refactor
        Point2D* point = Vision::blobs[i]->getPosition();
        unsigned int distance = 1; // TODO: Calculate based on perspective
        double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
        balls.push_back(new Ball(distance, angle));
      }
    }
  }

  void goalDetection(const Frame &frame) {
    goals.clear();
    for (unsigned int i = 0; i < Vision::blobs.size(); ++i) {
      if (Vision::blobs[i]->getColor() == BLUE_GOAL) {
        // TODO: Refactor
        Point2D* point = Vision::blobs[i]->getPosition();
        unsigned int distance = 1; // TODO: Calculate based on perspective
        double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
        goals.push_back(new Goal(distance, angle));
      } else if (Vision::blobs[i]->getColor() == YELLOW_GOAL) {
        // TODO: Refactor
        Point2D* point = Vision::blobs[i]->getPosition();
        unsigned int distance = 1; // TODO: Calculate based on perspective
        double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
        goals.push_back(new Goal(distance, angle));
      }
    }
  }

  void robotDetection(const Frame &frame) {
    robots.clear();
    // TODO
  }

}}
