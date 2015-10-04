/**
 * @file Shape2D.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef SHAPE2D_H
#define SHAPE2D_H

namespace rtx {

  class Shape2D {
    public:
      virtual double getArea() const = 0;
      virtual double getPerimeter() const = 0;
  };

};

#endif // SHAPE2D_H
