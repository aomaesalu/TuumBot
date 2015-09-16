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

 * Ball class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef BALL_H
#define BALL_H

#include "../mathematics/circle.h"
#include "../mathematics/point2d.h"

class Ball: public Object, public Circle {
private:

public:
  Ball(const Ball&);
  Ball(const Point2D*, const double&); // TODO: Add default ball radius
  Ball(const double&, const double&, const double&);
};

#endif // BALL_H
