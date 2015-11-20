/**
 * @file Color.hpp
 * Entity color definition.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 * @date 11. November 2015
 */

#include "Color.hpp"


namespace rtx {

  Color intToColor(const unsigned int &value) {
    return static_cast<Color>(value);
  }

  void getRGB(const Color &color, unsigned int &r, unsigned int &g, unsigned int &b) {
    switch (color) {
      case BALL:
        r = 255;
        g = 128;
        b = 0;
        break;
      case BLUE_GOAL:
        r = 0;
        g = 0;
        b = 255;
        break;
      case YELLOW_GOAL:
        r = 255;
        g = 255;
        b = 0;
        break;
      case FIELD:
        r = 0;
        g = 255;
        b = 0;
        break;
      case WHITE_LINE:
      case CHECKERBOARD_WHITE:
        r = 255;
        g = 255;
        b = 255;
        break;
      case BLACK_LINE:
      case CHECKERBOARD_BLACK:
      default:
        r = 0;
        g = 0;
        b = 0;
        break;
    }
  }

};
