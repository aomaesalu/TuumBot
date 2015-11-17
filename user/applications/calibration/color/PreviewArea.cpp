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
    std::cout << "G1" << std::endl;
    filteredImage = application->getImage()->copy(); // TODO: Copy only where is necessary (?)
    std::cout << "G2" << std::endl;

    unsigned int mode = application->getMode();
    std::cout << "G3" << std::endl;
    Filter *filter = application->getFilter();
    std::cout << "G4" << std::endl;

    guint8 *pixels = filteredImage->get_pixels();
    std::cout << "G5" << std::endl;
    unsigned int channels = filteredImage->get_n_channels();
    std::cout << "G6" << std::endl;
    unsigned int stride = filteredImage->get_rowstride();
    std::cout << "G7" << std::endl;

    guint8 *actualPixels = application->getFrame()->data;
    std::cout << "G8" << std::endl;
    unsigned int actualChannels = 3;
    std::cout << "G9" << std::endl;
    unsigned int actualStride = application->getFrame()->width * actualChannels;
    std::cout << "G10" << std::endl;

    // Color pixels
    for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
      for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
        std::cout << x << " " << y << std::endl;
        guint8 *pixel = pixels + x * channels + y * stride;
        std::cout << "H1" << std::endl;
        guint8 *actualPixel = actualPixels + x * actualChannels + y * actualStride;
        std::cout << "H2" << std::endl;
        std::cout << mode << std::endl;
        std::cout << "H3" << std::endl;
        std::cout << actualPixel[0] << std::endl;
        std::cout << "H4" << std::endl;
        if (filter->has(mode, actualPixel[0], actualPixel[1], actualPixel[2])) {
          std::cout << "H5" << std::endl;
          pixel[0] *= 0.2;
          pixel[1] *= 0.2;
          pixel[2] *= 0.2;
          std::cout << "H6" << std::endl;
        }
      }
    }
    std::cout << "G11" << std::endl;

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
