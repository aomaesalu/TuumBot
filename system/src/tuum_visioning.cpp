/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 26 November 2015
 */

#include <algorithm>
#include <fstream>
#include <iostream> // TODO: Remove
#include <sstream>
#include <cmath>

#include "Perspective.hpp"

#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"
#include "mathematicalConstants.hpp"

using namespace rtx;

namespace rtx { namespace Visioning {

  std::string filter;

  Timer debugTimer;

  FeatureSet features;

  EDS<Ball> ballDetect;
  EDS<Robot> robotDetect;

  Goal *yellowGoal;
  Goal *yellowGoalBuffer;
  Goal *blueGoal;
  Goal *blueGoalBuffer;

  bool editingGoals = false;

  void setup() {
    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera();

    readFilterFromFile("../data/colors/1.txt");

    Vision::setup();

    debugTimer.setPeriod(1000);
    debugTimer.start();

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
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    filter = buffer.str();
    inputFile.close();
  }

  void translateGoalsBuffer() {
    editingGoals = true;

    // TODO: Refactor buffer management

    /*if (blueGoal) {
      if (blueGoal != blueGoalBuffer) {
        delete(blueGoal);
        blueGoal = blueGoalBuffer;
      }
    } else {
      blueGoal = blueGoalBuffer;
    }

    if (yellowGoal) {
      if (yellowGoal != yellowGoalBuffer) {
        delete(yellowGoal);
        yellowGoal = yellowGoalBuffer;
      }
    } else {
      yellowGoal = yellowGoalBuffer;
    }*/

    if (blueGoalBuffer)
      blueGoal = new Goal(*blueGoalBuffer);
    else
      blueGoal = nullptr;
    if (yellowGoalBuffer)
      yellowGoal = new Goal(*yellowGoalBuffer);
    else
      yellowGoal = nullptr;

    // TODO: Remove casting to null pointers when localisation is working
    blueGoalBuffer = nullptr;
    yellowGoalBuffer = nullptr;

    editingGoals = false;
  }

  void featureDetection(const Frame &frame) {
    features.clear();
    // TODO
  }

  void ballDetection(const Frame &frame) {
    Vision::BlobSet blobs = Vision::getBlobs();
    BallSet n_balls;

    for(unsigned int i = 0; i < blobs.size(); ++i) {
      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();
      double ratio = blobs[i]->getBoxRatio();

      // STEP 1: Filter out invalid blobs
      if(color != BALL) continue;
      if(boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if(density > 1.0) continue;
      if(boxArea < 4 * 4) continue;
      if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      // STEP 2: Calculate relative position
      Point2D* point = blobs[i]->getPosition();

      std::pair<double, double> position = Vision::Perspective::virtualToReal(point->getX(), blobs[i]->getMaxY());
      double distance = sqrt(position.second * position.second + position.first * position.first);
      double angle = atan2(position.first, position.second);
      //unsigned int distance = CAMERA_HEIGHT - point->getY();
      //double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;

      // STEP 3: Create ball instance with absolute position
      n_balls.push_back(new Ball(Localization::toAbsoluteTransform(distance, angle)));
    }

    // STEP 4: Unite detected balls with balls from last frame or create new balls
    Ball* n_ball_ptr;
    BallSet* ball_set_ptr;
    for(int ix = 0; ix < n_balls.size(); ix++) {
      ballDetect.processProbableEntity(n_balls[ix]);
    }

    // STEP 5: Entity vectors updates - remove decayed balls and make healthy detectable
    ballDetect.update();

    /*
    if(debugTimer.isTime()) {
      std::cout << "[Visioning]Balls: " << ballDetect.getEntities()->size()
          << ". Unconfirmed balls: " << ballDetect.getTmpEntities()->size()
          << std::endl;

      for(auto& b : *(ballDetect.getEntities())) {
        Transform* t = b->getTransform();
        std::cout << "<Ball hp=" << b->getHealth() << ", x=" << t->getX() << ", y=" << t->getY() << ">" << std::endl;
      }

      debugTimer.start();
    }
    */
  }

  void goalDetection(const Frame &frame) {
    // TODO: Remove casting to null pointers when localisation is working
    blueGoalBuffer = nullptr;
    yellowGoalBuffer = nullptr;

    Vision::BlobSet blobs = Vision::getBlobs();

    unsigned int largestYellowArea = 0, largestBlueArea = 0;

    for (unsigned int i = 0; i < blobs.size(); ++i) {
      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();
      //double ratio = blobs[i]->getBoxRatio();

      // Filter out invalid blobs
      if (color != BLUE_GOAL && color != YELLOW_GOAL) continue;
      if (boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if (density > 1.0) continue;
      if(boxArea < 20 * 20) continue; // TODO: Calibrate with field tests
      if(blobs[i]->getBoxRatio() < 1.2) continue;
      //std::cout << "Ratio: " << blobs[i]->getBoxRatio() << std::endl;
      //if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      Point2D* point = blobs[i]->getPosition();
      // Relative position
      std::pair<double, double> position = Vision::Perspective::virtualToReal(point->getX(), blobs[i]->getMaxY());
      double distance = sqrt(position.second * position.second + position.first * position.first);
      double angle = atan2(position.first, position.second);
      // std::cout << "Goal: " << distance << " " << angle << std::endl;
      //unsigned int distance = CAMERA_HEIGHT - point->getY();
      //double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
      // TODO: Remove duplicate code
      if (color == BLUE_GOAL) {
        if (boxArea > largestBlueArea) {
          largestBlueArea = boxArea;
          //if (blueGoalBuffer == nullptr) {
          blueGoalBuffer = new Goal(Localization::toAbsoluteTransform(distance, angle), color);
          /*} else {
            blueGoalBuffer->setDistance(distance); // TODO: Compare with previous values as in ball detection
            blueGoalBuffer->setAngle(angle); // TODO: Compare with previous values as in ball detection
          }*/
        }
      } else {
        if (boxArea > largestYellowArea) {
          largestYellowArea = boxArea;
          //if (yellowGoalBuffer == nullptr) {
          yellowGoalBuffer = new Goal(Localization::toAbsoluteTransform(distance, angle), color);
          /*} else {
            yellowGoalBuffer->setDistance(distance); // TODO: Compare with previous values as in ball detection
            yellowGoalBuffer->setAngle(angle); // TODO: Compare with previous values as in ball detection
          }*/
        }
      }

    }

    translateGoalsBuffer();
  }

  void robotDetection(const Frame &frame) {
    Vision::BlobSet blobs = Vision::getBlobs();
    RobotSet n_robots;

    for(unsigned int i = 0; i < blobs.size(); ++i) {
      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();
      //double ratio = blobs[i]->getBoxRatio();

      if (color != ROBOT_YELLOW_BLUE && color != ROBOT_BLUE_YELLOW) continue;
      if (boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if (density > 1.0) continue;
      //if(boxArea < 8 * 3) continue; // TODO: Calibrate with field tests
      //if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      Point2D* point = blobs[i]->getPosition();

      std::pair<double, double> position = Vision::Perspective::virtualToReal(point->getX(), blobs[i]->getMaxY());
      double distance = sqrt(position.second * position.second + position.first * position.first);
      double angle = atan2(position.first, position.second);

      //unsigned int distance = CAMERA_HEIGHT - point->getY();
      //double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;

      n_robots.push_back(new Robot(Localization::toAbsoluteTransform(distance, angle)));
    }

    for (int ix = 0; ix < n_robots.size(); ix++) {
      std::cout << n_robots[ix]->toString() << std::endl;
      robotDetect.processProbableEntity(n_robots[ix]);
    }

    robotDetect.update();
  }

}}
