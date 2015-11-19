/**
 * @file Ball.hpp
 * Ball class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_BALL_H
#define RTX_ENTITIES_BALL_H

#include "Circle.hpp"
#include "Point2D.hpp"

#include "entityConstants.hpp"
#include "Entity.hpp"
#include "RelativePosition.hpp"


namespace rtx {

  class Ball: public Entity, public Circle {
  public:
    Ball(Ball&);
    Ball(const Transform, const double& = BALL_RADIUS);
  };

};

#endif // RTX_ENTITIES_BALL_H
