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

};
