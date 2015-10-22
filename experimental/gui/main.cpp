/**
 * @file main.cpp
 * Experimental GUI application using gtkmm.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <gtkmm/application.h>

#include "MainWindow.hpp"


int main(int argc, char *argv[]) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

  MainWindow window;

  // Show windows and return when closed
  return app->run(window);
}
