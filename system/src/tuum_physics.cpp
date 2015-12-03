/**
 *  @file tuum_physics.hpp
 *  Physics system interface
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
*/

#include "tuum_physics.hpp"

#include <cmath>


namespace rtx { namespace Physics {

  // Returns a pointer to the closest entity that is in the way of the ray. If
  // there seems to be no entity in the way of the ray, a null pointer is
  // returned.
  Entity* rayCast(const double &angle) {

    // Initialise the result to nothing being in the way of the ray
    Entity *result = nullptr;
    double minDistance = 999999;

    // Check for blobs cutting into the ray
    BallSet balls = *(Visioning::ballDetect().getEntities());
    for (BallSet::iterator ball = balls.begin(); ball != balls.end(); ++ball) {

      // If the angle is positive, the ray is located to the left from the
      // center of the camera frame
      if (angle >= 0) {

        // Calculate corresponding blob corner angles
        double bottomLeftAngle = atan2((*ball)->getBlob()->getMinX(),
                                       (*ball)->getBlob()->getMaxY());
        double topRightAngle = atan2((*ball)->getBlob()->getMaxX(),
                                     (*ball)->getBlob()->getMinY());

        // If the angle is smaller than the bottom left corner's angle and
        // larger than the top right corner's angle, the blob is in the way of
        // the ray.
        if (angle <= bottomLeftAngle && angle >= topRightAngle) {

          // TODO: If the blob is closer than the closest object, add it to the
          // result
          //if ((*ball)->)

        }

      // If the angle is negative, the ray is located to the right from the
      // center of the camera frame
      } else {

        // Calculate corresponding blob corner angles
        double topLeftAngle = atan2((*ball)->getBlob()->getMinX(),
                                    (*ball)->getBlob()->getMinY());
        double bottomRightAngle = atan2((*ball)->getBlob()->getMaxX(),
                                        (*ball)->getBlob()->getMaxY());

        // If the angle is smaller than the top left corner's angle and larger
        // than the bottom right corner's angle, the blob is in the way of the
        // ray.
        if (angle <= topLeftAngle && angle >= bottomRightAngle) {

          // TODO: If the blob is closer than the closest object, add it to the
          // result

        }

      }

    }

    // Check for field lines
    // TODO

  }

  Entity* rayCast(const double &angle, const double &width) {
    // TODO
  }

}}
