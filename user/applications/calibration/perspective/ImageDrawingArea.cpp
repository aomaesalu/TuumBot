/**
 * @file ImageDrawingArea.cpp
 * Perspective calibration application image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 * @date 11. November 2015
 */

#include "ImageDrawingArea.hpp"

#include "MainWindow.hpp"
#include "cameraConstants.hpp"
#include "tuum_visioning.hpp" // TODO: Correct

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

  ImageDrawingArea::ImageDrawingArea(MainWindow *mainWindow):
    mainWindow(mainWindow)
  {
    initialiseProperties();
    initialiseImage();
    initialiseConstants();
  }

  ImageDrawingArea::~ImageDrawingArea() {
    // Nothing to do here
  }

  bool ImageDrawingArea::isPlaying() const {
    return mainWindow->isPlaying();
  }

  bool ImageDrawingArea::isCalculating() const {
    return mainWindow->isCalculating();
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

  bool ImageDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!applyCalculations())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  void ImageDrawingArea::initialiseProperties() {
    set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
  }

  void ImageDrawingArea::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());

    // Show the whole image
    if (image)
      set_size_request(image->get_width(), image->get_height());
  }

  void ImageDrawingArea::initialiseConstants() {
    // TODO
  }

  void ImageDrawingArea::resetConstants() {
    // TODO
  }

  bool ImageDrawingArea::applyCalculations() {
    filteredImage = image->copy(); // TODO: Copy only where is necessary (?)

    guint8 *pixels = filteredImage->get_pixels();
    unsigned int channels = filteredImage->get_n_channels();
    unsigned int stride = filteredImage->get_rowstride();

    guint8 *actualPixels = frame->data;
    unsigned int actualChannels = 3;
    unsigned int actualStride = frame->width * actualChannels;

    // Color pixels
    for (unsigned int i = 0; i < CAMERA_WIDTH; ++i) {
      for (unsigned int j = 0; j < CAMERA_HEIGHT; ++j) {
        guint8 *pixel = pixels + i * channels + j * stride;
        guint8 *actualPixel = actualPixels + i * actualChannels + j * actualStride;
        if (mainWindow->isColored(actualPixel[0], actualPixel[1], actualPixel[2], 6)) {
          if (mainWindow->isColored(actualPixel[0], actualPixel[1], actualPixel[2], 7)) {
            pixel[0] = 127;
            pixel[1] = 127;
            pixel[2] = 127;
          } else {
            pixel[0] = 235;
            pixel[1] = 235;
            pixel[2] = 235;
          }
        } else if (mainWindow->isColored(actualPixel[0], actualPixel[1], actualPixel[2], 7)) {
          pixel[0] = 0;
          pixel[1] = 0;
          pixel[2] = 0;
        }
      }
    }

    for (Vision::BlobSet::iterator blob = Vision::blobs.begin(); blob != Vision::blobs.end(); ++blob) {
      if (*blob) {
        unsigned int x = (*blob)->getPosition()->getX();
        unsigned int y = (*blob)->getPosition()->getY();
        unsigned int minX = (*blob)->getMinX();
        unsigned int maxX = (*blob)->getMaxX();
        unsigned int minY = (*blob)->getMinY();
        unsigned int maxY = (*blob)->getMaxY();
        if (minX >= CAMERA_WIDTH || maxX >= CAMERA_WIDTH || minY >= CAMERA_HEIGHT || maxY >= CAMERA_HEIGHT) {
          continue;
        }
        //std::cout << (*blob)->getPosition()->getX() << " " << (*blob)->getPosition()->getY() << " " << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
        Color color = (*blob)->getColor();

        unsigned int value = 0;
        if (color == CHECKERBOARD_WHITE) {
          value = 235;
        }
        for (unsigned int i = minX; i <= maxX; ++i) {
          guint8 *pixel = pixels + i * channels + minY * stride;
          for (unsigned int p = 0; p < 3; ++p) {
            pixel[p] = value;
          }
          pixel = pixels + i * channels + maxY * stride;
          for (unsigned int p = 0; p < 3; ++p) {
            pixel[p] = value;
          }
        }
        for (unsigned int j = minY; j <= maxY; ++j) {
          guint8 *pixel = pixels + minX * channels + j * stride;
          for (unsigned int p = 0; p < 3; ++p) {
            pixel[p] = value;
          }
          pixel = pixels + maxX * channels + j * stride;
          for (unsigned int p = 0; p < 3; ++p) {
            pixel[p] = value;
          }
        }

        for (int dx = -3; dx < 3; ++dx) {
          for (int dy = -3; dy < 3; ++dy) {
            if (x + dx < CAMERA_WIDTH && x + dx >= 0 && y + dy < CAMERA_HEIGHT && y + dy >= 0) {
              guint8 *pixel = pixels + (x + dx) * channels + (y + dy) * stride;
              pixel[0] = 255;
              pixel[1] = 0;
              pixel[2] = 0;
            }
          }
        }

      }
    }

    return true;
  }

  bool ImageDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

};
