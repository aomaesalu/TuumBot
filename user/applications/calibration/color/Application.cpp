/**
 *  @file Application.cpp
 *  Color calibration application class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November, 2015
 */

#include "Application.hpp"


namespace rtx {

  Application::Application(Camera *frontCamera, Camera *backCamera) {
    // TODO
  }

  Application::~Application() {
    // TODO
  }

  MainWindow* Application::getMainWindow() const {
    return mainWindow;
  }

  Camera* Application::getFrontCamera() const {
    return frontCamera;
  }

  Camera* Application::getBackCamera() const {
    return backCamera;
  }

  std::vector<std::string> Application::getModes() const {
    return modes;
  }

  unsigned int Application::getMode() const {
    return mode;
  }

  void setPlaying(const bool &value) {
    // TODO
  }

  void setMasking(const bool &value) {
    // TODO
  }

  void run() {
    // TODO
  }

}
