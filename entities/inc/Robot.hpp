/**
 *  @file Robot.hpp
 *  Robot class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 26 November 2015
 */

#ifndef RTX_ENTITIES_ROBOT_H
#define RTX_ENTITIES_ROBOT_H

#include "Circle.hpp"
#include "Point2D.hpp"

#include "entityConstants.hpp"
#include "Entity.hpp"
#include "Color.hpp"


namespace rtx {

  class Robot: public Entity, public Circle {
    public:
      Robot();
      Robot(const Robot&);

      Robot(const Transform, const Blob*, const double& = ROBOT_MAXIMUM_DIAMETER / 2);

      bool isAlly() const;
      bool isOpponent() const;

  };

}

#endif // RTX_ENTITIES_ROBOT_H
