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
#include "RelativePosition.hpp"

#include "Blob.hpp"


namespace rtx {

  class Goal: public Entity, public Rectangle {
    public:
      Goal(Goal&);
      Goal(const Transform, const Blob*, const double& = GOAL_WIDTH, const double& = GOAL_LENGTH);

      Color getColor() const;

      bool isBlue() const;
      bool isYellow() const;

      // TODO: isFriendly(), isEnemy() - needs data about the current game

    private:
      Blob *blob;
  };

}

#endif // RTX_ENTITIES_GOAL_H
