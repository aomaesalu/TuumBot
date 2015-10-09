/**
 * @file main.cpp
 * Football 2vs2 simulation application main module.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <cstdlib>

#include "RobotSelf.hpp"
#include "Field2vs2.hpp"

using namespace rtx;


int main() {

  // Initialise game objects
	RobotSelf* self = new RobotSelf();
	Field2vs2* field = new Field2vs2(self);

  // TODO

  return EXIT_SUCCESS;
}
