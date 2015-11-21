/**
 *  @file ImageArea.cpp
 *  Perspective calibration application GUI preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#include "ImageArea.hpp"

#include "cameraConstants.hpp"
#include "GUI.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

  ImageArea::ImageArea(GUI *gui) {
    // Attach GUI interface information
    this->gui = gui;
  }

  ImageArea::~ImageArea() {
    // TODO
  }

  void ImageArea::initialise() {
    // Initialise area size
    set_size_request(gui->getImage()->get_width(), gui->getImage()->get_height());
  }

  void colorPixel(guint8 *pixel, const unsigned int &r, const unsigned int &g, const unsigned int &b) {
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
  }

  void colorPixel(guint8 *pixel, const Color &color) {
    unsigned int r = 0, g = 0, b = 0;
    getRGB(color, r, g, b);
    colorPixel(pixel, r, g, b);
  }

  bool ImageArea::applyFilter() {
    filteredImage = gui->getImage()->copy(); // TODO: Copy only where is necessary (?)

    guint8 *pixels = filteredImage->get_pixels();
    unsigned int channels = filteredImage->get_n_channels();
    unsigned int stride = filteredImage->get_rowstride();

    guint8 *actualPixels = gui->getFrame()->data;
    unsigned int actualChannels = 3;
    unsigned int actualStride = gui->getFrame()->width * actualChannels;

    // TODO

    return true;
  }

  bool ImageArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

  bool ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!applyFilter())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  bool ImageArea::on_button_press_event(GdkEventButton *buttonEvent) {
    if (buttonEvent->type == GDK_BUTTON_PRESS) {
      if (buttonEvent->button == 1) { // Left mouse button
        // TODO: Add point
      }
    }
    return true;
  }

}
