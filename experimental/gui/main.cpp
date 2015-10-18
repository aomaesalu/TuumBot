/**
  Tuum
  Robotex 2015
  University of Tartu
  Team:
    Kristjan Kanarbik
    Meelik Kiik
    Ants-Oskar Mäesalu
    Mart Müllerbeck
    Kaur Viiklaid
    Indrek Virro
    Mihkel Väljaots

  Experimental GUI class main file

  Contributors:
    Ants-Oskar Mäesalu

  Copyright (c) 2015 Ants-Oskar Mäesalu
*/

#include <QApplication>

#include "gui.h"

int main(int argc, char *argv[]) {
  // Initialise a Qt application
  QApplication app(argc, argv);

  // Create window and mark it as visible
  GUI window;
  window.show();

  // Enter message loop
  return app.exec();
}
