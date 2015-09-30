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

  Camera experimental main class
    Program to test the V4L2 camera module.

  Contributors:
    Ants-Oskar Mäesalu

  Copyright (c) 2015 Ants-Oskar Mäesalu
*/

#include "camera.h"

#include <iostream>

int main() {
  Camera *camera = new Camera();
  std::cout << camera->getDevice() << " " << camera->getWidth() << " " << camera->getHeight() << std::endl;
  return 0;
}
