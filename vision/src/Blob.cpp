/**
 * @file Blob.cpp
 * Blob seen in the camera frame.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "cameraConstants.hpp"

#include "Blob.hpp"

#include <iostream> // TODO: Remove


namespace rtx {

  Blob::Blob(const Blob &other):
    position{new Point2D(*(other.getPosition()))},
    width{other.getWidth()},
    height{other.getHeight()},
    numberOfPoints{other.getNumberOfPoints()},
    color{other.getColor()}
  {
    // Nothing to do here
  }

  Blob::Blob(const std::vector<Point2D*> &points, const Color &color) {
    this->color = color;
    minX = CAMERA_WIDTH, minY = CAMERA_HEIGHT;
    maxX = 0, maxY = 0;
    unsigned int xSum = 0, ySum = 0;
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
    width = maxX - minX + 1;
    height = maxY - minY + 1;
    position = new Point2D(xSum / numberOfPoints, ySum / numberOfPoints);
  }

  Blob::Blob(const std::vector<std::pair<unsigned int, unsigned int>> &points, const Color &color) {
    this->color = color;
    unsigned int minX = CAMERA_WIDTH - 1, minY = CAMERA_HEIGHT - 1;
    unsigned int maxX = 0, maxY = 0;
    unsigned int xSum = 0, ySum = 0;
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
    width = maxX - minX + 1;
    height = maxY - minY + 1;
    position = new Point2D(xSum / numberOfPoints, ySum / numberOfPoints);
  }

  Blob::~Blob() {
    // TODO
  }

  Point2D* Blob::getPosition() const {
    return position;
  }

  unsigned int Blob::getWidth() const {
    return width;
  }

  unsigned int Blob::getHeight() const {
    return height;
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
    return width * height;
  }

  double Blob::getDensity() const {
    return 1.0 * numberOfPoints / getBoxArea();
  }

};
