/** @file tuum_vision.cpp
 *  Vision system implementation.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.1
 *  @date 17 November 2015
 */

#include "__future__.hpp"
#include "tuum_visioning.hpp"
#include "mathematicalConstants.hpp"

#include <algorithm>
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

  double ballProbability(Ball* b1, Ball* b2) {
    double p1 = gaussian_probability(b1->getDistance(), 30, b2->getDistance());
    double c = 180/3.14;
    double p2 = gaussian_probability(b1->getAngle()*c, 5, b2->getAngle()*c);

    return (p1 + p2) / 2;
  }

  void ballDetection(const Frame &frame) {

    Vision::BlobSet blobs = Vision::blobs;
    while (Vision::editingBlobs) {
      blobs = Vision::blobs;
    }

    BallSet n_balls;
    for(unsigned int i = 0; i < blobs.size(); ++i) {

      Color color = blobs[i]->getColor();
      double density = blobs[i]->getDensity();
      unsigned int boxArea = blobs[i]->getBoxArea();

      if(color != BALL) continue;
      if(boxArea > CAMERA_WIDTH * CAMERA_HEIGHT) continue;
      if(boxArea < 20 * 20) continue;
      if(density > 1.0) continue;
      /* && density > 0.6*/

      //std::cout << "Dim: " << blobs[i]->getDensity() << " " << blobs[i]->getBoxArea() << std::endl;
      Point2D* point = blobs[i]->getPosition();

      // TODO: Calculate based on perspective
      unsigned int distance = CAMERA_HEIGHT - point->getY();
      double angle = (1 - point->getX() / (CAMERA_WIDTH / 2.0)) * 20 * PI / 180;

      n_balls.push_back(new Ball(distance, angle));
    }

    double p, _p, p_ix;
    Ball* n_ball_ptr;
    for(int ix = 0; ix < n_balls.size(); ix++) {
      p = 0.0;
      n_ball_ptr = n_balls[ix];
      for(int jx = 0; jx < balls.size(); jx++) {
        _p = ballProbability(balls[jx], n_ball_ptr);
        if(_p > p) {
          p = _p;
          p_ix = jx;
        }
      }

      if(p < 0.01) {
        balls.push_back(new Ball(*n_ball_ptr));
      } else {
        balls[p_ix]->update(n_ball_ptr->getDistance(), n_ball_ptr->getAngle());
      }
    }

    balls.erase(std::remove_if(balls.begin(), balls.end(), [](Ball*& b) {
        return b->decay() < -5;
    }), balls.end());
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
