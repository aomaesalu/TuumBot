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

  Entity::Entity(const Vec2i pos):
    m_transform(pos)
  {

  }

  Entity::Entity(const int x, const int y):
    m_transform(x, y)
  {

  }

  Entity::Entity(Transform transform) {
    m_transform = transform;
  }

  Entity::Entity(const int x, const int y, const double o):
    m_transform(x, y, o)
  {

  }

  int Entity::getHealth() {
    return m_health;
  }

  Transform* Entity::getTransform() {
    return &m_transform;
  }

  void Entity::update(Transform transform) {
    int x = m_transform.getX()*0.2 + transform.getX()*0.8;
    int y = m_transform.getY()*0.2 + transform.getY()*0.8;
    m_transform.setPosition(x, y);

    if(m_health < 10) m_health += 2;
  }

  void Entity::update() {
    m_health--;
  }

};
