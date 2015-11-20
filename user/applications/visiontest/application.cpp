/** @file application.cpp
 *  Robotex Teamfootball application.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "application.hpp"
#include "GUI.hpp"

#include <thread>
#include <iostream> // TODO: Remove

using namespace std;
using namespace rtx;

static void run(GUI *gui) {
  clock_t startTime = clock();
  clock_t lastTime = startTime;
  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Localization::process();

    gui->updateFrame();
  }
}

int main(int argc, char *argv[]) {
  printf("main(): Default tuum system application.\n");

  // Initialize hardware
  rtx::hal::setup();

  // Initialize system modules
  Visioning::setup();
  Localization::setup();

  GUI gui(argc, argv, hal::hw.getFrontCamera());

  std::thread applicationThread(run, &gui);
  applicationThread.detach();

  return gui.run();
}
