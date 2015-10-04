/**
 * @file main.cpp
 * Football 2vs2 application main module.
 *
 * @authors
 * @version 0.1
 */

#include "application.h"

#include <cstdlib>
#include <iostream>

#include "Robot.hpp"
#include "Field.hpp"

using namespace rtx;


int main() {
  Vision::init();

  printf("[main()]Starting 2vs2 Teamfootball application.\n");


  // Initialise game objects
  Robot* self = new Robot();
  Field* field = new Field(self);
  printf("[main()]Objects created.\n");

  delete self;
  delete field;
  printf("[main()]Objects destroyed.\n");


  printf("[main()]Camera test...\n");
  //CAM1.init();
  printf("[main()]Camera test done.\n");

  printf("[main()]Exiting application.\n");
  return EXIT_SUCCESS;
}
