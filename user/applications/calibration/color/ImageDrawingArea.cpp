/**
 * @file ImageDrawingArea.cpp
 * Color calibration application image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "ImageDrawingArea.hpp"

#include "MainWindow.hpp"

#include <cairomm/context.h>


namespace rtx {

  ImageDrawingArea::ImageDrawingArea(MainWindow *mainWindow):
    mainWindow(mainWindow)
  {
    // Nothing to do here
  }

  ImageDrawingArea::~ImageDrawingArea() {
    // Nothing to do here
  }

  bool ImageDrawingArea::isPlaying() const {
    return mainWindow->isPlaying();
  }

  void ImageDrawingArea::setPlaying(const bool &value) {
    mainWindow->setPlaying(value);
  }

};
