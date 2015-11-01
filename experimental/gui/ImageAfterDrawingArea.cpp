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

void calculateFilterBuffer(const std::vector<std::vector<bool> &additionMask, const std::vector<std::vector<bool>> &removalMask) {
  calculateFilterAdditionBuffer(additionMask);
  calculateFilterRemovalBuffer(removalMask);
}

void addBufferToFilter() {
  addRemovalBufferToFilter();
  addAdditionBufferToFilter();
}

bool ImageAfterDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
  if (!image)
    return false;

  Gdk::Cairo::set_source_pixbuf(cairo, image, 0, 0);

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
  initialiseFilter();
  initialiseFilterAdditionBuffer();
  initialiseFilterRemovalBuffer();
}

void ImageAfterDrawingArea::initialiseFilterMap(std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> &filterMap) {
  filterMap.clear();
  std::map<unsigned int, bool> subSubFilter;
  for (unsigned int i = 0; i < 256; ++i) {
    subSubFilter[i] = false;
  }
  std::map<unsigned int, std::map<unsigned int, bool>> subFilter;
  for (unsigned int i = 0; i < 256; ++i) {
    subFilter[i] = subSubFilter;
  }
  for (unsigned int i = 0; i < 256; ++i) {
    filterMap[i] = subFilter;
  }
}

void ImageAfterDrawingArea::initialiseFilter() {
  initialiseFilterMap(filter);
}

void ImageAfterDrawingArea::initialiseFilterAdditionBuffer() {
  initialiseFilterMap(filterAdditionBuffer);
}

void ImageAfterDrawingArea::initialiseFilterAdditionBuffer() {
  initialiseFilterMap(filterAdditionBuffer);
}

void ImageAfterDrawingArea::resetFilterMap(std::map<unsigned int, std::map<unsigned int, std::map<unsigned int, bool>>> &filterMap) {
  for (unsigned int i = 0; i < filterMap.size(); ++i) {
    for (unsigned int j = 0; j < filterMap[i].size(); ++j) {
      for (unsigned int k = 0; k < filterMap[i][j].size(); ++k) {
        filterMap[i][j][k] = false;
      }
    }
  }
}

void ImageAfterDrawingArea::resetFilter() {
  resetFilterMap(filter);
}

void ImageAfterDrawingArea::resetFilterAdditionBuffer() {
  resetFilterMap(filterAdditionBuffer);
}

void ImageAfterDrawingArea::resetFilterRemovalBuffer() {
  resetFilterMap(filterRemovalBuffer);
}

bool applyFilter() {
  filteredImage = image->copy(); // TODO: Copy only where is necessary (?)
  guint8 *pixels = filteredImage->get_pixels();
  unsigned int channels = filteredImage->get_n_channels();
  unsigned int stride = filteredImage->get_rowstride();

  // Color pixels
  for (unsigned int i = 0; i < 640; ++i) {
    for (unsigned int j = 0; j < 480; ++j) {
      guint8 *pixel = pixels + i * channels + j * stride;
      if (filter[pixel[0]][pixel[1]][pixel[2]]) { // TODO: Add buffers
        pixel[0] *= 0.3;
        pixel[1] *= 0.3;
        pixel[2] *= 0.3;
      }
    }
  }

  return true;
}

bool ImageBeforeDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
  if (!filteredImage)
    return false;

  Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

  return true;
}

void ImageBeforeDrawingArea::calculateFilterAdditionBuffer() {
  resetFilterAdditionBuffer();
  for (unsigned int i = 0; i < mask.size(); ++i) {
    for (unsgined int j = 0; j < mask[i].size(); ++j) {
      // TODO
    }
  }
}

void ImageBeforeDrawingArea::calculateFilterRemovalBuffer() {
  resetFilterRemovalBuffer();
  for (unsigned int i = 0; i < mask.size(); ++i) {
    for (unsgined int j = 0; j < mask[i].size(); ++j) {
      // TODO
    }
  }
}

void ImageBeforeDrawingArea::addAdditionBufferToFilter() {
  // TODO
  initialiseFilterAdditionBuffer();
}

void ImageBeforeDrawingArea::addRemovalBufferToFilter() {
  // TODO
  initialiseFilterRemovalBuffer();
}
