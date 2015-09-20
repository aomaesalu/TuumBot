/**
 * Tuum
 * Robotex 2015
 * University of Tartu
 * Team:
 *   Kristjan Kanarbik
 *   Meelik Kiik
 *   Ants-Oskar Mäesalu
 *   Mart Müllerbeck
 *   Kaur Viiklaid
 *   Indrek Virro
 *   Mihkel Väljaots

 * 1vs1 competition program
 * Contributors:
 *   Ants-Oskar Mäesalu

 * main.cpp
 * The main function of the competition program.
 */

#include <cstdlib>
#include "../../../modules/objects/robotSelf.h"
#include "../../../modules/objects/field1vs1.h"

int main() {
	// Initialise game objects
	RobotSelf* self = new RobotSelf();
	Field1vs1* field = new Field1vs1(self);
	return EXIT_SUCCESS;
}
