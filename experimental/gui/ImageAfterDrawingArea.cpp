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
  initialiseFilterBuffer();
}

void ImageAfterDrawingArea::initialiseFilter() {
  filter.clear();
  std::map<unsigned int, bool> subSubFilter;
  for (unsigned int i = 0; i < 256; ++i) {
    subSubFilter[i] = false;
  }
  std::map<unsigned int, std::map<unsigned int, bool>> subFilter;
  for (unsigned int i = 0; i < 256; ++i) {
    subFilter[i] = subSubFilter;
  }
  for (unsigned int i = 0; i < 256; ++i) {
    filter[i] = subFilter;
  }
}

void ImageAfterDrawingArea::initialiseFilterBuffer() {
  filterBuffer.clear();
  std::map<unsigned int, bool> subSubFilterBuffer;
  for (unsigned int i = 0; i < 256; ++i) {
    subSubFilterBuffer[i] = false;
  }
  std::map<unsigned int, std::map<unsigned int, bool>> subFilterBuffer;
  for (unsigned int i = 0; i < 256; ++i) {
    subFilterBuffer[i] = subSubFilterBuffer;
  }
  for (unsigned int i = 0; i < 256; ++i) {
    filterBuffer[i] = subFilterBuffer;
  }
}
