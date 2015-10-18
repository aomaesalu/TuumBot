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

  GUI class

  Contributors:
    Ants-Oskar Mäesalu

  Copyright (c) 2015 Ants-Oskar Mäesalu
*/

#ifndef GUI_H
#define GUI_H

#include <QMainWindow>

class GUI: public QMainWindow {
  Q_OBJECT

  public:
    GUI(QWidget* = 0);
    ~GUI();
};

#endif // GUI_H
