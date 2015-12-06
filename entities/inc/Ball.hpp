/**
 *  @file Ball.hpp
 *  Ball class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 2 December 2015
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
      Ball(Ball&);
      Ball(const Transform, Vision::Blob*, const double& = BALL_DIAMETER / 2.0);

      void initialiseAttributes();

      bool isInGoal() const;
      bool isOutOfGoal() const;
      bool isInBounds() const;
      bool isOutOfBounds() const;
      bool isKicked() const;
      bool isNotKicked() const;
      bool isValid() const;
      bool isNotValid() const;

      void setKicked(const bool& = true);
      void setInGoal(const bool& = true);
      void setInBounds(const bool& = true);

    private:
      bool kicked;
      bool inGoal;
      bool inBounds;

  };

}

#endif // RTX_ENTITIES_BALL_H
