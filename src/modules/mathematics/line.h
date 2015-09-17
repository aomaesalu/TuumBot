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

 * Line class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef LINE_H
#define LINE_H

#include "point2d.h"

class Line {
private:
  Point2D* pointA;
  Point2D* pointB;
public:
  Line(const Line&);
  Line(const Point2D*, const Point2D*);
  Line(const double&, const double&, const double&, const double&);
  void setPointA(const Point2D*);
  void setPointA(const double&, const double&);
  void setPointB(const Point2D*);
  void setPointB(const double&, const double&);
  double getLength() const;
  Point2D* getPointA() const;
  Point2D* getPointB() const;
};

#endif // LINE_H
