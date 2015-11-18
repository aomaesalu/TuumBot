/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17 November 2015
 */

#include "tuum_visioning.hpp"
#include "mathematicalConstants.hpp"

#include <fstream>
#include <iostream> // TODO: Remove

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
    if (filter.size() == 0) {
      std::cout << "Process: Filter is empty" << std::endl;
      return;
    }

    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera(); // TODO: Use

    Frame frontFrame, backFrame;
    if (frontCamera)
      frontFrame = frontCamera->getFrame();
    if (backCamera)
      backFrame = backCamera->getFrame();

    if (frontCamera)
      Vision::process(frontFrame, filter);
    if (backCamera)
      Vision::process(backFrame, filter);

    if (frontCamera) {
      featureDetection(frontFrame);
      ballDetection(frontFrame);
      goalDetection(frontFrame);
      robotDetection(frontFrame);
    }

    // TODO: Add back camera frame processing
  }

  void processCheckerboard() {
    if (filter.size() == 0) {
      std::cout << "Process: Filter is empty" << std::endl;
      return;
    }

    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera(); // TODO: Use

    Frame frontFrame, backFrame;
    if (frontCamera)
      frontFrame = frontCamera->getFrame();
    if (backCamera)
      backFrame = backCamera->getFrame();

    if (frontCamera)
      Vision::processCheckerboard(frontFrame, filter);
    if (backCamera)
      Vision::processCheckerboard(backFrame, filter);

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

    Vision::BlobSet blobs = Vision::blobs;
    while (Vision::editingBlobs) {
      blobs = Vision::blobs;
    }

    // DEBUG: std::cout << "Blobs in visioning: " << Vision::blobs.size() << std::endl;

    balls.clear();

    for (unsigned int i = 0; i < blobs.size(); ++i) {
      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();
      if (color == BALL/* && density > 0.4*/ && density <= 1.0 && boxArea <= CAMERA_WIDTH * CAMERA_HEIGHT/* && boxArea > 8 * 8*/) {
        //std::cout << "Dim: " << blobs[i]->getDensity() << " " << blobs[i]->getBoxArea() << std::endl;
        // TODO: Refactor
        Point2D* point = blobs[i]->getPosition();
        unsigned int distance = CAMERA_HEIGHT - point->getY(); // TODO: Calculate based on perspective
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
        unsigned int distance = CAMERA_HEIGHT - point->getY(); // TODO: Calculate based on perspective
        double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
        goals.push_back(new Goal(distance, angle));
      } else if (Vision::blobs[i]->getColor() == YELLOW_GOAL) {
        // TODO: Refactor
        Point2D* point = Vision::blobs[i]->getPosition();
        unsigned int distance = CAMERA_HEIGHT - point->getY(); // TODO: Calculate based on perspective
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
