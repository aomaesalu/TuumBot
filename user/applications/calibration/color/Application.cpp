/**
 *  @file Application.cpp
 *  Color calibration application class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#include "Application.hpp"

#include "cameraConstants.hpp"

#include <fstream>


namespace rtx {

  Application::Application(int &argc, char *argv[], Camera *camera):
    mask(8, CAMERA_WIDTH, CAMERA_HEIGHT),
    filter(8)
  {
    // Attach camera information
    this->camera = camera;

    // Initialise modes
    modes = {"Ball", "Blue goal", "Yellow goal", "Field", "White line", "Black line", "Checkerboard white", "Checkerboard black"}; // TODO: Read from file
    mode = 0;

    // Initialise properties
    playing = true;
    masking = false;

    // Create initial image
    initialiseImage();

    // Create gtkmm application
    gtkApplication = Gtk::Application::create(argc, argv, "Robotex Tuum Color Calibration Application");

    // Create window
    window = new MainWindow(this);
  }

  Application::~Application() {
    delete window;
  }

  void Application::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());
  }

  Glib::RefPtr<Gtk::Application> Application::getGtkApplication() const {
    return gtkApplication;
  }

  MainWindow* Application::getWindow() const {
    return window;
  }

  Camera* Application::getCamera() const {
    return camera;
  }

  Frame* Application::getFrame() {
    return &frame;
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

  Mask* Application::getMask() {
    return &mask;
  }

  Filter* Application::getFilter() {
    return &filter;
  }

  void Application::setPlaying(const bool &value) {
    if (value) {
      setMasking(false);
      filter.addBuffersToFilter();
    }
    window->setPlaying(value);
    playing = value;
  }

  void Application::setMasking(const bool &value) {
    if (value) {
      if (!isMasking()) {
        setPlaying(false);
      }
    } else {
      mask.reset();
      window->getMaskingArea()->resetDrawingModes();
    }
    masking = value;
  }

  void Application::setMode(const unsigned int &value) {
    mode = value;
  }

  void Application::sendMasksToFilter() {
    filter.addMasksToBuffers(frame, mask.getAdditionValues(), mask.getRemovalValues());
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
    return gtkApplication->run(*window);
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
