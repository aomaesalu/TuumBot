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
#include <vector>


namespace rtx { namespace Physics {

  // Returns a pointer to the closest entity that is in the way of the ray. If
  // there seems to be no entity in the way of the ray, a null pointer is
  // returned.
  Entity* rayCast(const double &angle, const double &width) {

    // Initialise the result to nothing being in the way of the ray
    Entity *closestEntity = nullptr;
    double minDistance = 999999;

    // Calculate ray radius
    double radius = width / 2;

    // Calculate the ray slope
    double slope = tan(angle + M_PI);
    double perpendicularSlope = -1 / slope;

    // Initialise the ray radius vector (to the right). This can be used to find
    // blob containment checking points.
    double radiusVectorX = sqrt(radius * radius / (perpendicularSlope * perpendicularSlope + 1));
    double radiusVectorY = perpendicularSlope * radiusVectorX;

    // Initialise a list of entities to check
    std::vector<Entity*> entities;

    // Add balls to the entities list
    Visioning::BallSet balls = *(Visioning::ballDetect.getEntities());
    entities.insert(entities.end(), balls.begin(), balls.end());

    // Add goals to the entities list if they exist
    if (Visioning::yellowGoal != nullptr)
      entities.push_back(Visioning::yellowGoal);
    if (Visioning::blueGoal != nullptr)
      entities.push_back(Visioning::blueGoal);

    // Add robots to the entities list
    Visioning::RobotSet robots = *(Visioning::robotDetect.getEntities());
    entities.insert(entities.end(), robots.begin(), robots.end());

    // DEBUG:
    std::cout << "Ray: " << std::endl;
    std::cout << radius << " " << slope << " " << perpendicularSlope << " " << radiusVectorX << " " << radiusVectorY << std::endl;
    std::cout << std::endl;

    // Check for entity blobs overlapping the ray area
    for (std::vector<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity) {

      /*// DEBUG:
      if (*entity == nullptr)
        std::cout << "Entity is null" << std::endl;
      else
        std::cout << "Entity is not null" << std::endl;

      // DEBUG:
      if ((*entity)->getBlob() == nullptr)
        std::cout << "Entity blob is null" << std::endl;
      else
        std::cout << "Entity blob is not null" << std::endl;*/

      // If the entity is a ball, ensure that it is valid. Otherwise, continue
      // with the next entity.
      if ((*entity)->isBall()) {
        if (((Ball*) *entity)->isNotValid()) {
          continue;
        }
      }

      // Calculate blob relative position
      std::pair<double, double> position = Vision::Perspective::virtualToReal((*entity)->getBlob()->getPosition());
      double distance = sqrt(position.second * position.second + position.first * position.first);

      // DEBUG:
      std::cout << "Entity: " << std::endl;
      std::cout << (*entity)->getBlob()->getColor() << " " << distance << " " << position.first << " " << position.second << std::endl;
      std::cout << std::endl;

      // If the blob is farther away than the closest object, continue with the
      // next blob
      if (distance >= minDistance)
        continue;

      // If the angle is positive, the ray is located to the left from the
      // center of the camera frame
      if (angle >= 0) {

        // Calculate corresponding blob corner angles
        double bottomLeftAngle = -atan2((*entity)->getBlob()->getMinX() - radiusVectorX, (*entity)->getBlob()->getMaxY() - radiusVectorY);
        double topRightAngle = -atan2((*entity)->getBlob()->getMaxX() + radiusVectorX, (*entity)->getBlob()->getMinY() + radiusVectorY);

        // If the angle is smaller than the bottom left corner's angle and
        // larger than the top right corner's angle, the blob is in the way of
        // the ray.
        if (angle <= bottomLeftAngle && angle >= topRightAngle) {

          // Change the result to the entity
          closestEntity = *entity;

        }

      // If the angle is negative, the ray is located to the right from the
      // center of the camera frame
      } else {

        // Calculate corresponding blob corner angles
        double topLeftAngle = -atan2((*entity)->getBlob()->getMinX() - radiusVectorX, (*entity)->getBlob()->getMinY() - radiusVectorY);
        double bottomRightAngle = -atan2((*entity)->getBlob()->getMaxX() + radiusVectorX, (*entity)->getBlob()->getMaxY() + radiusVectorY);

        // If the angle is smaller than the top left corner's angle and larger
        // than the bottom right corner's angle, the blob is in the way of the
        // ray.
        if (angle <= topLeftAngle && angle >= bottomRightAngle) {

          // Change the result to the entity
          closestEntity = *entity;

        }

      }

    }

    // Check for field lines in the way of the ray. What to do with the result
    // value in this case?
    // TODO

    return closestEntity;

  }

}}
