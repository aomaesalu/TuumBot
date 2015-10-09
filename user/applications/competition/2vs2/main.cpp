/**
 * @file main.cpp
 * Football 2vs2 application main module.
 *
 * @authors Ants-Oskar Mäesalu
 * @authors Meelik Kiik
 * @version 0.1
 */

#include "application.h"

#include <cstdlib>
#include <cstdio>

#include "RobotSelf.hpp"
#include "Field2vs2.hpp"

using namespace rtx;


int main() {
  //Vision::init();

  printf("[main()]Starting 2vs2 Teamfootball application.\n");

  // Initialise game objects
	RobotSelf* self = new RobotSelf();
	Field2vs2* field = new Field2vs2(self);
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
