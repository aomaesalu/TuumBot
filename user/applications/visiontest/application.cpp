/** @file application.cpp
 *  Vision testing application.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 5 December 2015
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
    rtx::hal::process();

    Visioning::process();
    Navigation::preProcess();
    Localization::process();

    gui->updateFrame();
  }
}

int main(int argc, char *argv[]) {
  printf("main(): Tuum vision test application.\n");

  // Initialise constants
  rtx::init(argc, argv);

  // Initialize hardware
  rtx::hal::setup();

  // Initialize system modules
  Visioning::setup();
  Localization::setup();

  int returnValue = 0;
  if (hal::hw.getFrontCamera() != nullptr) {
    GUI guiFront(argc, argv, hal::hw.getFrontCamera());
    returnValue = guiFront.run();

    std::thread applicationThread(run, &guiFront);
    applicationThread.detach();
  }

  if (hal::hw.getBackCamera() != nullptr) {
    GUI guiBack(argc, argv, hal::hw.getBackCamera());
    returnValue = guiBack.run();

    std::thread applicationThread(run, &guiBack);
    applicationThread.detach();
  }

  return returnValue;
}
