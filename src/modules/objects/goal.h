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

 * Goal class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef GOAL_H
#define GOAL_H

#include "../mathematics/rectangle.h"
#include "../mathematics/point2d.h"

class Goal: public Object, public Rectangle {
private:
  // TODO: Add color
public:
  Goal(const Goal&);
  Goal(const Point2D*, const double&, const double&); // TODO: Add default goal dimensions
  Goal(const double&, const double&, const double&, const double&); // TODO: Add default goal dimensions
}

#endif // GOAL_H
