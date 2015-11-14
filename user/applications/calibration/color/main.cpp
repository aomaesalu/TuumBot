/**
 * @file main.cpp
 * Color calibration application main module.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <gtkmm/application.h>
#include <thread>
#include <ctime> // For calculating FPS

#include <iostream> // TODO: Remove

#include "rtxhal.hpp"

#include "Application.hpp"

using namespace rtx;


static void process(Application *application) {
  unsigned int frameCounter = 0;
  clock_t startTime = clock();
  clock_t lastTime = startTime;
  while (true) {
    clock_t currentTime = clock();
    if (float(currentTime - lastTime) / CLOCKS_PER_SEC > 1) {
      //std::cout << "FPS: " << frameCounter << std::endl;
      frameCounter = 0;
      lastTime = currentTime;
    }
    if (application->updateFrame()) {
      frameCounter++;
    }
  }
}

int main(int argc, char *argv[]) {
  // Initialise hardware
  rtx::hal::setup();

  // Create application
  Application application(argc, argv, hal::hw.getFrontCamera());

  // Run camera frame thread // TODO: Move to application class
  std::thread frameThread(process, &application);
  frameThread.detach();

  // Run application and return when closed
  return application.run();
}
