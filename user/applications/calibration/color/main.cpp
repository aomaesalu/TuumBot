/**
 * @file main.cpp
 * Color calibration application main module.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <gtkmm/application.h>
#include <thread>

#include <iostream> // TODO: Remove

#include "cameraConstants.hpp"
#include "Camera.hpp"

#include "MainWindow.hpp"

using namespace rtx;


static void process(MainWindow *window) {
  while (true) {
    window->updateFrame();
  }
}

int main(int argc, char *argv[]) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

  Camera camera(CAMERA_DEVICE, CAMERA_WIDTH, CAMERA_HEIGHT);

  MainWindow window(&camera);

  std::thread frameThread(process, &window);
  frameThread.detach();

  // Show windows and return when closed
  return app->run(window);
}
