/**
 *  @file ImageArea.cpp
 *  Team football application GUI preview area.
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

  void ImageArea::colorBlob(const Blob *blob, guint8 *pixels, const unsigned int &channels, const unsigned int &stride) {
    // Get blob parameters
    unsigned int x = blob->getPosition()->getX();
    unsigned int y = blob->getPosition()->getY();
    unsigned int minX = blob->getMinX();
    unsigned int maxX = blob->getMaxX();
    unsigned int minY = blob->getMinY();
    unsigned int maxY = blob->getMaxY();
    Color color = blob->getColor();

    // Get color parameters
    unsigned int r = 0, g = 0, b = 0;
    getRGB(color, r, g, b);

    // Color blob pixels
    const std::vector<std::pair<unsigned int, unsigned int>> points = blob->getPoints();
    for (std::vector<std::pair<unsigned int, unsigned int>>::const_iterator point = points.begin(); point != points.end(); ++point) {
      colorPixel(pixels + point->first * channels + point->second * stride, r, g, b);
    }

    // Color blob box area
    for (unsigned int i = minX; i <= maxX; ++i) {
      guint8 *pixel = pixels + i * channels + minY * stride;
      colorPixel(pixel, r, g, b);
      pixel = pixels + i * channels + maxY * stride;
      colorPixel(pixel, r, g, b);
    }
    for (unsigned int j = minY; j <= maxY; ++j) {
      guint8 *pixel = pixels + minX * channels + j * stride;
      colorPixel(pixel, r, g, b);
      pixel = pixels + maxX * channels + j * stride;
      colorPixel(pixel, r, g, b);
    }

    // Color center
    for (int dx = -3; dx < 3; ++dx) {
      for (int dy = -3; dy < 3; ++dy) {
        if (x + dx < CAMERA_WIDTH && x + dx >= 0 && y + dy < CAMERA_HEIGHT && y + dy >= 0) {
          guint8 *pixel = pixels + (x + dx) * channels + (y + dy) * stride;
          pixel[0] = 255;
          pixel[1] = 0;
          pixel[2] = 0;
        }
      }
    }
  }

  bool ImageArea::applyFilter() {
    filteredImage = gui->getImage()->copy(); // TODO: Copy only where is necessary (?)

    guint8 *pixels = filteredImage->get_pixels();
    unsigned int channels = filteredImage->get_n_channels();
    unsigned int stride = filteredImage->get_rowstride();

    guint8 *actualPixels = gui->getFrame()->data;
    unsigned int actualChannels = 3;
    unsigned int actualStride = gui->getFrame()->width * actualChannels;

    Vision::BlobSet blobs = Vision::blobs;
    while (Vision::editingBlobs) {
      blobs = Vision::blobs;
    }

    // Draw rectangles
    // DEBUG: std::cout << "Blobs in GUI: " << Vision::blobs.size() << std::endl;
    for (Vision::BlobSet::iterator blob = blobs.begin(); blob != blobs.end(); ++blob) {
      if (*blob) {
        unsigned int minX = (*blob)->getMinX();
        unsigned int maxX = (*blob)->getMaxX();
        unsigned int minY = (*blob)->getMinY();
        unsigned int maxY = (*blob)->getMaxY();
        if (minX >= CAMERA_WIDTH || maxX >= CAMERA_WIDTH || minY >= CAMERA_HEIGHT || maxY >= CAMERA_HEIGHT)
          continue;

        //std::cout << (*blob)->getPosition()->getX() << " " << (*blob)->getPosition()->getY() << " " << minX << " " << maxX << " " << minY << " " << maxY << std::endl;

        double density = (*blob)->getDensity();
        unsigned int boxArea = (*blob)->getBoxArea();

        if (density > 1.0 || boxArea > CAMERA_WIDTH * CAMERA_HEIGHT)
          continue;

        Color color = (*blob)->getColor();

        if (color == BALL/* && density > 0.6*/ && boxArea > 10 * 10) {
          colorBlob(*blob, pixels, channels, stride);
        }

        if (color == BLUE_GOAL/* && boxArea > 30 * 30*/) {
          colorBlob(*blob, pixels, channels, stride);
        }

        if (color == YELLOW_GOAL/* && boxArea > 30 * 30*/) {
          colorBlob(*blob, pixels, channels, stride);
        }

        if (color == ROBOT_YELLOW_BLUE) {
          colorBlob(*blob, pixels, channels, stride);
        }

        if (color == ROBOT_BLUE_YELLOW) {
          colorBlob(*blob, pixels, channels, stride);
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

}
