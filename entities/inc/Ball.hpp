/**
 * @file Ball.hpp
 * Ball class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef BALL_H
#define BALL_H


#include "entityConstants.h"
#include "Entity.h"
#include "Circle.h"
#include "Point2D.h"

namespace rtx {

  class Ball: public Entity, public Circle {
    public:
      Ball(const Ball&);
      Ball(const Point2D*, const double& = BALL_RADIUS);
      Ball(const double&, const double&, const double& = BALL_RADIUS);

    private:
      // TODO
  }

};

#endif // BALL_H
