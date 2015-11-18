/**
 * @file Ball.cpp
 * Ball class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Ball.hpp"


namespace rtx {

  Ball::Ball(const Ball &other):
    RelativePosition(other.getDistance(), other.getAngle()),
    //Entity(other.getPosition()),
    Circle(other.getRadius())
  {
    m_health = 0;
  }

  /*Ball::Ball(const Point2D *position, const double &radius):
    //Entity(position),
    Circle(radius)
  {

  }*/

  /*Ball::Ball(const double &x, const double &y, const double &radius):
    //Entity(new Point2D(x, y)),
    Circle(radius)
  {

  }*/

  Ball::Ball(const unsigned int &distance, const double &angle, const double &radius):
    RelativePosition(distance, angle),
    Circle(radius)
  {
    m_health = 0;
  }

  void Ball::update(int d, double a) {
    this->distance = this->distance*0.2 + d*0.8;
    this->angle = this->angle*0.1 + a*0.9;

    if(m_health < 50) m_health += 2;
  }

  int Ball::decay() {
    return --m_health;
  }

};
