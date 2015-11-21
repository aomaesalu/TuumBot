/**
 *  @file PreviewArea.cpp
 *  Color calibration application preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 17 November 2015
 */

#include "PreviewArea.hpp"

#include "cameraConstants.hpp"
#include "Application.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

  PreviewArea::PreviewArea(Application* application) {
    // Set properties
    this->application = application;
  }

  PreviewArea::~PreviewArea() {
    // TODO
  }

  void PreviewArea::initialise() {
    // Initialise area size
    // set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
    std::cout << "F1" << std::endl;
    if (application->getImage()) {
      std::cout << "Preview Area +" << std::endl;
      set_size_request(application->getImage()->get_width(), application->getImage()->get_height());
    } else {
      std::cout << "Preview Area -" << std::endl;
    }
    std::cout << "F2" << std::endl;

    // Initialise delta scale // TODO: Separate from this class
    deltaScale = application->getWindow()->getDeltaScale();
    std::cout << "F3" << std::endl;
  }

  bool PreviewArea::applyFilter() {
    std::cout << "PreviewArea applyFilter started" << std::endl;

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
        if (filter->has(mode, actualPixel[0], actualPixel[1], actualPixel[2])) {
          pixel[0] *= 0.2;
          pixel[1] *= 0.2;
          pixel[2] *= 0.2;
        }
      }
    }

    std::cout << "PreviewArea applyFilter ended" << std::endl;

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
    std::cout << "L1" << std::endl;
    if (scrollEvent->direction == GDK_SCROLL_UP) {
      std::cout << "L2" << std::endl;
      if (deltaScale->get_value() < deltaScale->get_adjustment()->property_upper()) {
        std::cout << "L3" << std::endl;
        deltaScale->set_value(deltaScale->get_value() + deltaScale->get_adjustment()->get_step_increment());
        std::cout << "L4" << std::endl;
      }
      std::cout << "L5" << std::endl;
    } else if (scrollEvent->direction == GDK_SCROLL_DOWN) {
      std::cout << "L6" << std::endl;
      if (deltaScale->get_value() > deltaScale->get_adjustment()->property_lower()) {
        std::cout << "L7" << std::endl;
        deltaScale->set_value(deltaScale->get_value() - deltaScale->get_adjustment()->get_step_increment());
        std::cout << "L8" << std::endl;
      }
      std::cout << "L9" << std::endl;
    }
    std::cout << "L10" << std::endl;
    return true;
  }

}
