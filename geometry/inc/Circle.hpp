/**
 * @file Circle.hpp
 * Circle class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_GEOMETRY_CIRCLE_H
#define RTX_GEOMETRY_CIRCLE_H

#include "Shape2D.hpp"


namespace rtx {

  class Circle: public Shape2D {
  private:
    double radius;

  public:
    Circle();
    Circle(const Circle&);
    Circle(const double&);

    void setRadius(const double&);

    double getArea() const;
    double getPerimeter() const;
    double getRadius() const;
  };

};

#endif // RTX_GEOMETRY_CIRCLE_H
