/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 19 November 2015
 */

#include <algorithm>
#include <fstream>
#include <iostream> // TODO: Remove
#include <sstream>

#include <boost/thread/mutex.hpp>

#include "__future__.hpp"
#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"
#include "mathematicalConstants.hpp"

using namespace boost;
using namespace rtx;

namespace rtx { namespace Visioning {

  std::string filter;

  Timer debugTimer;

  // Entity Detection State
  template<class T>
  struct EDS {
    int mn_h = -5; // Entity removal health condition
    int mx_h = 5;  // Entity detection health condition

    std::vector<T*> objs;
    std::vector<T*> tmp_objs;
    mutex mLock;

    std::vector<T*>* getEntities() {
      mutex::scoped_lock scoped_lock(mLock);
      return &objs;
    }

    std::vector<T*>* getTmpEntities() {
      mutex::scoped_lock scoped_lock(mLock);
      return &tmp_objs;
    }

    void filter() {
      int health = mn_h;
      tmp_objs.erase(std::remove_if(tmp_objs.begin(), tmp_objs.end(),
        [health](T*& obj_ptr) {
          return obj_ptr->getHealth() < health;
        }), tmp_objs.end());

      //TODO: move healthy objects
      //TODO: move decaying objects
    }
  };

  //BallSet* Visioning::getBalls();
  EDS<Ball> ballDetect;

  FeatureSet features;
  BallSet balls; // Healty ball entities vs new/decaying ball entities?
  BallSet ballsBuffer;

  GoalSet goals;
  GoalSet goalsBuffer;

  RobotSet robots;
  RobotSet robotsBuffer;

  bool editingBalls = false;
  bool editingGoals = false;
  bool editingRobots = false;

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
    inputFile >> filter;
    inputFile.close();
  }

  void translateBallsBuffer() {
    editingBalls = true;

    balls.clear();
    balls = ballsBuffer;
    ballsBuffer.clear();

    editingBalls = false;
  }

  void translateGoalsBuffer() {
    editingGoals = true;

    goals.clear();
    goals = goalsBuffer;
    goalsBuffer.clear();

    editingGoals = false;
  }

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

    Vision::BlobSet blobs = Vision::blobs;
    while (Vision::editingBlobs) {
      blobs = Vision::blobs;
    }

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

      // STEP 1: Filter out invalid blobs
      if(color != BALL) continue;
      if(boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if(boxArea < 20 * 20) continue;
      if(density > 1.0) continue;
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
    ballDetect.filter();

    if(debugTimer.isTime()) {

      std::cout << "[Visioning]Balls: " << ballDetect.getEntities()->size()
	        << ". Unconfirmed balls: " << ballDetect.getTmpEntities()->size()
		<< std::endl;

      for(auto& b : *(ballDetect.getTmpEntities())) {
	Transform* t = b->getTransform();
        std::cout << "<Ball x=" << t->getX() << ", y=" << t->getY() << ">" << std::endl;
      }

      debugTimer.start();
    }
 }

  void goalDetection(const Frame &frame) {
    goalsBuffer.clear();

    for (unsigned int i = 0; i < Vision::blobs.size(); ++i) {
      if (Vision::blobs[i]->getColor() == BLUE_GOAL) {
        // TODO: Refactor
        Point2D* point = Vision::blobs[i]->getPosition();
        unsigned int distance = CAMERA_HEIGHT - point->getY(); // TODO: Calculate based on perspective
        double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
        goalsBuffer.push_back(new Goal(distance, angle));
      } else if (Vision::blobs[i]->getColor() == YELLOW_GOAL) {
        // TODO: Refactor
        Point2D* point = Vision::blobs[i]->getPosition();
        unsigned int distance = CAMERA_HEIGHT - point->getY(); // TODO: Calculate based on perspective
        double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;
        goalsBuffer.push_back(new Goal(distance, angle));
      }
    }

    translateGoalsBuffer();
  }

  void robotDetection(const Frame &frame) {
    robotsBuffer.clear();

    // TODO

    translateRobotsBuffer();
  }

}}
