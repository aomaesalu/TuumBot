/**
 * @file ImageDrawingArea.cpp
 * Experimental GUI color calibration application "after" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "ImageAfterDrawingArea.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>


ImageAfterDrawingArea::ImageAfterDrawingArea():
  ImageDrawingArea()
{
  image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files

  // Show the whole image
  if (image)
    set_size_request(image->get_width(), image->get_height());
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
