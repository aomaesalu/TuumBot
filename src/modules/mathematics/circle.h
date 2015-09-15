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

 * Circle class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape2d.h"

class Circle: public Shape2D {
private:
  double radius;
public:
  Circle(const Circle&);
  Circle(const double&);
  void setRadius(const double&);
  double getRadius() const;
};

#endif // CIRCLE_H
