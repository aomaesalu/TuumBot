/** @file application.cpp
 *  Team football application entry module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 02. November 2015
 */

#include <iostream>

#include "application.hpp"
#include "tfb_logic.hpp"

using namespace std;
using namespace rtx;

int main() {
  printf("main(): starting Team Football Application (tfb)\n");

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
