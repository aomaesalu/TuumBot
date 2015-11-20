/** @file tuum_navigation.cpp
 *  Navigation system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include "tuum_navigation.hpp"
#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"

namespace rtx { namespace Navigation {

  const int NAV_BALL_PICKUP_RANGE = 75;


  //TODO: position to relative position
  Transform calcBallPickupPos(Transform* bt) {
    Transform target((*bt) - NAV_BALL_PICKUP_RANGE);

    double o = bt->getPosition().getOrientation();
    target.setOrientation(o);
    return target;
  }

  Ball* getNearestBall() {
    Ball* ball = nullptr;
    Transform* t = Localization::getTransform();

    double d = 0.0, _d;
    for(auto b : *Visioning::ballDetect.getEntities()) {
      _d = t->distanceTo(b->getTransform()->getPosition());

      if(d < _d) {
	d = _d;
	ball = b;
      }
    }

    return ball;
  }

}}
