/** @file application.cpp
 *  Robotex Teamfootball application.
 *
 *  @authors Meelik Kiik
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
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

  TFBLogic::setup();

  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Navigation::preProcess();
    Localization::process();
    Motion::process();

    TFBLogic::process();
  }

  return 0;
}
