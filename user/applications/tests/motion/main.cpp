#include <cstdlib>
#include <iostream>
#include <string>

#include "application.hpp"

#include "tuum_motion.hpp"
#include "tuum_visioning.hpp"
#include "tuum_navigation.hpp"

using namespace std;
using namespace rtx;

Timer debugTimer;

Vec2i p_vec, a_vec;

void test1() {
  Motion::stop();
  p_vec = Vec2i({100, 100});
  Motion::setPositionTarget(p_vec);
  a_vec = Vec2i({150, 175});

  std::cout << "Position target test..." << std::endl;
  std::cout << "Expected position: " << p_vec.toString() << std::endl;
  std::cout << "Expected motion angle: " << p_vec.getOrientation() << std::endl;
  std::cout << "Expected orientation: " << (a_vec - p_vec).getOrientation() << std::endl;

  std::cout << "Set target: " << p_vec.toString() << std::endl;
  std::cout << "Got target position: " << Motion::getTargetPosition().toString() << std::endl;
  std::cout << "Got target transform: " << Motion::getTargetTransform().toString() << std::endl;

  Motion::start();
  Motion::process();

  Motion::setAimTarget(a_vec);
  std::cout << "Set aim target: " << a_vec.toString() << std::endl;
  std::cout << "Got target orientation: " << Motion::getTargetOrientation() << std::endl;
  std::cout << "Got target transform: " << Motion::getTargetTransform().toString() << std::endl;

  Motion::start();
  Motion::process();
}

void test2() {
  Motion::stop();
  std::cout << std::endl << "Second test..." << std::endl;
  p_vec = Vec2i({100, 50});
  a_vec = Vec2i({150, 50});

  std::cout << "Expected position: " << p_vec.toString() << std::endl;
  std::cout << "Expected motion angle: " << p_vec.getOrientation() << std::endl;
  std::cout << "Expected orientation: " << (a_vec - p_vec).getOrientation() << std::endl;


  Motion::setPositionTarget(p_vec);
  std::cout << "Set target: " << p_vec.toString() << std::endl;
  std::cout << "Got target position: " << Motion::getTargetPosition().toString() << std::endl;
  std::cout << "Got target transform: " << Motion::getTargetTransform().toString() << std::endl;

  Motion::start();
  Motion::process();

  Motion::setAimTarget(a_vec);
  std::cout << "Set aim target: " << a_vec.toString() << std::endl;
  std::cout << "Got target orientation: " << Motion::getTargetOrientation() << std::endl;
  std::cout << "Got target transform: " << Motion::getTargetTransform().toString() << std::endl;

  Motion::start();
  Motion::process();
}

int ball_locate_ctrl() {
  if(Visioning::ballDetect.size() <= 0) return -1;

  Ball* b = Navigation::getNearestBall();
  if(b == nullptr) return -1;

  if(!Motion::isTargetAchieved() || !Motion::isRunning()) {
    Vec2i pos = Navigation::calcBallPickupPos(b->getTransform()).getPosition();
    Motion::setPositionTarget(pos);
    Motion::setAimTarget(b->getTransform()->getPosition());
    Motion::start();
  }
}

int goal_locate_ctrl() {

  if(Visioning::yellowGoal != nullptr) {
    if(!Motion::isTargetAchieved() || !Motion::isRunning()) {
      Motion::setAimTarget(Visioning::yellowGoal->getTransform()->getPosition());
      Motion::start();
    }
  }

  if(debugTimer.isTime()) {
    if(Visioning::yellowGoal != nullptr) {
      Transform* t = Visioning::yellowGoal->getTransform();
      //std::cout << "YelloGoal: " << t->toString() << std::endl;
    }
    debugTimer.start();
  }
}

int main(int argc, char* argv[]) {
  printf("Running motion tests...\n");
  //debugTimer.setPeriod(2000);

  hal::setup();

  Visioning::setup();
  Motion::setup();
  Motion::setBehaviour(Motion::MOT_COMPLEX);

  //test1();
  //test2();

  bool running = true;
  //hal::hw.getMainBoard()->senseBall();
  while(running) {
    hal::process();

    Visioning::process();
    Motion::process();

    //goal_locate_ctrl();
    ball_locate_ctrl();
  };

  printf("Motion tests done.\n");
  return 0;
}
