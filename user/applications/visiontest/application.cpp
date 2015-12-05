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

static void runGUI(int argc, char *argv[], GUI *gui) {
  gui->run();
}

static void run(GUI *guiFront, GUI *guiBack) {
  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Navigation::preProcess();
    Localization::process();

    if (guiFront != nullptr)
      guiFront->updateFrame();
    if (guiBack != nullptr)
      guiBack->updateFrame();

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

  if (hal::hw.getFrontCamera() != nullptr) {
    if (hal::hw.getBackCamera() != nullptr) {
      GUI guiFront(argc, argv, hal::hw.getFrontCamera());
      std::thread guiFrontThread(runGUI, argc, argv, &guiFront);
      guiFrontThread.detach();
      GUI guiBack(argc, argv, hal::hw.getBackCamera());
      std::thread guiBackThread(runGUI, argc, argv, &guiBack);
      guiBackThread.detach();
      std::thread applicationThread(run, &guiFront, &guiBack);
      applicationThread.detach();
    } else {
      GUI guiFront(argc, argv, hal::hw.getFrontCamera());
      std::thread guiFrontThread(runGUI, argc, argv, &guiFront);
      guiFrontThread.detach();
      std::thread applicationThread(run, &guiFront, nullptr);
      applicationThread.detach();
    }
  } else {
    if (hal::hw.getBackCamera() != nullptr) {
      GUI guiBack(argc, argv, hal::hw.getBackCamera());
      std::thread guiBackThread(runGUI, argc, argv, &guiBack);
      guiBackThread.detach();
      std::thread applicationThread(run, nullptr, &guiBack);
      applicationThread.detach();
    }
  }

  std::cin.get();

  return 0;
}
