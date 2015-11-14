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

  Application::Application(int argc, char *argv[], Camera *camera):
    mask(8, CAMERA_WIDTH, CAMERA_HEIGHT),
    filter(8)
  {
    // Attach camera information
    camera = camera;

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

  void Application::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());

    // Show the whole image
    if (image)
      set_size_request(image->get_width(), image->get_height());
  }

  Glib::RefPtr<Gtk::Application> Application::getGtkApplication() const {
    return gtkApplication;
  }

  MainWindow* Application::getWindow() const {
    return mainWindow;
  }

  Camera* Application::getCamera() const {
    return camera;
  }

  Glib::RefPtr<Gdk::Pixbuf> Application::getImage() const {
    return image;
  }

  bool Application::isPlaying() const {
    return playing;
  }

  bool Application::isMasking() const {
    return masking;
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

  bool Application::updateFrame() {
    if (!playing) {
      return false;
    }
    frame = camera->getFrame();
    rgbFrame = toRGB(frame);
    window->updateFrame(&frame, &rgbFrame);
    return true;
  }

}
