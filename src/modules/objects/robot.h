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

 * Robot class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "object.h"
#include "../mathematics/circle.h"
#include "../mathematics/point2d.h"

class Robot: public Object, public Circle {
private:
public:
  Robot(const Robot&);
  Robot(const Point2D*, const double&); // TODO: Add default maximum robot radius; can be changed midgame
  Robot(const double&, const double&, const double&); // TODO: Add default maximum robot radius; can be changed midgame
};

#endif // ROBOT_H
