/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 20 November 2015
 */

#include <algorithm>
#include <fstream>
#include <iostream> // TODO: Remove
#include <sstream>

#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"
#include "mathematicalConstants.hpp"

using namespace rtx;

namespace rtx { namespace Visioning {

  std::string filter;

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

    // TODO: Refactor buffer management

    if (blueGoal) {
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

  void featureDetection(const Frame &frame) {
    features.clear();
    // TODO
  }

  double stateProbability(Transform* t1, Transform* t2) {
    double px = gaussian_probability(t1->getX(), 30, t2->getX());
    double py = gaussian_probability(t1->getY(), 30, t2->getY());
    return (px + py) / 2;
  }

  void ballDetection(const Frame &frame) {

    Vision::BlobSet blobs = Vision::getBlobs();

    BallSet n_balls;

    /*
    std::stringstream dbg;
    dbg << "Ball detect debug:" << std::endl;
    bool dbg_available = false;
    */

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

      //std::cout << "Dim: " << blobs[i]->getDensity() << " " << blobs[i]->getBoxArea() << std::endl;

      // STEP 2: Calculate relative position
      Point2D* point = blobs[i]->getPosition();
      // TODO: Calculate based on perspective
      unsigned int distance = CAMERA_HEIGHT - point->getY();
      double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;

      /*
      dbg << "<Blob d=" << distance << ", a=" << angle
	        << ", S=" << boxArea << ", ro=" << density
	        << std::endl;
      dbg_available = true;
      */

      // STEP 3: Create ball instance with absolute position
      //std::cout << "New ball: d=" << distance << ", a=" << angle << ", r=" << fabs(1.0 - ratio) << std::endl;
      n_balls.push_back(new Ball(Localization::toAbsoluteTransform(distance, angle)));
    }

    /*
    if(dbg_available) {
      dbg << "Ball detect debug end" << std::endl << std::endl;
      std::cout << dbg.str();
    }
    */

    // STEP 4: Unite detected balls with balls from last frame or create new balls
    double p, _p, p_ix;
    Ball* n_ball_ptr;
    BallSet* ball_set_ptr;
    for(int ix = 0; ix < n_balls.size(); ix++) {
      p = 0.0;
      n_ball_ptr = n_balls[ix];

      // STEP 4.1: Calculate existing entity probability
      ball_set_ptr = &(ballDetect.objs);
      for(int jx = 0; jx < ballDetect.objs.size(); jx++) {
        _p = stateProbability((*ball_set_ptr)[jx]->getTransform(), n_ball_ptr->getTransform());
        if(_p > p) {
          p = _p;
          p_ix = jx;
        }
      }

      for(int jx = 0; jx < ballDetect.tmp_objs.size(); jx++) {
        _p = stateProbability(ballDetect.tmp_objs[jx]->getTransform(), n_ball_ptr->getTransform());
        if(_p > p) {
          p = _p;
          p_ix = jx;
	        if(ball_set_ptr != &(ballDetect.tmp_objs)) ball_set_ptr = &(ballDetect.tmp_objs);
        }
      }

      // STEP 4.2: Create or update entities
      if(p < 0.01) {
        ballDetect.tmp_objs.push_back(new Ball(*n_ball_ptr));
      } else {
        (*ball_set_ptr)[p_ix]->update(*n_ball_ptr->getTransform());
      }
    }

    // STEP 5: Entity vectors updates - remove decayed balls and make healthy detectable
    ballDetect.update();

    if(debugTimer.isTime()) {
      /*std::cout << "[Visioning]Balls: " << ballDetect.getEntities()->size()
	        << ". Unconfirmed balls: " << ballDetect.getTmpEntities()->size()
		<< std::endl;
      */

      /*for(auto& b : *(ballDetect.getEntities())) {
	Transform* t = b->getTransform();
        std::cout << "<Ball hp=" << b->getHealth() << ", x=" << t->getX() << ", y=" << t->getY() << ">" << std::endl;
      }*/

      debugTimer.start();
    }
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
      //if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      Point2D* point = blobs[i]->getPosition();
      unsigned int distance = CAMERA_HEIGHT - point->getY(); // TODO: Calculate based on perspective
      double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
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

      // STEP 1: Filter out invalid blobs
      if (color != ROBOT_YELLOW_BLUE && color != ROBOT_BLUE_YELLOW) continue;
      if (boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if (density > 1.0) continue;
      //if(boxArea < 8 * 3) continue; // TODO: Calibrate with field tests
      //if(fabs(1 - ratio) > 0.3) continue;
      /* && density > 0.6*/

      // STEP 2: Calculate relative position
      Point2D* point = blobs[i]->getPosition();
      // TODO: Calculate based on perspective
      unsigned int distance = CAMERA_HEIGHT - point->getY();
      double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;

      // STEP 3: Create robot instance with absolute position
      n_robots.push_back(new Robot(Localization::toAbsoluteTransform(distance, angle)));
    }

    // STEP 4: Unite detected robots with robots from last frame or create new robots
    double p, _p, p_ix;
    Robot* n_robot_ptr;
    RobotSet* robot_set_ptr;
    for (int ix = 0; ix < n_robots.size(); ix++) {
      p = 0.0;
      n_robot_ptr = n_robots[ix];

      // STEP 4.1: Calculate existing entity probability
      robot_set_ptr = &(robotDetect.objs);
      for (int jx = 0; jx < robotDetect.objs.size(); jx++) {
        _p = stateProbability((*robot_set_ptr)[jx]->getTransform(), n_robot_ptr->getTransform());
        if (_p > p) {
          p = _p;
          p_ix = jx;
        }
      }

      for (int jx = 0; jx < robotDetect.tmp_objs.size(); jx++) {
        _p = stateProbability(robotDetect.tmp_objs[jx]->getTransform(), n_robot_ptr->getTransform());
        if (_p > p) {
          p = _p;
          p_ix = jx;
          if(robot_set_ptr != &(robotDetect.tmp_objs)) robot_set_ptr = &(robotDetect.tmp_objs);
        }
      }

      // STEP 4.2: Create or update entities
      if (p < 0.01) {
        robotDetect.tmp_objs.push_back(new Robot(*n_robot_ptr));
      } else {
        (*robot_set_ptr)[p_ix]->update(*n_robot_ptr->getTransform());
      }
    }

    // STEP 5: Entity vectors updates - remove decayed balls and make healthy detectable
    robotDetect.update();
  }

}}
