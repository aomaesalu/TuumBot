/**
 * @file ImageDrawingArea.cpp
 * Perspective calibration application image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "ImageDrawingArea.hpp"

#include "MainWindow.hpp"
#include "cameraConstants.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

  ImageDrawingArea::ImageDrawingArea(MainWindow *mainWindow):
  {
    initialiseProperties();
    initialiseImage();
    intialiseConstants();
  }

  ImageDrawingArea::~ImageDrawingArea() {
    // Nothing to do here
  }

  bool ImageDrawingArea::isPlaying() const {
    return mainWindow->isPlaying();
  }

  bool ImageDrawingArea::isCalculating() const {
    return mainWindow->isMasking();
  }

  void ImageDrawingArea::setPlaying(const bool &value) {
    if (value) {
      setCalculating(false);
    }
    mainWindow->setPlaying(value);
  }

  void ImageDrawingArea::setCalculating(const bool &value) {
    if (value) {
      if (!isCalculating()) {
        setPlaying(false);
      }
    } else {
      initialiseConstants();
    }
    mainWindow->setCalculating(value);
  }

  void ImageDrawingArea::updateFrame(Frame *frame, Frame *rgbFrame) {
    this->frame = frame;
    this->rgbFrame = rgbFrame;
    image = Gdk::Pixbuf::create_from_data((const guint8*) rgbFrame->data, Gdk::COLORSPACE_RGB, false, 8, (int) rgbFrame->width, (int) rgbFrame->height, (int) rgbFrame->width * 3);
    queue_draw();
  }

  bool ImageBeforeDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (isCalculating() && !applyCalculations())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  void ImageBeforeDrawingArea::initialiseProperties() {
    set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
  }

  void ImageBeforeDrawingArea::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files

    // Show the whole image
    if (image)
      set_size_request(image->get_width(), image->get_height());
  }

  void ImageBeforeDrawingArea::initialiseConstants() {
    // TODO
  }

  void ImageDrawingArea::resetConstants() {
    // TODO
  }

  bool ImageBeforeDrawingArea::applyCalculations() {
    // TODO
    return true;
  }

  bool ImageBeforeDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

};
