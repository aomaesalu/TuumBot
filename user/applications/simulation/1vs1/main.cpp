/**
 * @file main.cpp
 * Football 1vs1 simulation application main module.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <cstdlib>

#include "RobotSelf.hpp"
#include "Field1vs1.hpp"

using namespace rtx;


int main() {

	// Initialise game objects
	RobotSelf* self = new RobotSelf();
	Field1vs1* field = new Field1vs1(self);

	// TODO

	return EXIT_SUCCESS;
}
