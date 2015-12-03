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
    std::cout << "(" << radius << ", " << angle << "), (" << slope << ", " << perpendicularSlope << "), (" << radiusVectorX << ", " << radiusVectorY << ")" << std::endl;
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

      // DEBUG:
      std::cout << "Entity: " << std::endl;
      std::pair<double, double> pos = Vision::Perspective::virtualToReal((*entity)->getBlob()->getPosition());
      double dis = sqrt(pos.second * pos.second + pos.first * pos.first);
      std::cout << intToColor((*entity)->getBlob()->getColor()) << " " << dis << " " << pos.first << " " << pos.second << std::endl;
      if ((*entity)->isBall()) {
        if (((Ball*) *entity)->isValid()) {
          std::cout << "  The ball is valid." << std::endl;
        } else {
          std::cout << "  The ball is not valid." << std::endl;
        }
      }

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

      // If the blob is farther away than the closest object, continue with the
      // next blob
      if (distance >= minDistance)
        continue;

      // If the angle is positive, the ray is located to the left from the
      // center of the camera frame
      if (angle >= 0) {

        // Calculate corresponding blob corner angles
        std::pair<double, double> bottomLeftCorner = Vision::Perspective::virtualToReal((*entity)->getBlob()->getMinX() - radiusVectorX, (*entity)->getBlob()->getMaxY() - radiusVectorY);
        double bottomLeftAngle = -atan2(bottomLeftCorner.first, bottomLeftCorner.second);
        std::pair<double, double> topRightCorner = Vision::Perspective::virtualToReal((*entity)->getBlob()->getMaxX() + radiusVectorX, (*entity)->getBlob()->getMinY() + radiusVectorY);
        double topRightAngle = -atan2(topRightCorner.first, topRightCorner.second);

        // DEBUG:
        std::cout << (*entity)->getBlob()->getPosition()->getX() << " " << (*entity)->getBlob()->getPosition()->getY() << " " << -atan2((*entity)->getBlob()->getPosition()->getX(), (*entity)->getBlob()->getPosition()->getY()) << std::endl;
        std::cout << (*entity)->getBlob()->getMinX() << " " << (*entity)->getBlob()->getMaxY() << " " << -atan2((*entity)->getBlob()->getMinX(), (*entity)->getBlob()->getMaxY()) << std::endl;
        std::cout << (*entity)->getBlob()->getMaxX() << " " << (*entity)->getBlob()->getMinY() << " " << -atan2((*entity)->getBlob()->getMaxX(), (*entity)->getBlob()->getMinY()) << std::endl;
        std::cout << "(" << bottomLeftAngle << ", " << topRightAngle << ")" << std::endl;

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
        std::pair<double, double> topLeftCorner = Vision::Perspective::virtualToReal((*entity)->getBlob()->getMinX() - radiusVectorX, (*entity)->getBlob()->getMinY() - radiusVectorY);
        double topLeftAngle = -atan2(topLeftCorner.first, topLeftCorner.second);
        std::pair<double, double> bottomRightCorner = Vision::Perspective::virtualToReal((*entity)->getBlob()->getMaxX() + radiusVectorX, (*entity)->getBlob()->getMaxY() + radiusVectorY);
        double bottomRightAngle = -atan2(bottomRightCorner.first, bottomRightCorner.second);

        // DEBUG:
        std::cout << "(" << topLeftAngle << ", " << bottomRightAngle << ")" << std::endl;

        // If the angle is smaller than the top left corner's angle and larger
        // than the bottom right corner's angle, the blob is in the way of the
        // ray.
        if (angle <= topLeftAngle && angle >= bottomRightAngle) {

          // Change the result to the entity
          closestEntity = *entity;

        }

      }

      // DEBUG:
      std::cout << std::endl;

    }

    // Check for field lines in the way of the ray. What to do with the result
    // value in this case?
    // TODO

    return closestEntity;

  }

}}
