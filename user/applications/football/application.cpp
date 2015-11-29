/** @file application.cpp
 *  Robotex Teamfootball application.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include <thread>
#include <iostream> // TODO: Remove

#include "application.hpp"
#include "rtx_fb.hpp"

using namespace std;
using namespace rtx;


int main(int argc, char *argv[]) {
  printf("main(): Tuum Robotex 1vs1 application.\n");
  rtx::init(argc, argv);
  rtx::hal::setup();

  Visioning::setup();
  Localization::setup();
  Motion::setup();

  FBLogic::setup();

  bool running = true;
  while(running) {
    hal::process();

    Visioning::process();
    Localization::process();
    Motion::process();

    FBLogic::process();
  }

  return 0;
}
