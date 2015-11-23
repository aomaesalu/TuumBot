#include <cstdlib>
#include <iostream>
#include <string>

#include "application.hpp"

#include "rtxhal.hpp"

#include "tuum_motion.hpp"
#include "tuum_visioning.hpp"
#include "tuum_navigation.hpp"

using namespace std;
using namespace rtx;
using namespace hal;

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


Goal* getGoal() {
  return Visioning::yellowGoal;
}

bool ballInDribbler() {
  if(hal::hw.getMainBoard()->getBallSensorState()) {
    hal::hw.getMainBoard()->startDribbler();
    return true;
  } else {
    hal::hw.getMainBoard()->stopDribbler();
    return false;
  }
}

int ball_scan_ctrl() {
  if(ballInDribbler()) return 1;

  if(Visioning::ballDetect.size() > 0) {
    Motion::stop();
    return 1;
  }

  Motion::setAimTarget(Vec2i({1, 1}));
  if(!Motion::isRunning()) Motion::start();
  return 0;
}

int goal_scan_ctrl() {
  if(getGoal() != nullptr) return 1;
  if(!ballInDribbler()) return -1;

  Motion::setAimTarget(Vec2i({1, -1}));
  if(!Motion::isRunning()) Motion::start();
  return 0;
}

int ball_locate_ctrl() {
  if(ballInDribbler()) return 1;
  if(Visioning::ballDetect.size() <= 0) return -1;

  Ball* b = Navigation::getNearestBall();
  if(b == nullptr) return -1;


  Vec2i pos = Navigation::calcBallPickupPos(b->getTransform()).getPosition();
  Motion::setPositionTarget(pos);
  Motion::setAimTarget(b->getTransform()->getPosition());
  
  Transform* t = Localization::getTransform();
  double d = t->distanceTo(b->getTransform()->getPosition());

  MainBoard* mb = hal::hw.getMainBoard();
  std::cout << d << std::endl;
  if(d < 250) {
    mb->startDribbler();
  } else {
    mb->stopDribbler();
  }

  if(!Motion::isTargetAchieved()) {
    if(!Motion::isRunning()) Motion::start();
  }

  if(mb->getBallSensorState()) return 1;

  return 0;
}


int goal_shoot_ctrl() {
  std::cout << "GOAL SHOOT CTRL" << std::endl;
  if(getGoal() == nullptr) return -1;
  std::cout << "got goal";

  if(!ballInDribbler()) return -1;
  std::cout << "got ball" << std::endl;

  Motion::setAimTarget(getGoal()->getTransform()->getPosition());
  if(!Motion::isTargetAchieved()) {
    if(!Motion::isRunning()) Motion::start();
  } else {
    hal::hw.getMainBoard()->doCoilKick();
  }

  return 0;
}

enum LST {
  BALL_SCAN,
  BALL_RETRIEVE,
  GOAL_SCAN,
  GOAL_SHOOT,
  HALT,
};

LST state = BALL_SCAN;

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
  std::cout << "BALL_SCAN" << std::endl;
  //hal::hw.getMainBoard()->senseBall();
  while(running) {
    hal::process();

    Visioning::process();
    Motion::process();

    //goal_locate_ctrl();
    int res;
    switch(state) {
      case BALL_SCAN:
	res = ball_scan_ctrl();
	if(res == 0) break;
	else if(res > 0) {
	  std::cout << "ENTER BALL_RETRIEVE" << std::endl;
	  state = BALL_RETRIEVE;
	  Motion::stop();
	}
	break;
      case BALL_RETRIEVE:
	res = ball_locate_ctrl();
	if(res == 0) break;
	else if(res > 0) {
	  state = GOAL_SCAN;
	  std::cout << "ENTER GOAL_SCAN" << std::endl;
	  Motion::stop();
	}
	else if(res < 0) {
	  std::cout << "ENTER BALL_SCAN" << std::endl;
	  state = BALL_SCAN;
	  Motion::stop();
	}
	break;
      case GOAL_SCAN:
	res = goal_scan_ctrl();
	if(res == 0) break;
	else if(res > 0) {
          Motion::stop();
	  state = GOAL_SHOOT;
	  std::cout << "ENTER GOAL SHOOT" << std::endl;
	}
	else if(res < 0) {
	  Motion::stop();
          state = BALL_RETRIEVE;
	  std::cout << "ENTER BALL RETRIEVE" << std::endl;
	}
	break;
      case GOAL_SHOOT:
	res = goal_shoot_ctrl();
	if(res == 0) break;
	else if(res > 0) {
	  Motion::stop();
          state = BALL_RETRIEVE;
	}
	else if(res < 0) {
          Motion::stop();
	  state = GOAL_SCAN;
	}
	break;
      case HALT:
	break;
    }

  };

  printf("Motion tests done.\n");
  return 0;
}
