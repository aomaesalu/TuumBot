/**
 * @file MainWindow.cpp
 * Experimental GUI application main window.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "MainWindow.hpp"


MainWindow::MainWindow() {
  setProperties();

  add(grid);

  show_all_children();
}

MainWindow::~MainWindow() {
  // Nothing to do here
}

void MainWindow::setProperties() {
  // Set window properties
  set_title("Main window");
  set_size_request(300, 300);
  set_resizable(false);
  // Set window border width
  set_border_width(10);
}
