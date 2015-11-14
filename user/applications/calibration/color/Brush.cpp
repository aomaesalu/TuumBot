/**
 *  @file Brush.cpp
 *  Color calibration application brush class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#include "Brush.hpp"


namespace rtx {

  Brush::Brush(Gtk::Scale *scale) {
    scale = scale;
    x = 0;
    y = 0;
  }

  Brush::~Brush() {
    // TODO
  }

  Gtk::Scale* Brush::getScale() const {
    return scale;
  }

  unsigned int Brush::getSize() const {
    // TODO
  }

  unsigned int Brush::getX() const {
    return x;
  }

  unsigned int Brush::getY() const {
    return y;
  }

  void Brush::setX(const unsigned int &value) {
    x = value;
  }

  void Brush::setY(const unsigned int &value) {
    y = value;
  }

}
