/**
 * @file Shape2D.hpp
 * Description placeholder.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_GEOMETRY_SHAPE2D_H
#define RTX_GEOMETRY_SHAPE2D_H


namespace rtx {

  class Shape2D {
    public:
      virtual double getArea() const = 0;
      virtual double getPerimeter() const = 0;
  };

};

#endif // RTX_GEOMETRY_SHAPE2D_H
