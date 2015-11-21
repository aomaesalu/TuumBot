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

    std::vector<std::vector<std::pair<unsigned int, unsigned int>>> points = gui->getCheckerboard()->getPoints();

    guint8 *pixels = filteredImage->get_pixels();
    unsigned int channels = filteredImage->get_n_channels();
    unsigned int stride = filteredImage->get_rowstride();

    // Define RGB color
    unsigned int r = 255;
    unsigned int g = 0;
    unsigned int b = 0;

    // Color point pixels
    for (std::vector<std::vector<std::pair<unsigned int, unsigned int>>>::iterator pointList = points.begin(); pointList != points.end(); ++pointList) {
      for (std::vector<std::pair<unsigned int, unsigned int>>::iterator point = pointList->begin(); point != pointList->end(); ++point) {
        for (int dx = -1; dx < 1; ++dx) {
          for (int dy = -1; dy < 1; ++dy) {
            if (point->first + dx >= CAMERA_WIDTH || point->second + dy >= CAMERA_HEIGHT)
              continue;
            colorPixel(pixels + (point->first + dx) * channels + (point->second + dy) * stride, r, g, b);
          }
        }
      }
    }

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
        if (!gui->getCheckerboard()->isFilled()) {
          gui->getCheckerboard()->addPoint(buttonEvent->x, buttonEvent->y);
        } else {
          std::cout << "Cannot add new point! Checkerboard is already filled." << std::endl;
        }
      }
    }
    return true;
  }

}
