/**
 *  @file Checkerboard.cpp
 *  Perspective calibration application checkerboard class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#include "Checkerboard.hpp"


namespace rtx {

  Checkerboard::Checkerboard(const unsigned int &width, const unsigned int &height, const unsigned int& squareWidth):
    width(width),
    height(height),
    squareWidth(squareWidth),
    points(width + 1, std::vector<std::pair<unsigned int, unsigned int>>(height + 1, std::pair<unsigned int, unsigned int>(0, 0))),
    filled(0)
  {

  }

  const std::vector<std::vector<std::pair<unsigned int, unsigned int>>>& Checkerboard::getPoints() const {
    return points;
  }

  unsigned int Checkerboard::getWidth() const {
    return width;
  }

  unsigned int Checkerboard::getHeight() const {
    return height;
  }

  unsigned int Checkerboard::getSize() const {
    return width * height;
  }

  unsigned int Checkerboard::getNumberOfPointsFilled() const {
    return filled;
  }

  bool Checkerboard::isEmpty() const {
    return filled == 0;
  }

  bool Checkerboard::isFilled() const {
    return filled == (width + 1) * (height + 1);
  }

  void Checkerboard::addPoint(const unsigned int &x, const unsigned int &y) {
    if (!isFilled()) {
      points[filled % (width + 1)][filled / (width + 1)] = std::pair<unsigned int, unsigned int>(x, y);
      filled++;
    }
  }

}
