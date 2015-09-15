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

class Ball: public Circle {
private:
  Point2D* position;
public:
  Ball(const Ball&);
  Ball(const Point2D*, const double&); // TODO: Add default ball radius
  void setPosition(const Point2D*);
  void setPosition(const double&, const double&);
  Point2D* getPosition() const;
}

#endif // BALL_H
