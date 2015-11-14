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
    return scale->get_value();
  }

  unsigned int Brush::getRadius() const {
    return getSize() / 2;
  }

  unsigned int Brush::getUpperLimit() const {
    return scale->get_adjustment()->property_upper();
  }

  unsigned int Brush::getLowerLimit() const {
    return scale->get_adjustment()->property_lower();
  }

  unsigned int Brush::getStepSize() const {
    return scale->get_adjustment()->get_step_increment();
  }

  unsigned int Brush::getX() const {
    return x;
  }

  unsigned int Brush::getY() const {
    return y;
  }

  unsigned int Brush::getMinX() const {
    unsigned int value = x - getRadius();
    if (value >= CAMERA_WIDTH) {
      value = 0;
    }
    return value;
  }

  unsigned int Brush::getMinY() const {
    unsigned int value = y - radius;
    if (value >= CAMERA_HEIGHT) {
      value = 0;
    }
    return value;
  }

  unsigned int Brush::getMaxX() const {
    unsigned int value = x + radius;
    if (value >= CAMERA_WIDTH) {
      value = CAMERA_WIDTH - 1;
    }
    return value;
  }

  unsigned int Brush::getMaxY() const {
    unsigned int value = y + radius;
    if (value >= CAMERA_HEIGHT) {
      value = CAMERA_HEIGHT - 1;
    }
  }

  void Brush::setSize(const unsigned int &value) {
    scale->set_value(value);
  }

  void Brush::incrementSize() {
    setSize(getSize() + getStepSize());
  }

  void Brush::decrementSize() {
    setSize(getSize() - getStepSize());
  }

  void Brush::setX(const unsigned int &value) {
    x = value;
  }

  void Brush::setY(const unsigned int &value) {
    y = value;
  }

}
