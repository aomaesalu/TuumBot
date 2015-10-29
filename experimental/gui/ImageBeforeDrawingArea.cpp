/**
 * @file ImageDrawingArea.cpp
 * Experimental GUI color calibration application "before" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "ImageBeforeDrawingArea.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream>


ImageBeforeDrawingArea::ImageBeforeDrawingArea(Gtk::Scale *brushScale):
  ImageDrawingArea()
{
  initialiseProperties();
  initialiseImage();
  initialiseBrush(brushScale);
  initialiseMask();
  initialiseDrawingModes();
}

ImageBeforeDrawingArea::~ImageBeforeDrawingArea() {
  // Nothing to do here
}

bool ImageBeforeDrawingArea::maskEmpty() const {
  for (std::vector<std::vector<bool>>::const_iterator i = mask.begin(); i != mask.end(); ++i) {
    for (std::vector<bool>::const_iterator j = (*i).begin(); j != (*i).end(); ++j) {
      if (*j) {
        return false;
      }
    }
  }
  return true;
}

bool ImageBeforeDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
  if (!drawImage(cairo))
    return false;

  if (!applyMask())
    return false;

  return true;
}

bool ImageBeforeDrawingArea::on_button_press_event(GdkEventButton *buttonEvent) {
  if (buttonEvent->type == GDK_BUTTON_PRESS) {
    setPlaying(false);
    if (buttonEvent->button == 1) { // Left mouse button
      if (!erasingMode) {
        addingMode = true;
        addToMask(buttonEvent->x, buttonEvent->y);
      }
    } else if (buttonEvent->button == 3) { // Right mouse button
      if (!addingMode) {
        erasingMode = true;
        eraseFromMask(buttonEvent->x, buttonEvent->y);
      }
    }
  }
  std::cout << "Press" << std::endl;
  return true;
}

bool ImageBeforeDrawingArea::on_button_release_event(GdkEventButton *buttonEvent) {
  if (buttonEvent->type == GDK_BUTTON_RELEASE) {
    if (buttonEvent->button == 1) { // Left mouse button
      if (addingMode) {
        addingMode = false;
      }
    } else if (buttonEvent->button == 3) { // Right mouse button
      if (erasingMode) {
        erasingMode = false;
      }
    }
  }
  std::cout << "Release" << std::endl;
  return true;
}

bool ImageBeforeDrawingArea::on_motion_notify_event(GdkEventMotion *motionEvent) {
  if (!drawBrush(motionEvent->x, motionEvent->y))
    return false;
  if (addingMode) {
    addToMask(motionEvent->x, motionEvent->y);
  } else if (erasingMode) {
    eraseFromMask(motionEvent->x, motionEvent->y);
  }
  return true;
}

bool ImageBeforeDrawingArea::on_scroll_event(GdkEventScroll *scrollEvent) {
  if (scrollEvent->direction == GDK_SCROLL_UP) {
    if (brushScale->get_value() < brushScale->get_adjustment()->property_upper()) {
      brushScale->set_value(brushScale->get_value() + brushScale->get_adjustment()->get_step_increment());
    }
  } else if (scrollEvent->direction == GDK_SCROLL_DOWN) {
    if (brushScale->get_value() > brushScale->get_adjustment()->property_lower()) {
      brushScale->set_value(brushScale->get_value() - brushScale->get_adjustment()->get_step_increment());
    }
  }
  return true;
}

void ImageBeforeDrawingArea::initialiseProperties() {
  set_size_request(640, 480);
}

void ImageBeforeDrawingArea::initialiseImage() {
  image = Gdk::Pixbuf::create_from_file("frame.ppm"); // TODO: Remove association with files

  // Show the whole image
  if (image)
    set_size_request(image->get_width(), image->get_height());
}

void ImageBeforeDrawingArea::initialiseBrush(Gtk::Scale *brushScale) {
  this->brushScale = brushScale;
  // TODO: Different brush types, precalculate relative pixels
}

void ImageBeforeDrawingArea::initialiseMask() {
  mask.clear();
  std::vector<bool> row(image->get_width(), false);
  for (int i = 0; i < image->get_height(); ++i) {
    mask.push_back(row);
  }
}

void ImageBeforeDrawingArea::initialiseDrawingModes() {
  addingMode = false;
  erasingMode = false;
}

bool ImageBeforeDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
  if (!image)
    return false;

  Gdk::Cairo::set_source_pixbuf(cairo, image, 0, 0);
  cairo->paint();

  return true;
}

bool ImageBeforeDrawingArea::drawBrush(const unsigned int &x, const unsigned int &y) {
  guint8 *pixels = image->get_pixels();
  unsigned int channels = image->get_n_channels();
  unsigned int stride = image->get_rowstride();

  // Color pixels
  unsigned int radiusSquared = (brushScale->get_value() / 2) * (brushScale->get_value() / 2);
  for (unsigned int i = 0; i < brushScale->get_value(); ++i) {
    for (unsigned int j = 0; j < brushScale->get_value(); ++j) {
      unsigned int dx = i - x;
      unsigned int dy = j - y;
      unsigned int distanceSquared = dx * dx + dy * dy;
      if (distanceSquared <= radiusSquared) {
        guint8 *pixel = pixels + i * channels + j * stride;
        pixel[0] *= 2;
      }
    }
  }

  // Redraw
  queue_draw();

  return true;
}

bool ImageBeforeDrawingArea::applyMask() {
  guint8 *pixels = image->get_pixels();
  unsigned int channels = image->get_n_channels();
  unsigned int stride = image->get_rowstride();

  // Color pixels
  for (unsigned int i = 0; i < mask.size(); ++i) {
    for (unsigned int j = 0; j < mask.size(); ++j) {
      if (mask[i][j]) {
        guint8 *pixel = pixels + i * channels + j * stride;
        pixel[0] *= 0.5;
        pixel[1] *= 0.5;
        pixel[2] *= 0.5;
      }
    }
  }

  // Redraw
  queue_draw();

  return true;
}

void ImageBeforeDrawingArea::addToMask(const unsigned int &x, const unsigned int &y) {
  std::cout << "Add to mask: " << x << " " << y << std::endl; // TODO: Remove
  changeValueInMask(x, y, true);
}

void ImageBeforeDrawingArea::eraseFromMask(const unsigned int &x, const unsigned int &y) {
  std::cout << "Add to mask: " << x << " " << y << std::endl; // TODO: Remove
  changeValueInMask(x, y, false);
}

void ImageBeforeDrawingArea::changeValueInMask(const unsigned int &x, const unsigned int &y, const bool &value) {
  unsigned int radiusSquared = (brushScale->get_value() / 2) * (brushScale->get_value() / 2);
  for (unsigned int i = 0; i < brushScale->get_value(); ++i) {
    for (unsigned int j = 0; j < brushScale->get_value(); ++j) {
      unsigned int dx = i - x;
      unsigned int dy = j - y;
      unsigned int distanceSquared = dx * dx + dy * dy;
      if (distanceSquared <= radiusSquared) {
        mask[i][j] = value;
      }
    }
  }
}
