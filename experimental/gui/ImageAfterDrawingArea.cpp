/**
 * @file ImageDrawingArea.cpp
 * Experimental GUI color calibration application "after" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "ImageAfterDrawingArea.hpp"

#include "MainWindow.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream>


ImageAfterDrawingArea::ImageAfterDrawingArea(MainWindow *mainWindow, Gtk::Scale *deltaScale):
  ImageDrawingArea(mainWindow)
{
  initialiseProperties();
  initialiseImage();
  initialiseDeltaScale(deltaScale);
  initialiseFilters();
}

ImageAfterDrawingArea::~ImageAfterDrawingArea() {
  // Nothing to do here
}

void ImageAfterDrawingArea::calculateFilterBuffer(const std::vector<std::set<unsigned int>> &additionMaskList, const std::vector<std::set<unsigned int>> &removalMaskList) {
  resetFilterBuffers();

  unsigned int mode = mainWindow->getMode();

  guint8 *pixels = image->get_pixels();
  unsigned int channels = image->get_n_channels();
  unsigned int stride = image->get_rowstride();

  for (std::set<unsigned int>::iterator i = additionMaskList[mode].begin(); i != additionMaskList[mode].end(); ++i) {
    guint8 *pixel = pixels + ((*i) % 640) * channels + ((*i) / 640) * stride;
    filterAdditionBufferList[mode].insert(pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2]);
  }

  for (std::set<unsigned int>::iterator i = removalMaskList[mode].begin(); i != removalMaskList[mode].end(); ++i) {
    guint8 *pixel = pixels + ((*i) % 640) * channels + ((*i) / 640) * stride;
    filterRemovalBufferList[mode].insert(pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2]);
  }

  // Redraw
  queue_draw();
}

void ImageAfterDrawingArea::addBufferToFilter() {
  unsigned int mode = mainWindow->getMode();

  for (std::set<unsigned int>::iterator i = filterRemovalBufferList[mode].begin(); i != filterRemovalBufferList[mode].end(); ++i) {
    filter[mode].erase(*i);
  }

  for (std::set<unsigned int>::iterator i = filterAdditionBufferList[mode].begin(); i != filterAdditionBufferList[mode].end(); ++i) {
    filter[mode].insert(*i);
  }

  resetFilterBuffers();

  // Redraw
  queue_draw();
}

std::string ImageAfterDrawingArea::getOutput() const {
  std::string output = "";
  for (unsigned int i = 0; i < 256 * 256 * 256; ++i) {
    char colorValue = 0;
    for (unsigned int mode = 0; mode < 8; ++mode) { // We assume 0 < numberOfModes <= 8
      bool modeValue = 0;
      if (mode < mainWindow->getModes().size()) {
        if (filter[mode].find(i) != filter[mode].end()) {
          modeValue = 1;
        }
      }
      colorValue = (colorValue << 1) + modeValue;
    }
    output += colorValue;
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
  set_size_request(640, 480);
}

void ImageAfterDrawingArea::initialiseImage() {
  image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files

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

void ImageAfterDrawingArea::resetFilter() {
  unsigned int mode = mainWindow->getMode();
  filter[mode].clear();
}

void ImageAfterDrawingArea::resetFilterBuffers() {
  unsigned int mode = mainWindow->getMode();
  filterAdditionBufferList[mode].clear();
  filterRemovalBufferList[mode].clear();
}

bool ImageAfterDrawingArea::applyFilter() {
  filteredImage = image->copy(); // TODO: Copy only where is necessary (?)

  unsigned int mode = mainWindow->getMode();

  guint8 *pixels = filteredImage->get_pixels();
  unsigned int channels = filteredImage->get_n_channels();
  unsigned int stride = filteredImage->get_rowstride();

  // Color pixels
  for (unsigned int i = 0; i < 640; ++i) {
    for (unsigned int j = 0; j < 480; ++j) {
      guint8 *pixel = pixels + i * channels + j * stride;
      unsigned int pixelCode = pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2];
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
