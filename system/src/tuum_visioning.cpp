/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 4 December 2015
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

  std::vector<std::string> filters;

  Timer debugTimer;

  FeatureSet features;

  EDS<Ball> ballDetect;
  BallSet balls; // Healthy ball entities vs new/decaying ball entities?
  BallSet ballsBuffer; // Unused

  Goal *yellowGoal;
  Goal *yellowGoalBuffer;
  Goal *blueGoal;
  Goal *blueGoalBuffer;

  EDS<Robot> robotDetect;
  RobotSet robots;
  RobotSet robotsBuffer; // Unused

  bool editingBalls = false; // Unused
  bool editingGoals = false;
  bool editingRobots = false; // Unused

  void setup() {

    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera();

    if (frontCamera != nullptr)
      readFilterFromFile("../data/colors/1.txt");
    if (backCamera != nullptr)
      readFilterFromFile("../data/colors/2.txt");

    if (frontCamera != nullptr)
      Vision::setup(0);
    if (backCamera != nullptr)
      Vision::setup(1);

    debugTimer.setPeriod(1000);
    debugTimer.start();

    printf("\033[1;32m");
    printf("[Visioning::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    if (filters.size() == 0) {
      std::cout << "Process: Filters are empty" << std::endl;
      return;
    } else {
      for (std::vector<std::string>::iterator filter = filters.begin(); filter != filters.end(); ++filter) {
        if (filter->size() == 0) {
          std::cout << "Process: A filter is empty" << std::endl;
        }
      }
    }

    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera();

    Frame frontFrame, backFrame;
    if (frontCamera)
      frontFrame = frontCamera->getFrame();
    if (backCamera)
      backFrame = backCamera->getFrame();

    if (frontCamera != nullptr) {
      if (backCamera != nullptr) {
        Vision::process({&frontFrame, &backFrame}, filters);
      } else {
        Vision::process({&frontFrame}, filters);
      }
    }

    ballDetection();
    goalDetection();
    robotDetection();

  }

  void processCheckerboard() {
    if (filters.size() == 0) {
      std::cout << "Process: Filters are empty" << std::endl;
      return;
    } else {
      for (std::vector<std::string>::iterator filter = filters.begin(); filter != filters.end(); ++filter) {
        if (filter->size() == 0) {
          std::cout << "Process: A filter is empty" << std::endl;
        }
      }
    }

    Camera *frontCamera = hal::hw.getFrontCamera();
    Camera *backCamera = hal::hw.getBackCamera(); // TODO: Use

    Frame frontFrame, backFrame;
    if (frontCamera)
      frontFrame = frontCamera->getFrame();
    if (backCamera)
      backFrame = backCamera->getFrame();

    if (frontCamera)
      Vision::processCheckerboard(frontFrame, filters, 0);
    if (backCamera)
      Vision::processCheckerboard(backFrame, filters, 1);

    // TODO: Add back camera frame processing
  }

  void readFilterFromFile(const std::string &fileName) {
    std::cout << "Reading filter from " << fileName << std::endl;
    std::ifstream inputFile(fileName);
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    filters.push_back(buffer.str());
    inputFile.close();
  }

  // Unused
  void translateBallsBuffer() {
    editingBalls = true;

    balls.clear();
    balls = ballsBuffer;
    ballsBuffer.clear();

    editingBalls = false;
  }

  void translateGoalsBuffer() {
    editingGoals = true;

    const int mn_h = -5;
    const int mx_h = 5;

    Goal* g = blueGoal;
    Goal* new_g = blueGoalBuffer;

    if (blueGoal != nullptr) {

      if (blueGoalBuffer != nullptr) {
        blueGoal->update(*blueGoalBuffer->getTransform());
        blueGoal->update(*blueGoalBuffer->getBlob());
      } else {
        blueGoal->update();
      }

      if (blueGoal->getHealth() <= mn_h) {
        delete blueGoal;
      blueGoal = nullptr;
      }
    } else if (blueGoalBuffer != nullptr) {
      blueGoal = new Goal(*blueGoalBuffer);
    }

    if (yellowGoal != nullptr) {
      if (yellowGoalBuffer != nullptr) {
        yellowGoal->update(*yellowGoalBuffer->getTransform());
        yellowGoal->update(*yellowGoalBuffer->getBlob());
      } else {
        yellowGoal->update();
      }

      if(yellowGoal->getHealth() <= mn_h) {
        delete yellowGoal;
        yellowGoal = nullptr;
      }
    } else if(yellowGoalBuffer != nullptr) {
      yellowGoal = new Goal(*yellowGoalBuffer);
    }

    // TODO: Remove casting to null pointers when localisation is working
    blueGoalBuffer = nullptr;
    yellowGoalBuffer = nullptr;

    editingGoals = false;
  }

  // Unused
  void translateRobotsBuffer() {
    editingRobots = true;

    robots.clear();
    robots = robotsBuffer;
    robotsBuffer.clear();

    editingRobots = false;
  }

  double stateProbability(Transform* t1, Transform* t2) {
    const double A = 125.0;
    double px = A*gauss_prob2(t1->getX(), 120, t2->getX());
    double py = A*gauss_prob2(t1->getY(), 120, t2->getY());
    return 2*px*py / (px+py);
  }

  void ballDetection() {
    Vision::BlobSet blobs = Vision::getBlobs();
    BallSet n_balls;

    for(unsigned int i = 0; i < blobs.size(); ++i) {
      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();
      double ratio = blobs[i]->getBoxRatio();

      if(color != BALL) continue;
      if(boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if(density > 1.0) continue;
      if(boxArea < 4 * 4) continue;
      if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      double distance = blobs[i]->getDistance();
      double angle = blobs[i]->getAngle();

      n_balls.push_back(new Ball(Localization::toAbsoluteTransform(distance, angle), blobs[i], false));
    }

    double p, _p, p_ix;
    Ball* n_ball_ptr;
    BallSet* ball_set_ptr;
    for(int ix = 0; ix < n_balls.size(); ix++) {
      ballDetect.processProbableEntity(n_balls[ix]);
    }

    ballDetect.update();
  }

  void goalDetection() {
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
      //if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      // Relative position
      //std::pair<double, double> position = Vision::Perspective::virtualToReal(blobs[i]->getPosition(), blobs[i]->getCameraID());
      double distance = blobs[i]->getDistance();
      double angle = blobs[i]->getAngle();
      // std::cout << "Goal: " << distance << " " << angle << std::endl;
      //unsigned int distance = CAMERA_HEIGHT - point->getY();
      //double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
      // TODO: Remove duplicate code
      if (color == BLUE_GOAL) {
        if (boxArea > largestBlueArea) {
          largestBlueArea = boxArea;
          //if (blueGoalBuffer == nullptr) {
          blueGoalBuffer = new Goal(Localization::toAbsoluteTransform(distance, angle), blobs[i]);
          /*} else {
            blueGoalBuffer->setDistance(distance); // TODO: Compare with previous values as in ball detection
            blueGoalBuffer->setAngle(angle); // TODO: Compare with previous values as in ball detection
          }*/
        }
      } else {
        if (boxArea > largestYellowArea) {
          largestYellowArea = boxArea;
          //if (yellowGoalBuffer == nullptr) {
          yellowGoalBuffer = new Goal(Localization::toAbsoluteTransform(distance, angle), blobs[i]);
          /*} else {
            yellowGoalBuffer->setDistance(distance); // TODO: Compare with previous values as in ball detection
            yellowGoalBuffer->setAngle(angle); // TODO: Compare with previous values as in ball detection
          }*/
        }
      }

    }

    translateGoalsBuffer();
  }

  void robotDetection() {
    Vision::BlobSet blobs = Vision::getBlobs();
    RobotSet n_robots;

    for(unsigned int i = 0; i < blobs.size(); ++i) {
      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();

      if (color != ROBOT_YELLOW_BLUE && color != ROBOT_BLUE_YELLOW) continue;
      if (boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if (density > 1.0) continue;

      double distance = blobs[i]->getDistance();
      double angle = blobs[i]->getAngle();

      n_robots.push_back(new Robot(Localization::toAbsoluteTransform(distance, angle), blobs[i]));
    }

    double p, _p, p_ix;
    Robot* n_robot_ptr;
    RobotSet* robot_set_ptr;
    for (int ix = 0; ix < n_robots.size(); ix++) {
      robotDetect.processProbableEntity(n_robots[ix]);
    }

    robotDetect.update();
  }

}}
