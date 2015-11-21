/**
 *  @file Goal.hpp
 *  Goal class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 20 November 2015
 */

#ifndef RTX_ENTITIES_GOAL_H
#define RTX_ENTITIES_GOAL_H

//#include "rtxc.h"
#include "entityConstants.hpp"

#include "Rectangle.hpp"
#include "Point2D.hpp"

#include "entityConstants.hpp"
#include "Entity.hpp"
#include "RelativePosition.hpp"

#include "Color.hpp"


namespace rtx {

  class Goal: public Entity, public Rectangle {
    public:
      Goal(const Goal&);
      /*Goal(const Point2D*, const double& = GOAL_WIDTH,
           const double& = GOAL_LENGTH);
      Goal(const double&, const double&, const double& = GOAL_WIDTH,
           const double& = GOAL_LENGTH);*/
      Goal(const Transform, const Color&, const double& = GOAL_WIDTH, const double& = GOAL_LENGTH);

      Color getColor() const;

      bool isBlue() const;
      bool isYellow() const;

      // TODO: isFriendly(), isEnemy() - needs data about the current game

    private:
      Color color;
  };

}

#endif // RTX_ENTITIES_GOAL_H
