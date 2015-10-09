/**
 * @file Goal.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef GOAL_H
#define GOAL_H

#include "constants.h"

#include "Rectangle.hpp"
#include "Point2D.hpp"
#include "Entity.hpp"


namespace rtx {

  class Goal: public Entity, public Rectangle {
    public:
      Goal(const Goal&);
      Goal(const Point2D*, const double& = GOAL_WIDTH, const double& = GOAL_LENGTH);
      Goal(const double&, const double&, const double& = GOAL_WIDTH, const double& = GOAL_LENGTH);
    
    private:
      // TODO: Add color
  };

};

#endif // GOAL_H
