/**
 *  @file Blob.cpp
 *  Blob seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.3
 *  @date 4 December 2015
 */

#include "cameraConstants.hpp"
#include "entityConstants.hpp"

#include "Blob.hpp"

#include <algorithm>
#include <iostream> // TODO: Remove
#include <utility>


namespace rtx {

  Blob::Blob(const Blob &other):
    centroid{new Point2D(*(other.getCentroid()))},
    minX{other.getMinX()},
    maxX{other.getMaxX()},
    minY{other.getMinY()},
    maxY{other.getMaxY()},
    numberOfPoints{other.getNumberOfPoints()},
    color{other.getColor()}
  {
    // Nothing to do here
  }

  Blob::Blob(const std::vector<Point2D*> &points, const Color &color) {
    // TODO: Add points
    this->color = color;
    minX = CAMERA_WIDTH - 1, minY = CAMERA_HEIGHT - 1;
    maxX = 0, maxY = 0;
    unsigned int xSum = 0, ySum = 0;
    numberOfPoints = 0;
    for (std::vector<Point2D*>::const_iterator i = points.begin(); i != points.end(); ++i) {
      numberOfPoints++;
      xSum += (*i)->getX();
      ySum += (*i)->getY();
      if ((*i)->getX() < minX) {
        minX = (*i)->getX();
      }
      if ((*i)->getX() > maxX) {
        maxX = (*i)->getX();
      }
      if ((*i)->getY() < minY) {
        minY = (*i)->getY();
      }
      if ((*i)->getY() > maxY) {
        maxY = (*i)->getY();
      }
    }
    centroid = new Point2D(xSum / numberOfPoints, ySum / numberOfPoints);
  }

  Blob::Blob(const std::vector<std::pair<unsigned int, unsigned int>> &points, const Color &color) {
    this->points = points;
    this->color = color;
    minX = CAMERA_WIDTH - 1, minY = CAMERA_HEIGHT - 1;
    maxX = 0, maxY = 0;
    unsigned int xSum = 0, ySum = 0;
    numberOfPoints = 0;
    for (std::vector<std::pair<unsigned int, unsigned int>>::const_iterator i = points.begin(); i != points.end(); ++i) {
      numberOfPoints++;
      xSum += i->first;
      ySum += i->second;
      if (i->first < minX) {
        minX = i->first;
      }
      if (i->first > maxX) {
        maxX = i->first;
      }
      if (i->second < minY) {
        minY = i->second;
      }
      if (i->second > maxY) {
        maxY = i->second;
      }
    }
    centroid = new Point2D(xSum / numberOfPoints, ySum / numberOfPoints);
  }

  Blob::~Blob() {
    // TODO
  }

  const std::vector<std::pair<unsigned int, unsigned int>>& Blob::getPoints() const {
    return points;
  }

  Point2D* Blob::getCentroid() const {
    return centroid;
  }

  Point2D* Blob::getMidPoint() const {
    return new Point2D((minX + maxX) / 2, (minY + maxY) / 2);
  }

  Point2D* Blob::getPosition() const {
    return new Point2D(centroid->getX(), maxY);
  }

  unsigned int Blob::getWidth() const {
    return maxX - minX + 1;
  }

  unsigned int Blob::getHeight() const {
    return maxY - minY + 1;
  }

  unsigned int Blob::getMinX() const {
    return minX;
  }

  unsigned int Blob::getMaxX() const {
    return maxX;
  }

  unsigned int Blob::getMinY() const {
    return minY;
  }

  unsigned int Blob::getMaxY() const {
    return maxY;
  }

  unsigned int Blob::getNumberOfPoints() const {
    return numberOfPoints;
  }

  Color Blob::getColor() const {
    return color;
  }

  unsigned int Blob::getBoxArea() const {
    return getWidth() * getHeight();
  }

  double Blob::getBoxRatio() const {
    return getWidth() / (double)getHeight();
  }

  double Blob::getDensity() const {
    return 1.0 * numberOfPoints / getBoxArea();
  }

  std::pair<unsigned int, unsigned int> Blob::getExpectedVirtualSize() const {
    return getBlobExpectedVirtualSize(color, std::pair<unsigned int, unsigned int>(centroid->getX(), getMaxY()));
  }

  bool Blob::isOrange() const {
    return color == BALL;
  }

  bool Blob::isBlue() const {
    return color == BLUE_GOAL;
  }

  bool Blob::isYellow() const {
    return color == YELLOW_GOAL;
  }

  bool Blob::isYellowBlue() const {
    return color == ROBOT_YELLOW_BLUE;
  }

  bool Blob::isBlueYellow() const {
    return color == ROBOT_BLUE_YELLOW;
  }

  bool Blob::isFullyVisible() const { // TODO: Refactor
    // Currently only checks the horisontal coordinates of the box area
    return minX >= 0.03 * CAMERA_WIDTH && maxX <= 0.97 * CAMERA_WIDTH;
  }

  bool Blob::isSameColor(const Blob &other) const {
    return color == other.getColor();
  }

  bool Blob::isAbove(const Blob &other) const {
    return centroid->getY() < other.getCentroid()->getY();
  }

  bool Blob::isBelow(const Blob &other) const {
    return !isAbove(other);
  }

  bool Blob::isIn(const Blob &other) const {
    // Based on the box areas
    return minX >= other.getMinX() && maxX <= other.getMaxX() && minY >= other.getMinY() && maxY <= other.getMaxY();
  }

  bool Blob::contains(const Blob &other) const {
    return other.isIn(*this);
  }

  bool Blob::overlaps(const Blob &other) const {
    return minX <= other.getMaxX() && maxX >= other.getMinX() && minY <= other.getMaxY() && maxY >= other.getMinY();
  }

  bool Blob::isClose(const Blob &other, const double &maxError) const {
    //if (overlaps(other)) // DEBUG! TODO: Check if is needed
    //  return true;
    if (!(minX <= other.getMaxX() && maxX >= other.getMinX() || minY <= other.getMaxY() && maxY >= other.getMinY()))
      return false;
    std::pair<unsigned int, unsigned int> expectedSize;
    if (isSameColor(other)) {
      expectedSize = getExpectedVirtualSize();
    } else {
      if ((isBlue() && other.isYellow()) || (isYellow() && other.isBlue()) || ((isYellowBlue() || isBlueYellow()) && (other.isYellow() || other.isBlue())) || ((isYellow() || isBlue()) && (other.isYellowBlue() || other.isBlueYellow()))) {
        // The expected sizes for both robot color combinations are the same
        expectedSize = getBlobExpectedVirtualSize(ROBOT_YELLOW_BLUE, std::pair<unsigned int, unsigned int>(centroid->getX(), getMaxY() + ROBOT_MARKER_MAX_HEIGHT));
      } else {
        expectedSize = std::pair<unsigned int, unsigned int>(0, 0);
      }
    }
    if (std::max(maxX, other.getMaxX()) - std::min(minX, other.getMinX()) <= (1 + maxError) * expectedSize.first &&
        std::max(maxY, other.getMaxY()) - std::min(minY, other.getMinY()) <= (1 + maxError) * expectedSize.second) {

      // DEBUG:
      /*std::cout << intToColor(color) << " " << intToColor(other.getColor()) << ":" << std::endl;
      std::cout << "(" << (std::max(maxX, other.getMaxX()) - std::min(minX, other.getMinX())) << " <= " << (1 + maxError) * expectedSize.first << ")" << "(" << (std::max(maxY, other.getMaxY()) - std::min(minY, other.getMinY())) << " <= " << (1 + maxError) * expectedSize.second << ")" << std::endl;
      std::cout << std::endl;*/

      return true;
    }
    return false;
  }

  void Blob::join(Blob &other) {
    // Define new box area
    minX = std::min(minX, other.getMinX());
    maxX = std::max(maxX, other.getMaxX());
    minY = std::min(minY, other.getMinY());
    maxY = std::max(maxY, other.getMaxY());
    // Add points
    numberOfPoints += other.getNumberOfPoints();
    points.insert(points.end(), other.getPoints().begin(), other.getPoints().end());
    // Calculate new centroid // TODO: Calculate based on points
    unsigned int xSum = 0, ySum = 0;
    for (std::vector<std::pair<unsigned int, unsigned int>>::iterator point = points.begin(); point != points.end(); ++point) {
      xSum += point->first;
      ySum += point->second;
    }
    centroid->setX(xSum / numberOfPoints);
    centroid->setY(ySum / numberOfPoints);
  }

  void Blob::setColor(const Color &color) {
    this->color = color;
  }

}
