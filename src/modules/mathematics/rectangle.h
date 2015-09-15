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

class Rectangle {
private:
  double x;
  double y;
public:
  Rectangle(const Rectangle&);
  Rectangle(const double&, const double&);
  void setX(const double&);
  void setY(const double&);
  double getArea() const;
  double getX() const;
  double getY() const;
};

#endif // RECTANGLE_H
