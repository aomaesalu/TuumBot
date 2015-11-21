/**
 *  @file ImageDrawingArea.cpp
 *  Color calibration application "after" image drawing area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#include "ImageAfterDrawingArea.hpp"

#include "MainWindow.hpp"
#include "cameraConstants.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

  ImageAfterDrawingArea::ImageAfterDrawingArea(MainWindow *mainWindow, Gtk::Scale *deltaScale):
    ImageDrawingArea(mainWindow)
  {
    initialiseProperties();
    initialiseImage();
    initialiseDeltaScale(deltaScale);
    initialiseFilters();
    initialiseSavingMap();
  }

  ImageAfterDrawingArea::~ImageAfterDrawingArea() {
    // Nothing to do here
  }

  void ImageAfterDrawingArea::filterFromString(const std::string &filterString) {
    resetFilter();
    resetFilterBuffers();
    for (unsigned int i = 0; i < 256 * 256 * 256; ++i) {
      unsigned int bitmask = 1 << 7;
      for (unsigned int mode = 0; mode < 8; ++mode) {
        if (filterString[i] & bitmask) {
          filter[mode].insert(i);
        }
        bitmask >>= 1;
      }
    }
  }

  void ImageAfterDrawingArea::calculateFilterBuffer(const std::vector<std::set<unsigned int>> &additionMaskList, const std::vector<std::set<unsigned int>> &removalMaskList) {
    resetFilterBuffers();

    //unsigned int mode = mainWindow->getMode();

    guint8 *pixels = frame->data;
    unsigned int channels = 3;
    unsigned int stride = frame->width * channels;

    for (unsigned int mode = 0; mode < 8; ++mode) {
      for (std::set<unsigned int>::iterator i = additionMaskList[mode].begin(); i != additionMaskList[mode].end(); ++i) {
        guint8 *pixel = pixels + ((*i) % CAMERA_WIDTH) * channels + ((*i) / CAMERA_WIDTH) * stride;
        for (int j = -3; j <= 3; ++j) {
          for (int k = -3; k <= 3; ++k) {
            for (int m = -3; m <= 3; ++m) {
              if (pixel[0] + j >= 0 && pixel[0] + j < 256 && pixel[1] + k >= 0 && pixel[1] + k < 256 && pixel[2] + m >= 0 && pixel[2] < 256) {
                filterAdditionBufferList[mode].insert((pixel[0] + j) * 256 * 256 + (pixel[1] + k) * 256 + pixel[2] + m);
              }
            }
          }
        }
        //filterAdditionBufferList[mode].insert(pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2]);
      }

      for (std::set<unsigned int>::iterator i = removalMaskList[mode].begin(); i != removalMaskList[mode].end(); ++i) {
        guint8 *pixel = pixels + ((*i) % CAMERA_WIDTH) * channels + ((*i) / CAMERA_WIDTH) * stride;
        for (int j = -3; j <= 3; ++j) {
          for (int k = -3; k <= 3; ++k) {
            for (int m = -3; m <= 3; ++m) {
              if (pixel[0] + j >= 0 && pixel[0] + j < 256 && pixel[1] + k >= 0 && pixel[1] + k < 256 && pixel[2] + m >= 0 && pixel[2] < 256) {
                filterRemovalBufferList[mode].insert((pixel[0] + j) * 256 * 256 + (pixel[1] + k) * 256 + pixel[2] + m);
              }
            }
          }
        }
        //filterRemovalBufferList[mode].insert(pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2]);
      }
    }

    // Redraw
    queue_draw();
  }

  void ImageAfterDrawingArea::addBufferToFilter() {
    //unsigned int mode = mainWindow->getMode();
    for (unsigned int mode = 0; mode < 8; ++mode) {
      for (std::set<unsigned int>::iterator i = filterRemovalBufferList[mode].begin(); i != filterRemovalBufferList[mode].end(); ++i) {
        filter[mode].erase(*i);
      }

      for (std::set<unsigned int>::iterator i = filterAdditionBufferList[mode].begin(); i != filterAdditionBufferList[mode].end(); ++i) {
        filter[mode].insert(*i);
      }
    }

    resetFilterBuffers();

    // Redraw
    queue_draw();
  }

  void ImageAfterDrawingArea::updateFrame(Frame *frame, Frame *rgbFrame) {
    this->frame = frame;
    this->rgbFrame = rgbFrame;
    image = Gdk::Pixbuf::create_from_data((const guint8*) rgbFrame->data, Gdk::COLORSPACE_RGB, false, 8, (int) rgbFrame->width, (int) rgbFrame->height, (int) rgbFrame->width * 3);
    queue_draw();
  }

  bool isInFilter(const std::vector<std::set<unsigned int>> &filter, const unsigned int &mode, const unsigned int &value) {
    return filter[mode].find(value) != filter[mode].end();
  }

  std::string ImageAfterDrawingArea::getOutput() const {
    std::string output(256 * 256 * 256, (unsigned char) 0);
    unsigned int bitmask = 1 << 7;
    for (unsigned int mode = 0; mode < 8; ++mode) {
      for (std::set<unsigned int>::iterator value = filter[mode].begin(); value != filter[mode].end(); ++value) {
        output[*value] |= bitmask;
      }
      bitmask >>= 1;
    }
    return output;
  }

  bool ImageAfterDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!applyFilter())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  bool ImageAfterDrawingArea::on_scroll_event(GdkEventScroll *scrollEvent) {
    if (scrollEvent->direction == GDK_SCROLL_UP) {
      if (deltaScale->get_value() < deltaScale->get_adjustment()->property_upper()) {
        deltaScale->set_value(deltaScale->get_value() + deltaScale->get_adjustment()->get_step_increment());
      }
    } else if (scrollEvent->direction == GDK_SCROLL_DOWN) {
      if (deltaScale->get_value() > deltaScale->get_adjustment()->property_lower()) {
        deltaScale->set_value(deltaScale->get_value() - deltaScale->get_adjustment()->get_step_increment());
      }
    }
    return true;
  }

  void ImageAfterDrawingArea::initialiseProperties() {
    set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
  }

  void ImageAfterDrawingArea::initialiseImage() {
    image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files
    //image = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, (int) image->get_width(), (int) image->get_height());

    // Show the whole image
    if (image)
      set_size_request(image->get_width(), image->get_height());
  }

  void ImageAfterDrawingArea::initialiseDeltaScale(Gtk::Scale *deltaScale) {
    this->deltaScale = deltaScale;
    // TODO: Define exact usage
  }

  void ImageAfterDrawingArea::initialiseFilters() {
    std::set<unsigned int> emptySet;
    for (unsigned int i = 0; i < mainWindow->getModes().size(); ++i) {
      filter.push_back(emptySet);
      filterAdditionBufferList.push_back(emptySet);
      filterRemovalBufferList.push_back(emptySet);
    }
  }

  void ImageAfterDrawingArea::initialiseSavingMap() {
    for (unsigned int i = 0; i < 256; ++i) {
      bool v[8];
      for (unsigned int j = 0; j < 8; ++j) {
        v[j] = (i >> (7 - j)) & 0x1;
      }
      savingMap[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] = i;
    }
  }

  void ImageAfterDrawingArea::resetFilter() {
    unsigned int mode = mainWindow->getMode();
    filter[mode].clear();
  }

  void ImageAfterDrawingArea::resetFilterBuffers() {
    //unsigned int mode = mainWindow->getMode();
    for (unsigned int mode = 0; mode < 8; ++mode) {
      filterAdditionBufferList[mode].clear();
      filterRemovalBufferList[mode].clear();
    }
  }

  bool ImageAfterDrawingArea::applyFilter() {
    filteredImage = image->copy(); // TODO: Copy only where is necessary (?)

    unsigned int mode = mainWindow->getMode();

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
        unsigned int pixelCode = actualPixel[0] * 256 * 256 + actualPixel[1] * 256 + actualPixel[2];
        if (((filter[mode].find(pixelCode) != filter[mode].end()) && (filterRemovalBufferList[mode].find(pixelCode) == filterRemovalBufferList[mode].end())) || (filterAdditionBufferList[mode].find(pixelCode) != filterAdditionBufferList[mode].end())) {
          pixel[0] *= 0.2;
          pixel[1] *= 0.2;
          pixel[2] *= 0.2;
        }
      }
    }

    return true;
  }

  bool ImageAfterDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

};
