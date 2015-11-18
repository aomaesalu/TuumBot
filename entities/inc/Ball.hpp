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

  class Ball: public RelativePosition, public Circle {
    public:
      Ball(const Ball&);
      /*Ball(const Point2D*, const double& = BALL_RADIUS);
      Ball(const double&, const double&, const double& = BALL_RADIUS);*/
      Ball(const unsigned int&, const double&, const double& = BALL_RADIUS);

      void update(int, double);
      int decay();
    private:
      int m_health;
  };

};

#endif // RTX_ENTITIES_BALL_H
