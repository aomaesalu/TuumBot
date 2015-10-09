/**
 * @file RobotEnemy.hpp
 * Enemy robot class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_ROBOT_ENEMY_H
#define RTX_ENTITIES_ROBOT_ENEMY_H

#include "entityConstants.hpp"
#include "Robot.h"


namespace rtx {

  class RobotEnemy: public Robot {
    public:
      RobotEnemy(const RobotEnemy&);
      RobotEnemy(const Point2D*, const double& = ROBOT_MAXIMUM_DIAMETER / 2);
      RobotEnemy(const double&, const double&,
                 const double& = ROBOT_MAXIMUM_DIAMETER / 2);
      // TODO: Add size, movement and strategy analysing methods
    private:
      // TODO
  };

};

#endif // RTX_ENTITIES_ROBOT_ENEMY_H
