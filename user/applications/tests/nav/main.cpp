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


int main(int argc, char* argv[]) {
  printf("Running navigation tests...\n");
  debugTimer.setPeriod(1000);
  rtx::init(argc, argv);

  hal::setup();

  Visioning::setup();
  Motion::setup();

  bool running = true;
  while(running) {
    hal::process();

    Visioning::process();
    Motion::process();

    if(debugTimer.isTime()) {

      Goal* g = Navigation::getOpponentGoal();
      if(g != nullptr) {
        std::cout << "Goal: " << g->toString() << std::endl;
      }

      Robot* ally = Navigation::getAlly();
      if(ally != nullptr) {
        std::cout << "Ally: " << ally->toString() << std::endl;
      }

      debugTimer.start();
    }
  }

  printf("Navigation tests done.\n");
  return 0;
}
