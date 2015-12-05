/**
 *  @file Perspective.cpp
 *  Perspective class for coordinate mapping.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 4 December 2015
 */

#include "Perspective.hpp"

#include "cameraConstants.hpp"


namespace rtx { namespace Vision { namespace Perspective {

  std::vector<double> A = {rtx::gC.getInt("Perspective.First.A"),
                           rtx::gC.getInt("Perspective.Second.A")};
  std::vector<double> B = {rtx::gC.getInt("Perspective.First.B"),
                           rtx::gC.getInt("Perspective.Second.B")};
  std::vector<double> C = {rtx::gC.getInt("Perspective.First.C"),
                           rtx::gC.getInt("Perspective.Second.C")};

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

  std::pair<double, double> virtualToReal(const Point2D *point, const unsigned int &cameraID) {
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
