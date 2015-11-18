/**
 * @file Blob.cpp
 * Blob seen in the camera frame.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "cameraConstants.hpp"

#include "Blob.hpp"

#include <algorithm>
#include <iostream> // TODO: Remove


namespace rtx {

  Blob::Blob(const Blob &other):
    position{new Point2D(*(other.getPosition()))},
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
    position = new Point2D(xSum / numberOfPoints, ySum / numberOfPoints);
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
    position = new Point2D(xSum / numberOfPoints, ySum / numberOfPoints);
  }

  Blob::~Blob() {
    // TODO
  }

  const std::vector<std::pair<unsigned int, unsigned int>>& Blob::getPoints() { // TODO: Constness
    return points;
  }

  Point2D* Blob::getPosition() const {
    return position;
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

  double Blob::getDensity() const {
    return 1.0 * numberOfPoints / getBoxArea();
  }

  bool Blob::overlaps(const Blob &other) const {
    return color == other.getColor() && minX <= other.getMaxX() && maxX >= other.getMinX() && minY <= other.getMaxY() && maxY >= other.getMinY();
  }

  bool Blob::isClose(const Blob &other) const {
    if (overlaps(other))
      return true;
    if (color != other.getColor())
      return false;
    if (minY <= other.getMaxY() && maxY >= other.getMinY()) { // The rectangles overlap by the Y coordinate
      if (std::min(getHeight(), other.getHeight()) - (std::min(maxY, other.getMaxY()) - std::max(minY, other.getMinY())) < 0.5) { // The Y coordinate overlapping is over half of the height of the smaller blob
        if (maxX <= other.getMinX()) { // Current is to the left of other
          unsigned int intermediateWidth = other.getMinX() - maxX;
          if (intermediateWidth < std::max(getWidth(), other.getWidth()) / 2) {
            return true;
          }
        } else if (minX >= other.getMaxX()) { // Current is to the right of other
          unsigned int intermediateWidth = minX - other.getMaxX();
          if (intermediateWidth < std::max(getWidth(), other.getWidth()) / 2) {
            return true;
          }
        }
      }
    } else if (minX <= other.getMaxX() && maxX >= other.getMinX()) { // The rectangles overlap by the X coordinate
      if (std::min(getWidth(), other.getWidth()) - (std::min(maxX, other.getMaxX()) - std::max(minX, other.getMinX())) < 0.5) { // The X coordinate overlapping is over half of the width of the smaller blob
        if (maxY <= other.getMinY()) { // Current is above the other
          unsigned int intermediateHeight = other.getMinY() - maxY;
          if (intermediateHeight < std::max(getHeight(), other.getHeight()) / 2) {
            return true;
          }
        } else if (minY >= other.getMaxY()) { // Current is below the other
          unsigned int intermediateHeight = minY - other.getMaxY();
          if (intermediateHeight < std::max(getHeight(), other.getHeight()) / 2) {
            return true;
          }
        }
      }
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
    // Calculate new position // TODO: Calculate based on points
    unsigned int xSum = 0, ySum = 0;
    for (std::vector<std::pair<unsigned int, unsigned int>>::iterator point = points.begin(); point != points.end(); ++point) {
      xSum += point->first;
      ySum += point->second;
    }
    position->setX(xSum / numberOfPoints);
    position->setY(ySum / numberOfPoints);
  }

};
