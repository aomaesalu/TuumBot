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

 * 2-dimensional shape class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef SHAPE2D_H
#define SHAPE2D_H

class Shape2D {
public:
  virtual double getArea() const = 0;
  virtual double getPerimeter() const = 0;
};

#endif // SHAPE2D_H
