/**
 * @file RobotSelf.hpp
 * Robot self class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_ROBOT_SELF_H
#define RTX_ENTITIES_ROBOT_SELF_H

#include "entityConstants.hpp"
#include "Robot.h"


namespace rtx {

  class RobotSelf: public Robot {
    public:
      RobotSelf(const RobotSelf&);
      RobotSelf(const Point2D*, const double& = ROBOT_DIAMETER / 2);
      // TODO: Add motors, coilgun, tribbler, cameras parameters
      RobotSelf(const double& = ROBOT_INITIAL_POSITION_X,
                const double& = ROBOT_INITIAL_POSITION_Y,
                const double& = ROBOT_DIAMETER / 2);
                // TODO: Add motors, coilgun, tribbler, cameras parameters
      // TODO: Add communication and strategy methods
      // TODO: Add movement methods
      // TODO: Add computer vision methods

    private:
      // TODO: Add motors, coilgun, tribbler, cameras, ...
  };

};

#endif // RTX_ENTITIES_ROBOT_SELF_H
