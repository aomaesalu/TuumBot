/**
 *  @file GUI.cpp
 *  Team football application GUI interface class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#include "GUI.hpp"

#include "cameraConstants.hpp"


namespace rtx {

  GUI::GUI(int &argc, char *argv[], Camera *camera) {
    // Attach command line parameters
    this->argc = argc;
    this->argv = argv;

    // Attach camera information
    this->camera = camera;

    // Create gtkmm application
    gtkApplication = Gtk::Application::create(argc, argv);

    // Create window
    window = new MainWindow(this);

    // Create initial image
    initialiseImage();

    window->initialise();
  }

  GUI::~GUI() {
    // TODO: Delete window?
  }

  void GUI::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());
  }

  bool GUI::isColored(const unsigned int &mode, const unsigned int &pixel) const {
    return Vision::isColored(hal::hw.getFrontCamera()->getFrame(), Visioning::filter, pixel, mode);
  }

  bool GUI::isColored(const unsigned int &mode, const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
    return isColored(mode, (x << 16) + (y << 8) + z);
  }

  Glib::RefPtr<Gtk::Application> GUI::getGtkApplication() const {
    return gtkApplication;
  }

  MainWindow* GUI::getWindow() const {
    return window;
  }

  Camera* GUI::getCamera() const {
    return camera;
  }

  Frame* GUI::getFrame() {
    return &frame;
  }

  Glib::RefPtr<Gdk::Pixbuf> GUI::getImage() const {
    return image;
  }

  int Application::run() {
    // Show windows and return when closed
    return gtkApplication->run(*window);
  }

  bool Application::updateFrame() {
    frame = camera->getFrame();
    rgbFrame = toRGB(frame);
    window->getImageArea()->queue_draw();
    return true;
  }

}
