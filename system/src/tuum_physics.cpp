/**
 *  @file tuum_physics.hpp
 *  Physics system interface
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
*/

#include "tuum_physics.hpp"

#include "tuum_visioning.hpp"
#include "Vision.hpp"

namespace rtx { namespace Physics {

  void rayCast(const double &angle) {

    // Check for blobs cutting into the ray
    BallSet balls = *(Visioning::ballDetect().getEntities());
    for (BallSet::iterator ball = balls.begin(); ball != balls.end(); ++ball) {

      unsigned int minX = (*ball)->getBlob()->getMinX();
      unsigned int maxX = (*ball)->getBlob()->getMaxX();
      unsigned int minY = (*ball)->getBlob()->getMinY();
      unsigned int maxY = (*ball)->getBlob()->getMaxY();

    }

    // Check for field lines
    // TODO

  }

  void rayCast(const double &angle, const double &width) {
    // TODO
  }

}}
