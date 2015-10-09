/**
 * @file Robot.hpp
 * Robot class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Circle.hpp"
#include "Point2D.hpp"

#include "Entity.hpp"


namespace rtx {

  class Robot : public Entity, public Circle {
    public:
      Robot();
      Robot(const Robot&);
      Robot(const Point2D*, const double&);
      // TODO: Add default maximum robot radius; can be changed midgame
      Robot(const double&, const double&, const double&);
      // TODO: Add default maximum robot radius; can be changed midgame

    private:
      // TODO
  };

};

#endif // ROBOT_H
