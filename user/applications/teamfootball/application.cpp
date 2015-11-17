/** @file application.cpp
 *  Robotex Teamfootball application.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "application.hpp"
#include <iostream>

#include "tfb_logic.hpp"

using namespace std;
using namespace rtx;

int main() {
  printf("main(): Default tuum system application.\n");

  // Initialize hardware
  rtx::hal::setup();

  // Initialize system modules
  Visioning::setup();
  Localization::setup();
  Motion::setup();

  Logic::setup();

  clock_t startTime = clock();
  clock_t lastTime = startTime;
  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Localization::process();
    Motion::process();

    Logic::process();

    clock_t currentTime = clock();
    if (float(currentTime - lastTime) / CLOCKS_PER_SEC > 1) {
      for (Visioning::BallSet::iterator ball = Visioning::balls.begin(); ball != Visioning::balls.end(); ++ball) {
        std::cout << "Ball(d:" << (*ball)->getDistance() << ", a:" << (*ball)->getAngle() << ");" << std::endl;
      }
      lastTime = currentTime;
    }
  }

  return 0;
}
