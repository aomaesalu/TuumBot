/**
 * @file Entity.cpp
 * Description placeholder.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Entity.hpp"


namespace rtx {

  Entity::Entity(const Entity &other):
    position{new Point2D(other.getPosition()->getX(), other.getPosition()->getY())}
  {

  }

  Entity::Entity(const Point2D *position):
    position{new Point2D(position->getX(), position->getY())}
  {

  }

  Entity::Entity(const double &x, const double &y):
    position{new Point2D(x, y)}
  {

  }

  void Entity::setPosition(const Point2D *position) {
    this->position = new Point2D(position->getX(), position->getY());
  }

  void Entity::setPosition(const double &x, const double &y) {
    this->position = new Point2D(x, y);
  }

  Point2D* Entity::getPosition() const {
    return position;
  }

};
