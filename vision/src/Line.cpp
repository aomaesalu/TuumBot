/**
 *  @file Line.cpp
 *  Line seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#include "Line.hpp"

#include <cmath>
#include <iostream> // TODO: Remove


namespace rtx { namespace Vision {

  Line::Line(const Line &other):
    Feature(other)
  {
    // Nothing to do here
  }

  Line::Line(const double &distance, const double &angle):
    Feature(distance, angle)
  {
    // Nothing to do here
  }

  Line::Line(const std::vector<std::pair<double, double>> &points):
    Feature(0, 0)
  {
    std::cout << "Line:" << std::endl;
    // TODO: Use regression instead?
    // Find average point
    std::pair<double, double> averagePoint(0, 0);
    for (std::vector<std::pair<double, double>>::const_iterator point = points.begin(); point != points.end(); ++point) {
      std::cout << "(" << point->first << ", " << point->second << ")" << std::endl;
      averagePoint.first += point->first;
      averagePoint.second += point->second;
    }
    averagePoint.first /= points.size();
    averagePoint.second /= points.size();
    // Find average slope
    double averageSlope = 0;
    for (unsigned int i = 0; i < points.size() - 1; ++i) {
      averageSlope += (points[i + 1].second - points[i].second) / (points[i + 1].first - points[i].first);
    }
    averageSlope /= points.size() - 1;
    // Find perpendicular line point
    double perpendicularSlope = -1 / averageSlope;
    double perpendicularX = (averagePoint.second - averageSlope * averagePoint.first) / (perpendicularSlope - averageSlope);
    double perpendicularY = perpendicularSlope * perpendicularX;
    // Find distance and angle
    distance = sqrt(perpendicularY * perpendicularY + perpendicularX * perpendicularX);
    angle = -atan2(perpendicularX, perpendicularY);
    std::cout << std::endl << std::endl;
  }

  Line::~Line() {
    // Nothing to do here
  }

  double Line::getSlope() const {
    std::pair<double, double> point = getRelativePoint();
    return point.second / point.first;
  }

}}
