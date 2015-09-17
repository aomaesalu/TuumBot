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

 * Rectangle class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape2d.h"

class Rectangle: public Shape2D {
private:
  double width;
  double length;
public:
  Rectangle(const Rectangle&);
  Rectangle(const double&, const double&);
  void setWidth(const double&);
  void setLength(const double&);
  double getArea() const;
  double getPerimeter() const;
  double getWidth() const;
  double getLength() const;
};

#endif // RECTANGLE_H
