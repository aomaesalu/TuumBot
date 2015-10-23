/**
 * @file ImageDrawingArea.cpp
 * Experimental GUI color calibration application image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include <cairomm/context.h>

#include "ImageDrawingArea.hpp"


ImageDrawingArea::ImageDrawingArea() {
  setPlaying();
}

ImageDrawingArea::~ImageDrawingArea() {
  // Nothing to do here
}

bool ImageDrawingArea::isPlaying() const {
  return playing;
}

void ImageDrawingArea::setPlaying(const bool &value) {
  playing = value;
}
