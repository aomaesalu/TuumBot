/**
 *  @file Perspective.cpp
 *  Perspective class for coordinate mapping.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 26 November 2015
 */

#include "Perspective.hpp"

#include "cameraConstants.hpp"


namespace rtx { namespace Vision { namespace Perspective {

  double A = 21;
  double B = 93048;
  double C = 150;

  std::pair<double, double> virtualToReal(const unsigned int &x, const unsigned int &y) {
    // ActualDistance = A + B / PixelVerticalCoord
    double verticalCoordinate = A + B / y;
    // ActualRight = C * PixelRight / PixelVerticalCoord
    double horisontalCoordinate = C * ((double) x - CAMERA_WIDTH / 2.0) / y;
    return std::pair<double, double>(horisontalCoordinate, verticalCoordinate);
  }

  std::pair<double, double> virtualToReal(const std::pair<unsigned int, unsigned int> &point) {
    return virtualToReal(point.first, point.second);
  }

  std::pair<unsigned int, unsigned int> realToVirtual(const double &x, const double &y) {
    // PixelVerticalCoord = B / (ActualDistance - A)
    unsigned int verticalCoordinate = B / (y - A);
    // PixelRight = ActualRight * PixelVerticalCoord / C
    unsigned int horisontalCoordinate = x * verticalCoordinate / C + CAMERA_WIDTH / 2.0;
    return std::pair<unsigned int, unsigned int>(horisontalCoordinate, verticalCoordinate);
  }

  std::pair<unsigned int, unsigned int> realToVirtual(const std::pair<double, double> &point) {
    return realToVirtual(point.first, point.second);
  }

}}}
