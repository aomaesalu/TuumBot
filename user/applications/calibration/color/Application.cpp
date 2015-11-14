/**
 *  @file Application.cpp
 *  Color calibration application class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#include "Application.hpp"


namespace rtx {

  Application::Application(int argc, char *argv[], Camera *frontCamera, Camera *backCamera):
    mask(8, CAMERA_WIDTH, CAMERA_HEIGHT),
    filter(8)
  {
    // Attach camera information
    frontCamera = frontCamera;
    backCamera = backCamera;

    // Initialise modes
    modes = {"Ball", "Blue goal", "Yellow goal", "Field", "White line", "Black line", "Checkerboard white", "Checkerboard black"}; // TODO: Read from file
    mode = 0;

    // Initialise properties
    playing = true;
    masking = false;

    // Create gtkmm application
    gtkApplication = Gtk::Application::create(argc, argv, "Robotex Tuum Color Calibration Application");

    // Create window
    window = MainWindow(this);
  }

  Application::~Application() {
    delete window;
  }

  Glib::RefPtr<Gtk::Application> Application::getGtkApplication() const {
    return gtkApplication;
  }

  MainWindow* Application::getWindow() const {
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

  void Application::setPlaying(const bool &value) {
    window->setPlaying(value);
    playing = value;
  }

  void Application::setMasking(const bool &value) {
    window->setMasking(value);
    masking = value;
  }

  void Application::readFilterFromFile(const std::string &fileName) {
    // TODO
  }

  void Application::saveFilterToFile(const std::string &fileName) {
    // TODO: Refactor filter transformation instructions
    setPlaying(true);
    setPlaying(false);

    // Output information to file
    std::ofstream outputFile(fileName);
    outputFile << filter.getString();
    outputFile.close();
  }

  int Application::run() {
    // Show windows and return when closed
    return gtkApplication->run(window);
  }

}
