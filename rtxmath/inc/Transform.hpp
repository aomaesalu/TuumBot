/** @file Transform.cpp
 *  Object positional transform state class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 19. November 2015
 */

#ifndef RTX_TRANSFORM_H
#define RTX_TRANSFORM_H

#include "__future__.hpp"

namespace rtx {

  struct Transform {
    Vec2i pos;
    double o;

    Transform();

    Transform(const int, const int);
    Transform(const Vec2i*);

    Transform(const int, const int, const double);
    Transform(const Vec2i*, const double);

    void setPosition(const Vec2i);
    Vec2i getPosition();

    int getX();
    int getY();
  };

}

#endif // RTX_TRANSFORM_H
