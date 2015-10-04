/**
 * @file Transform.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#include "Goal.hpp"


namespace rtx {

  Goal::Goal(const Goal& other):
    Entity(other.getPosition()),
    Rectangle(other.getWidth(), other.getLength())
  {

  }

  Goal::Goal(const Point2D* position, const double& width, const double& length):
    Entity(position),
    Rectangle(width, length)
  {

  }

  Goal::Goal(const double& x, const double& y, const double& width, const double& length):
    Entity(new Point2D(x, y)),
    Rectangle(width, length)
  {

  }

};
