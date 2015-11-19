/** @file Entity.cpp
 *  Entity class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 */

#include "Entity.hpp"

namespace rtx {

  Entity::Entity() {

  }

  Entity::Entity(const Entity& entity) {
    (*this) = entity;
  }

  Entity::Entity(const Vec2i* pos):
    m_transform(pos)
  {

  }

  Entity::Entity(const int x, const int y):
    m_transform(x, y)
  {

  }

  Entity::Entity(const Transform transform) {
    m_transform = transform;
  }

  Entity::Entity(const int x, const int y, const double o):
    m_transform(x, y, o)
  {

  }

  Transform* Entity::getTransform() {
    return &m_transform;
  }

};
