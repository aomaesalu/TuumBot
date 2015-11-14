/**
 *  @file PreviewArea.cpp
 *  Color calibration application preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#include "PreviewArea.hpp"

#include "cameraConstants.hpp"
#include "Application.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>


namespace rtx {

  PreviewArea::PreviewArea(Application*) {
    // Set properties
    this->application = application;

    // Initialise area size
    set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);

    // Initialise delta scale // TODO: Separate from this class
    deltaScale = application->getWindow()->getDeltaScale();
  }

  PreviewArea::~PreviewArea() {
    // TODO
  }

  bool PreviewArea::applyFilter() {
    filteredImage = application->getImage()->copy(); // TODO: Copy only where is necessary (?)

    unsigned int mode = application->getMode();
    Filter *filter = application->getFilter();

    guint8 *pixels = filteredImage->get_pixels();
    unsigned int channels = filteredImage->get_n_channels();
    unsigned int stride = filteredImage->get_rowstride();

    guint8 *actualPixels = application->getFrame()->data;
    unsigned int actualChannels = 3;
    unsigned int actualStride = application->getFrame()->width * actualChannels;

    // Color pixels
    for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
      for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
        guint8 *pixel = pixels + x * channels + y * stride;
        guint8 *actualPixel = actualPixels + x * actualChannels + y * actualStride;
        if (filter.has(mode, actualPixel[0], actualPixel[1], actualPixel[2])) {
          pixel[0] *= 0.2;
          pixel[1] *= 0.2;
          pixel[2] *= 0.2;
        }
      }
    }

    return true;
  }

  bool PreviewArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!filteredImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, filteredImage, 0, 0);

    return true;
  }

  bool PreviewArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!applyFilter())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  bool PreviewArea::on_scroll_event(GdkEventScroll *scrollEvent) {
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

}
