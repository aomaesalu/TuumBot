#include <cstdlib>
#include <iostream>
#include <string>

#include "application.hpp"

#include "tuum_motion.hpp"
#include "tuum_visioning.hpp"

using namespace std;
using namespace rtx;

Timer debugTimer;

int main(int argc, char* argv[]) {
  printf("Running motion tests...\n");
  debugTimer.setPeriod(2000);

  hal::setup();

  Motion::setup();


  bool running = true;
  hal::hw.getMainBoard()->senseBall();
  while(running) {
    hal::process();

    Motion::process();

    if(Visioning::blueGoal != nullptr) {
      Motion::setBehaviour(Motion::MOT_AIM);
      Motion::setTarget(*(Visioning::blueGoal->getTransform()));
      if(!Motion::isRunning()) Motion::start();
    }

    if(debugTimer.isTime()) {
      std::cout << "DEBUG" << std::endl;
      std::cout << "BlueGoal: " << (Visioning::blueGoal != nullptr) << std::endl;
      debugTimer.start();
    }
  };

  printf("Motion tests done.\n");
  return 0;
}
