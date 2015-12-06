/**
 *  @file ImageArea.cpp
 *  Vision testing application GUI preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
 */

#include "ImageArea.hpp"

#include "cameraConstants.hpp"
#include "GUI.hpp"

#include "Perspective.hpp"
#include "tuum_physics.hpp"

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

  void colorPixel(guint8 *pixel, const Vision::Color &color) {
    unsigned int r = 0, g = 0, b = 0;
    getRGB(color, r, g, b);
    colorPixel(pixel, r, g, b);
  }

  void ImageArea::colorBlob(const Vision::Blob *blob, guint8 *pixels, const unsigned int &channels, const unsigned int &stride) {
    // Get blob parameters
    unsigned int x = blob->getCentroid()->getX();
    unsigned int y = blob->getCentroid()->getY();
    unsigned int minX = blob->getMinX();
    unsigned int maxX = blob->getMaxX();
    unsigned int minY = blob->getMinY();
    unsigned int maxY = blob->getMaxY();
    Vision::Color color = blob->getColor();

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

  void ImageArea::colorLine(const Vision::Line *line, guint8 *pixels, const unsigned int &channels, const unsigned int &stride) {
    std::pair<double, double> point = line->getRelativePoint();
    double slope = line->getSlope();

    // Color the line
    for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
      double realY = Vision::Perspective::virtualToReal(0, y).second;
      double realX = (realY - point.second) / slope + point.first;
      unsigned int x = Vision::Perspective::realToVirtual(realX, realY).first;
      if (x >= CAMERA_WIDTH)
        continue;
      guint8 *pixel = pixels + x * channels + y * stride;
      colorPixel(pixel, 102, 0, 51);
    }

    /*for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
      double realX = Vision::Perspective::virtualToReal(x, 0).first;
      double realY = slope * (realX + point.first) + point.second;
      unsigned int y = Vision::Perspective::realToVirtual(realX, realY).second;
      if (y >= CAMERA_HEIGHT)
        continue;
      guint8 *pixel = pixels + x * channels + y * stride;
      colorPixel(pixel, 102, 0, 51);
    }*/

    // Color the transition points // TODO: Remove (from the Line class, too)
    std::vector<std::pair<double, double>> linePoints = line->getPoints();
    for (std::vector<std::pair<double, double>>::iterator point = linePoints.begin(); point != linePoints.end(); ++point) {
      std::pair<unsigned int, unsigned int> vPoint = Vision::Perspective::realToVirtual(*point);
      if (vPoint.first >= CAMERA_WIDTH || vPoint.second >= CAMERA_HEIGHT)
        continue;
      for (int dx = -2; dx <= 2; ++dx) {
        if (vPoint.first + dx >= CAMERA_WIDTH)
          continue;
        for (int dy = -2; dy <= 2; ++dy) {
          if (vPoint.second >= CAMERA_HEIGHT)
            continue;
          guint8 *pixel = pixels + (vPoint.first + dx) * channels + (vPoint.second + dy) * stride;
          colorPixel(pixel, 102, 51, 0);
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

    Vision::BlobSet blobs = Vision::getBlobs();

    // Draw rectangles
    // DEBUG: std::cout << "Blobs in GUI: " << Vision::blobs.size() << std::endl;
    for (Vision::BlobSet::iterator blob = blobs.begin(); blob != blobs.end(); ++blob) {
      if (!(*blob))
        continue;

      unsigned int minX = (*blob)->getMinX();
      unsigned int maxX = (*blob)->getMaxX();
      unsigned int minY = (*blob)->getMinY();
      unsigned int maxY = (*blob)->getMaxY();
      if (minX >= CAMERA_WIDTH || maxX >= CAMERA_WIDTH || minY >= CAMERA_HEIGHT || maxY >= CAMERA_HEIGHT)
        continue;

      //std::cout << (*blob)->getCentroid()->getX() << " " << (*blob)->getCentroid()->getY() << " " << minX << " " << maxX << " " << minY << " " << maxY << std::endl;

      double density = (*blob)->getDensity();
      unsigned int boxArea = (*blob)->getBoxArea();

      if (density > 1.0 || boxArea > CAMERA_WIDTH * CAMERA_HEIGHT)
        continue;

      Vision::Color color = (*blob)->getColor();

      if (color == Vision::BALL/* && density > 0.6*/ && boxArea > 4 * 4) {
        colorBlob(*blob, pixels, channels, stride);
      }

      if (color == Vision::BLUE_GOAL/* && boxArea > 30 * 30*/) {
        colorBlob(*blob, pixels, channels, stride);
      }

      if (color == Vision::YELLOW_GOAL/* && boxArea > 30 * 30*/) {
        colorBlob(*blob, pixels, channels, stride);
      }

      if (color == Vision::ROBOT_YELLOW_BLUE) {
        colorBlob(*blob, pixels, channels, stride);
      }

      if (color == Vision::ROBOT_BLUE_YELLOW) {
        colorBlob(*blob, pixels, channels, stride);
      }

    }

    Vision::LineSet lines = Vision::getLines();

    for (Vision::LineSet::iterator line = lines.begin(); line != lines.end(); ++line) {
      if (!(*line))
        continue;

      colorLine(*line, pixels, channels, stride);
    }

    // DEBUG
    // Analyse ray casting
    Entity *entity = Physics::rayCast(0, BALL_DIAMETER);
    if (entity != nullptr) {
      std::cout << "Found " << intToColor(entity->getColor()) << " at (" << entity->getTransform()->getX() << ", " << entity->getTransform()->getY() << ")" << std::endl;
    } else {
      std::cout << "No entity found in the way" << std::endl;
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
