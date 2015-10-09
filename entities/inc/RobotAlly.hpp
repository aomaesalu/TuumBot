/**
 * @file RobotAlly.hpp
 * Robot ally (friendly robot) class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_ROBOT_ALLY_H
#define RTX_ENTITIES_ROBOT_ALLY_H

#include "entityConstants.hpp"
#include "Robot.hpp"


namespace rtx {

  class RobotAlly: public Robot {
    public:
      RobotAlly(const RobotAlly&);
      RobotAlly(const Point2D*, const double& = ROBOT_DIAMETER / 2);
      RobotAlly(const double& = ROBOT_INITIAL_POSITION_X,
                const double& = ROBOT_INITIAL_POSITION_Y,
                const double& = ROBOT_DIAMETER / 2);
      // TODO: Add communication and strategy methods

    private:
      // TODO
  };

};

#endif // RTX_ENTITIES_ROBOT_ALLY_H
