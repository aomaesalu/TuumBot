/**
 *  @file Perspective.cpp
 *  Perspective class for coordinate mapping.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 4 December 2015
 */

#include "Perspective.hpp"

#include "cameraConstants.hpp"


namespace rtx { namespace Vision { namespace Perspective {

  std::vector<double> A = {2, 2};//21;
  std::vector<double> B = {105436, 105436};//93048;
  std::vector<double> C = {169, 169};//150;

  std::pair<double, double> virtualToReal(const unsigned int &x, const unsigned int &y, const unsigned int &cameraID) {
    // ActualDistance = A + B / PixelVerticalCoord
    double verticalCoordinate = A[cameraID] + B[cameraID] / y;
    // ActualRight = C * PixelRight / PixelVerticalCoord
    double horisontalCoordinate = C[cameraID] * ((double) x - CAMERA_WIDTH / 2.0) / y;
    return std::pair<double, double>(horisontalCoordinate, verticalCoordinate);
  }

  std::pair<double, double> virtualToReal(const std::pair<unsigned int, unsigned int> &point, const unsigned int &cameraID) {
    return virtualToReal(point.first, point.second, cameraID);
  }

  std::pair<double, double> virtualToReal(const Point2D *point) {
    return virtualToReal(point->getX(), point->getY(), cameraID);
  }

  std::pair<unsigned int, unsigned int> realToVirtual(const double &x, const double &y, const unsigned int &cameraID) {
    // PixelVerticalCoord = B / (ActualDistance - A)
    unsigned int verticalCoordinate = B[cameraID] / (y - A[cameraID]);
    // PixelRight = ActualRight * PixelVerticalCoord / C
    unsigned int horisontalCoordinate = x * verticalCoordinate / C[cameraID] + CAMERA_WIDTH / 2.0;
    return std::pair<unsigned int, unsigned int>(horisontalCoordinate, verticalCoordinate);
  }

  std::pair<unsigned int, unsigned int> realToVirtual(const std::pair<double, double> &point, const unsigned int &cameraID) {
    return realToVirtual(point.first, point.second, cameraID);
  }

}}}
