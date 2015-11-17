/**
 *  @file Interface.cpp
 *  Team football application GUI interface class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#include "Interface.hpp"

#include "cameraConstants.hpp"


namespace rtx {

  Interface::Interface(int &argc, char *argv[], Camera *camera) {
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

  Interface::~Interface() {
    // TODO: Delete window?
  }

  void Interface::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());
  }

  Glib::RefPtr<Gtk::Application> Interface::getGtkApplication() const {
    return gtkApplication;
  }

  MainWindow* Interface::getWindow() const {
    return window;
  }

  Camera* Interface::getCamera() const {
    return camera;
  }

  Frame* Interface::getFrame() {
    return &frame;
  }

  Glib::RefPtr<Gdk::Pixbuf> Interface::getImage() const {
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
