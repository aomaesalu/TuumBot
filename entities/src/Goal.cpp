/**
 *  @file Goal.cpp
 *  Goal class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 20 November 2015
 */

#include "Goal.hpp"


namespace rtx {

  Goal::Goal(const Goal &other):
    //Entity(other.getPosition()),
    RelativePosition(other.getDistance(), other.getAngle()),
    Rectangle(other.getWidth(), other.getLength()),
    color(other.getColor())
  {

  }

  /*Goal::Goal(const Point2D *position, const double &width,
             const double &length):
    Entity(position),
    Rectangle(width, length)
  {

  }

  Goal::Goal(const double &x, const double &y, const double &width,
             const double &length):
    Entity(new Point2D(x, y)),
    Rectangle(width, length)
  {

  }*/

  Goal::Goal(const unsigned int &distance, const double &angle, const Color &color, const double &width, const double &length):
    RelativePosition(distance, angle),
    Rectangle(width, length),
    color(color)
  {

  }

  Color Goal::getColor() const {
    return color;
  }

  bool Goal::isBlue() const {
    return color == BLUE_GOAL;
  }

  bool Goal::isYellow() const {
    return color == YELLOW_GOAL;
  }

}
