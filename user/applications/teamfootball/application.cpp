/** @file application.cpp
 *  Robotex Teamfootball application.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "application.hpp"

#include "tfb_logic.hpp"

#include <iostream> // TODO: Remove

using namespace std;
using namespace rtx;

int main(int argc, char *argv[]) {
  printf("main(): Default tuum system application.\n");

  // Initialize hardware
  rtx::hal::setup();

  // Initialize system modules
  Visioning::setup();
  Localization::setup();
  Motion::setup();

  Logic::setup();
  
  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Localization::process();
    Motion::process();

    Logic::process();
  }

  return 0;
}
