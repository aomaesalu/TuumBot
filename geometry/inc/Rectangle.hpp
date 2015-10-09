/**
 * @file Rectangle.hpp
 * Description placeholder.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape2D.hpp"


namespace rtx {

  class Rectangle: public Shape2D {
    public:
      Rectangle(const Rectangle&);
      Rectangle(const double&, const double&);

      void setWidth(const double&);
      void setLength(const double&);

      double getArea() const;
      double getPerimeter() const;
      double getWidth() const;
      double getLength() const;

    private:
      double width;
      double length;
  };

};

#endif // RECTANGLE_H
