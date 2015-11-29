/**
 *  @file Ball.hpp
 *  Ball class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
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
    Ball(const Transform, const double& = BALL_DIAMETER / 2.0);
  };

};

#endif // RTX_ENTITIES_BALL_H
