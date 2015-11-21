/** @file application.cpp
 *  Perspective calibration application.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#include "application.hpp"
#include "GUI.hpp"

#include <thread>
#include <iostream> // TODO: Remove

using namespace std;
using namespace rtx;

static void run(GUI *gui) {
  bool running = true;
  while(running) {
    gui->updateFrame();
  }
}

int main(int argc, char *argv[]) {
  printf("main(): Default tuum system application.\n");

  // Initialize hardware
  rtx::hal::setup();

  GUI gui(argc, argv, hal::hw.getFrontCamera());

  std::thread applicationThread(run, &gui);
  applicationThread.detach();

  return gui.run();
}
