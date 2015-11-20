/**
 * @file Color.hpp
 * Entity color definition.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 * @date 11. November 2015
 */

#ifndef RTX_VISION_COLOR_H
#define RTX_VISION_COLOR_H


namespace rtx {

  enum Color {
    BALL,
    BLUE_GOAL,
    YELLOW_GOAL,
    FIELD,
    WHITE_LINE,
    BLACK_LINE,
    CHECKERBOARD_WHITE,
    CHECKERBOARD_BLACK
  };

  Color intToColor(const unsigned int&);
  
  void getRGB(const Color&, unsigned int&, unsigned int&, unsigned int&);

};

#endif // RTX_VISION_COLOR_H
