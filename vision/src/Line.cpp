/**
 *  @file Line.cpp
 *  Line seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#include "Line.hpp"


namespace rtx {

  Line::Line(const Line &other):
    distance(other.getDistance()),
    angle(other.getAngle())
  {
    // Nothing to do here
  }

  Line::Line(const double &distance, const double &angle):
    distance(distance),
    angle(angle)
  {
    // Nothing to do here
  }

  Line::Line(const std::vector<std::pair<double, double>> &points) {
    // TODO: Use regression instead?
    // Find average point
    std::pair<double, double> averagePoint(0, 0)
    for (std::vector<std::pair<double, double>>::const_iterator point = points.begin(); point != points.end(); ++point) {
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
    std::pair<double, double> perpendicularPoint(perpendicularX, perpendicularY);
    // Find distance and angle
    // TODO
  }

  Line::~Line() {
    // Nothing to do here
  }

  double Line::getDistance() const {
    return distance;
  }

  double Line::getAngle() const {
    return angle;
  }

  void Line::setDistance(const double &distance) {
    this->distance = distance;
  }

  void Line::setAngle(const double &angle) {
    this->angle = angle;
  }

}
