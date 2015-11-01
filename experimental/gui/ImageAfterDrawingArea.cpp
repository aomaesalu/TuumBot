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

void ImageAfterDrawingArea::calculateFilterBuffer(const std::vector<std::vector<bool>> &additionMask, const std::vector<std::vector<bool>> &removalMask) { // TODO: Optimise speed
  resetFilterBuffers();

  guint8 *pixels = image->get_pixels();
  unsigned int channels = image->get_n_channels();
  unsigned int stride = image->get_rowstride();

  for (unsigned int i = 0; i < additionMask.size(); ++i) { // We know that the addition mask and the removal mask are the same size
    for (unsigned int j = 0; j < additionMask[i].size(); ++j) {
      if (additionMask[i][j]) {
        guint8 *pixel = pixels + i * channels + j * stride;
        filterAdditionBuffer[pixel[0]][pixel[1]][pixel[2]] = true;
      }
      if (removalMask[i][j]) {
        guint8 *pixel = pixels + i * channels + j * stride;
        filterRemovalBuffer[pixel[0]][pixel[1]][pixel[2]] = true;
      }
    }
  }

  // Redraw
  queue_draw();
}

void ImageAfterDrawingArea::addBufferToFilter() { // TODO: Optimise speed (maybe add buffer values to vectors?)
  for (unsigned int i = 0; i < filter.size(); ++i) {
    for (unsigned int j = 0; j < filter[i].size(); ++j) {
      for (unsigned int k = 0; k < filter[i][j].size(); ++k) {
        if (filterRemovalBuffer[i][j][k]) {
          filter[i][j][k] = false;
        }
        if (filterAdditionBuffer[i][j][k]) {
          filter[i][j][k] = true;
        }
      }
    }
  }

  resetFilterBuffers();

  // Redraw
  queue_draw();
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
  initialiseFilter();
  initialiseFilterBuffers();
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

void ImageAfterDrawingArea::initialiseFilterBuffers() {
  initialiseFilterAdditionBuffer();
  filterRemovalBuffer = filterAdditionBuffer;
}

void ImageAfterDrawingArea::initialiseFilterAdditionBuffer() {
  initialiseFilterMap(filterAdditionBuffer);
}

void ImageAfterDrawingArea::initialiseFilterRemovalBuffer() {
  initialiseFilterMap(filterRemovalBuffer);
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

void ImageAfterDrawingArea::resetFilterBuffers() {
  for (unsigned int i = 0; i < filterAdditionBuffer.size(); ++i) { // We know that the filter addition buffer and the filter removal buffer are the same size
    for (unsigned int j = 0; j < filterAdditionBuffer[i].size(); ++j) {
      for (unsigned int k = 0; k < filterAdditionBuffer[i][j].size(); ++k) {
        filterAdditionBuffer[i][j][k] = false;
        filterRemovalBuffer[i][j][k] = false;
      }
    }
  }
}

void ImageAfterDrawingArea::resetFilterAdditionBuffer() {
  resetFilterMap(filterAdditionBuffer);
}

void ImageAfterDrawingArea::resetFilterRemovalBuffer() {
  resetFilterMap(filterRemovalBuffer);
}

bool ImageAfterDrawingArea::applyFilter() {
  filteredImage = image->copy(); // TODO: Copy only where is necessary (?)
  guint8 *pixels = filteredImage->get_pixels();
  unsigned int channels = filteredImage->get_n_channels();
  unsigned int stride = filteredImage->get_rowstride();

  // Color pixels
  for (unsigned int i = 0; i < 640; ++i) {
    for (unsigned int j = 0; j < 480; ++j) {
      guint8 *pixel = pixels + i * channels + j * stride;
      if ((filter[pixel[0]][pixel[1]][pixel[2]] && !filterRemovalBuffer[pixel[0]][pixel[1]][pixel[2]]) || filterAdditionBuffer[pixel[0]][pixel[1]][pixel[2]]) {
        pixel[0] *= 0.3;
        pixel[1] *= 0.3;
        pixel[2] *= 0.3;
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
