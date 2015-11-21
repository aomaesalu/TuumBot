/** @file Entity.cpp
 *  Entity class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 */

#include <iostream>
#include <sstream>
#include <string>

#include "Entity.hpp"

namespace rtx {

  unsigned int Entity::id_seq = 0;

  unsigned int Entity::newID() {
    return ++id_seq;
  }

  Entity::Entity() {
    id = Entity::newID();
  }

  Entity::Entity(const Entity& entity) {
    (*this) = entity;
    id = Entity::newID();
  }

  Entity::Entity(const Vec2i pos):
    m_transform(pos)
  {
    id = Entity::newID();
  }

  Entity::Entity(const int x, const int y):
    m_transform(x, y)
  {
    id = Entity::newID();
  }

  Entity::Entity(Transform transform) {
    id = Entity::newID();
    m_transform = transform;
  }

  Entity::Entity(const int x, const int y, const double o):
    m_transform(x, y, o)
  {
    id = Entity::newID();
  }

  unsigned int Entity::getID() {
    return id;
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

    if(m_health < 30) m_health += 2;
  }

  void Entity::update() {
    m_health--;
  }

  std::string Entity::toString() {
    std::stringstream output;
    output << "<Entity #"
           << getID()
           << ", hp="
           << getHealth()
	   << ", x=" << m_transform.getX()
	   << ", y=" << m_transform.getY()
	   << ">";
    return output.str();
  }

};
