/**
 * @file Blob.cpp
 * Blob seen in the camera frame.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Blob.hpp"


namespace rtx {

  Blob::Blob(const Blob &other) {
    position{new Point2D(other.getPosition())},
    width{other.getWidth()},
    height{other.getHeight()},
    numberOfPoints{other.getNumberOfPoints()},
    color{other.getColor()}
  }

  Blob::Blob(const std::vector<Point2D*> &points) {
    // TODO
  }

  Blob::Blob(const std::vector<std::pair<unsigned int, unsigned int>> &points) {
    // TODO
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
