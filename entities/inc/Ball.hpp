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


namespace rtx {

  class Ball: public Entity, public Circle {
    public:
      Ball(const Ball&);
      Ball(const Point2D*, const double& = BALL_RADIUS);
      Ball(const double&, const double&, const double& = BALL_RADIUS);

    private:
      // TODO
  };

};

#endif // RTX_ENTITIES_BALL_H
