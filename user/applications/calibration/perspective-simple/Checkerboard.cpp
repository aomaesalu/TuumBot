/**
 *  @file Checkerboard.cpp
 *  Perspective calibration application checkerboard class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#include "Checkerboard.hpp"

#include "cameraConstants.hpp"

#include <algorithm>
#include <iostream> // TODO: Remove


namespace rtx {

  Checkerboard::Checkerboard(const unsigned int &width, const unsigned int &height, const unsigned int& squareWidth):
    width(width),
    height(height),
    squareWidth(squareWidth),
    points(width + 1, std::vector<std::pair<unsigned int, unsigned int>>(height + 1, std::pair<unsigned int, unsigned int>(0, 0))),
    filled(0),
    A(0),
    B(0),
    C(0)
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

  unsigned int Checkerboard::getSquareWidth() const {
    return squareWidth;
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

  void Checkerboard::calculateConstants() {
    // Vertical formula construction
    std::vector<double> proportions;
    for (unsigned int squareSize = 1; squareSize <= std::min(width + 1, height + 1); ++squareSize) {
      for (unsigned int y = 0; y < height + 1 - squareSize; ++y) {
        for (unsigned int x = 0; x < width + 1 - squareSize; ++x) {
          unsigned int lowerLeft = y * (width + 1) + x + 1;
          unsigned int lowerRight = y * (width + 1) + (x + squareSize) + 1;
          unsigned int upperLeft = (y + squareSize) * (width + 1) + x + 1;
          unsigned int upperRight = (y + squareSize) * (width + 1) + (x + squareSize) + 1;
          if (filled < upperRight)
            continue;
          std::cout << squareSize << " " << x << " " << y << std::endl;
          proportions.push_back(squareSize * ((double) (points[x + squareSize][y + squareSize].first - points[x][y + squareSize].first) / (points[x + squareSize][y].first - points[x][y].first)));
        }
      }
    }
    B = 0;
    for (std::vector<double>::iterator p = proportions.begin(); p != proportions.end(); ++p) {
      std::cout << *p << " ";
      B += *p;
    }
    if (!proportions.empty())
      B /= proportions.size();
    std::cout << std::endl << std::endl << B << std::endl << std::endl;
      // Horisontal formula construction
      //for (unsigned int y = 0; y < height + 1; ++y) {
      //  for (unsigned int x = 0; x < width + 1 - squareSize; ++x) {
      //    unsigned int lowerLeft = y * (width + 1) + x + 1;
      //    //std::cout << x << " " << y << " " << lowerLeft << std::endl;
      //    unsigned int lowerRight = y * (width + 1) + (x + squareSize) + 1;
      //    if (filled < lowerRight)
      //      continue;
      //    std::cout << filled << " " << squareSize << " (" << x << ", " << y << ") (" << lowerLeft << ", " << lowerRight << ") " << std::endl;
          //unsigned int upperLeft = (y + squareSize) * (width + 1) + x + 1;
          //unsigned int upperRight = (y + squareSize) * (width + 1) + (x + squareSize) + 1;
          //if (filled < upperRight)
          //  continue;
          //std::cout << filled << " " << squareSize << " " << x << " " << y << " " << lowerLeft << " " << lowerRight << " " << upperLeft << " " << upperRight << std::endl;
          //std::cout << filled << " " << filled % (width + 1) << " " << filled / (width + 1) << " " << squareSize << " " << y * (width + 1) + x + 1 << " " << y * (width + 1) + (x + squareSize) + 1 << " " << (y + squareSize) * (width + 1) + x + 1 << " " << (y + squareSize) * (width + 1) + (x + squareSize) + 1 << std::endl;
          //std::pair<unsigned int, unsigned int> lowerLeft(/* TODO */, /* TODO */);
          //std::pair<double, double> lowerRight(/* TODO */, /* TODO */);
          //std::pair<double, double> upperLeft(/* TODO */, /* TODO */);
          //std::pair<double, double> upperRight(/* TODO */, /* TODO */);
      //  }
      //}
  }

  std::pair<double, double> Checkerboard::virtualToReal(const unsigned int &x, const unsigned int &y) {
    // ActualDistance = A + B / PixelVerticalCoord
    double verticalCoordinate = A + B / y;
    // ActualRight = C * PixelRight / PixelVerticalCoord
    double horisontalCoordinate = C * (x - CAMERA_WIDTH / 2) / y;
    return std::pair<double, double>(horisontalCoordinate, verticalCoordinate);
  }

  std::pair<unsigned int, unsigned int> Checkerboard::realToVirtual(const double &x, const double &y) {
    // PixelVerticalCoord = B / (ActualDistance - A)
    unsigned int verticalCoordinate = B / (y - A);
    // PixelRight = ActualRight * PixelVerticalCoord / C
    unsigned int horisontalCoordinate = x * verticalCoordinate / C;
    return std::pair<unsigned int, unsigned int>(horisontalCoordinate, verticalCoordinate);
  }

}
