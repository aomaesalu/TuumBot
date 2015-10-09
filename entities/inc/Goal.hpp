/**
 * @file Goal.hpp
 * Goal class.
 *
 * @authors
 * @version 0.1
 */

#ifndef RTX_ENTITIES_GOAL_H
#define RTX_ENTITIES_GOAL_H

#include "Rectangle.hpp"
#include "Point2D.hpp"

#include "entityConstants.hpp"
#include "Entity.hpp"


namespace rtx {

  class Goal: public Entity, public Rectangle {
    public:
      Goal(const Goal&);
      Goal(const Point2D*, const double& = GOAL_WIDTH,
           const double& = GOAL_LENGTH);
      Goal(const double&, const double&, const double& = GOAL_WIDTH,
           const double& = GOAL_LENGTH);

    private:
      // TODO: Add color
  };

};

#endif // RTX_ENTITIES_GOAL_H
