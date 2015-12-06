/**
 *  @file Goal.hpp
 *  Goal class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 2 December 2015
 */

#ifndef RTX_ENTITIES_GOAL_H
#define RTX_ENTITIES_GOAL_H

#include "entityConstants.hpp"

#include "Rectangle.hpp"
#include "Point2D.hpp"

#include "entityConstants.hpp"
#include "Entity.hpp"

#include "Blob.hpp"


namespace rtx {

  class Goal: public Entity, public Rectangle {
    public:
      Goal(Goal&);
      Goal(const Transform, Vision::Blob*, const double& = GOAL_WIDTH, const double& = GOAL_LENGTH);

      bool isAlly() const;
      bool isOpponent() const;
  };

}

#endif // RTX_ENTITIES_GOAL_H
