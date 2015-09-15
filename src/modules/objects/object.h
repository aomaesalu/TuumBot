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

 * Object class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "../mathematics/shape2d.h"
#include "../mathematics/point2d.h"

class Object: public Shape2D {
private:
  Point2D* position;
public:
  Object(const Object&);
  Object(const Point2D*);
  Object(const double&, const double&);
  void setPosition(const Point2D*);
  void setPosition(const double&, const double&);
  Point2D* getPosition() const;
}

#endif // OBJECT_H
