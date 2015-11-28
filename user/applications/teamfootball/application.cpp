/** @file application.cpp
 *  Robotex Teamfootball application.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "application.hpp"

#include "rtx_tfb.hpp"

#include <iostream> // TODO: Remove

using namespace std;
using namespace rtx;

int main(int argc, char *argv[]) {
  printf("main(): Tuum team football application.\n");

  // Initialize hardware
  rtx::init(argc, argv);
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
