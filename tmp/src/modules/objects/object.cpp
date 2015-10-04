/**
 * Tuum
 * Robotex 2015
 * University of Tartu
 * Team:
 *   Kristjan Kanarbik
 *   Meelik Kiik
 *   Ants-Oskar Mäesalu
 *   Mart Müllerbeck
 *   Kaur Viiklaid
 *   Indrek Virro
 *   Mihkel Väljaots

 * Object class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "object.h"

Object::Object(const Object& other):
position{new Point2D(other.getPosition()->getX(), other.getPosition()->getY())}
{}

Object::Object(const Point2D* position):
position{new Point2D(position->getX(), position->getY())}
{}

Object::Object(const double& x, const double& y):
position{new Point2D(x, y)}
{}

void Object::setPosition(const Point2D* position) {
  this->position = new Point2D(position->getX(), position->getY());
}

void Object::setPosition(const double& x, const double& y) {
  this->position = new Point2D(x, y);
}

Point2D* Object::getPosition() const {
  return position;
}
