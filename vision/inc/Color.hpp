/**
 * @file Color.hpp
 * Entity color definition.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 * @date 29 November 2015
 */

#ifndef RTX_VISION_COLOR_H
#define RTX_VISION_COLOR_H

#include <utility>

#include "Perspective.hpp"
#include "Point2D.hpp"


namespace rtx {

  enum Color {
    BALL,
    BLUE_GOAL,
    YELLOW_GOAL,
    FIELD,
    WHITE_LINE,
    BLACK_LINE,
    CHECKERBOARD_WHITE,
    CHECKERBOARD_BLACK,
    ROBOT_YELLOW_BLUE,
    ROBOT_BLUE_YELLOW
  };

  Color intToColor(const unsigned int&);

  void getRGB(const Color&, unsigned int&, unsigned int&, unsigned int&);

  std::pair<double, double> getBlobExpectedRealSize(const Color&);
  std::pair<unsigned int, unsigned int> getBlobExpectedVirtualSize(const Color&, const std::pair<unsigned int, unsigned int>&, const unsigned int&);
  std::pair<unsigned int, unsigned int> getBlobExpectedVirtualSize(const Color&, const Point2D*, const unsigned int&);

};

#endif // RTX_VISION_COLOR_H
