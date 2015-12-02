/** @file Entity.cpp
 *  Entity class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @authors Meelik Kiik
 *  @version 0.3
 *  @date 2 December 2015
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
    blob = nullptr;
  }

  Entity::Entity(const Entity& entity) {
    (*this) = entity;
    id = Entity::newID();
  }

  Entity::Entity(const Vec2i pos, Blob *blob):
    m_transform(pos)
  {
    id = Entity::newID();
    this->blob = blob;
  }

  Entity::Entity(const int x, const int y, Blob *blob):
    m_transform(x, y)
  {
    id = Entity::newID();
    this->blob = blob;
  }

  Entity::Entity(Transform transform, Blob *blob) {
    id = Entity::newID();
    m_transform = transform;
    this->blob = blob;
  }

  Entity::Entity(Transform transform, Blob &blob) {
    id = Entity::newID();
    m_transform = transform;
    this->blob = &blob;
  }

  Entity::Entity(Transform transform, bool recv_id, Blob *blob) {
    if(recv_id) id = Entity::newID();
    else id = 0;
    m_transform = transform;
    this->blob = blob;
  }

  Entity::Entity(const int x, const int y, const double o, Blob *blob):
    m_transform(x, y, o)
  {
    id = Entity::newID();
    this->blob = blob;
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

    if(m_health < 35) m_health += 2;
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

  Blob* Entity::getBlob() const {
    return blob;
  }

  Color Entity::getColor() const {
    return blob->getColor();
  }

}
