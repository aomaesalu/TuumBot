/**
 * @file Circle.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape2D.hpp"


namespace rtx {

  class Circle: public Shape2D {
    private:
      double radius;

    public:
      Circle(const Circle&);
      Circle(const double&);

      void setRadius(const double&);

      double getArea() const;
      double getPerimeter() const;
      double getRadius() const;
  };

};

#endif // CIRCLE_H
