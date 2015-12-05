#include <cstdlib>
#include <iostream>
#include <string>

#include "application.hpp"

#include "rtx_ctl.hpp"

#include "rtxhal.hpp"

#include "tuum_motion.hpp"
#include "tuum_visioning.hpp"
#include "tuum_navigation.hpp"

using namespace std;
using namespace rtx;
using namespace hal;

Timer debugTimer;


int main(int argc, char* argv[]) {
  printf("Running navigation tests...\n");
  debugTimer.setPeriod(1000);
  rtx::init(argc, argv);

  hal::setup();

  Visioning::setup();
  Motion::setup();

  Context ctx;

  bool running = true;
  while(running) {
    hal::process();

    Visioning::process();
    Navigation::preProcess();
    Motion::process();

    if(debugTimer.isTime()) {

      Goal* g = Navigation::getOpponentGoal();
      if(g != nullptr) {
        std::cout << "Goal: " << g->toString() << std::endl;
      }

      Robot* ally = Navigation::getAlly();
      if(ally != nullptr) {
        //std::cout << "Ally: " << ally->toString() << std::endl;
      }

      if(Visioning::ballDetect.size() > 0) {
	std::cout << "Balls: " << std::endl;
	for(auto& b : *Visioning::ballDetect.getEntities()) {
          //std::cout << b->toString() << " : isValid() == " << (b->isValid()) << std::endl;
	
      }

      debugTimer.start();
    }

    }
    auto b = Navigation::getNearestBall();
    if(b != nullptr) {
      Motion::setAimTarget(b->getTransform()->getPosition());
      Vec2i pos = Navigation::calcBallPickupPos(b->getTransform()).getPosition();
      Motion::setPositionTarget(pos);

      if(!Motion::isRunning()) Motion::start();
    } else {
      Motion::stop();
    }
  }

  printf("Navigation tests done.\n");
  return 0;
}
