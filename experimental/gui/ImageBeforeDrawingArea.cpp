/**
 * @file ImageDrawingArea.cpp
 * Experimental GUI color calibration application "before" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "ImageBeforeDrawingArea.hpp"

#include "MainWindow.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream>


ImageBeforeDrawingArea::ImageBeforeDrawingArea(MainWindow *mainWindow, Gtk::Scale *brushScale):
  ImageDrawingArea(mainWindow),
  addingMode(false),
  erasingMode(false)
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

bool ImageBeforeDrawingArea::isMasking() const {
  return mainWindow->isMasking();
}

bool ImageBeforeDrawingArea::isMaskEmpty() const {
  for (std::vector<std::vector<bool>>::const_iterator i = mask.begin(); i != mask.end(); ++i) {
    for (std::vector<bool>::const_iterator j = (*i).begin(); j != (*i).end(); ++j) {
      if (*j) {
        return false;
      }
    }
  }
  return true;
}

void ImageBeforeDrawingArea::setPlaying(const bool &value) {
  if (value) {
    setMasking(false);
  }
  mainWindow->setPlaying(value);
}

void ImageBeforeDrawingArea::setMasking(const bool &value) {
  if (value) {
    if (!isMasking()) {
      setPlaying(false);
    }
  } else {
    initialiseMask(); // TODO: Fix lagging behaviour
    initialiseDrawingModes();
  }
  mainWindow->setMasking(value);
}

bool ImageBeforeDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
  if (isMasking() && !applyMask())
    return false;

  if (!applyBrush())
    return false;

  if (!drawImage(cairo))
    return false;

  cairo->paint();

  return true;
}

bool ImageBeforeDrawingArea::on_button_press_event(GdkEventButton *buttonEvent) {
  if (buttonEvent->type == GDK_BUTTON_PRESS) {
    setMasking();
    initialiseMaskBoundaries();
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
    initialiseMaskBoundaries();
  }
  return true;
}

bool ImageBeforeDrawingArea::on_motion_notify_event(GdkEventMotion *motionEvent) {
  if (!locateBrush(motionEvent->x, motionEvent->y))
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
  locateBrush(scrollEvent->x, scrollEvent->y);
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
  brushX = 0;
  brushY = 0;
  // TODO: Different brush types, precalculate relative pixels
}

void ImageBeforeDrawingArea::initialiseMask() {
  mask.clear();
  std::vector<bool> row(image->get_height(), false);
  for (int i = 0; i < image->get_width(); ++i) {
    mask.push_back(row);
  }
  mainWindow->setMasking(false);
  maskedImage = image->copy();
  brushedImage = maskedImage->copy();
  initialiseMaskBoundaries();
}

void ImageBeforeDrawingArea::initialiseMaskBoundaries() {
  maskMinX = 640;
  maskMinY = 480;
  maskMaxX = 0;
  maskMaxY = 0;
}

void ImageBeforeDrawingArea::initialiseDrawingModes() {
  addingMode = false;
  erasingMode = false;
}

bool ImageBeforeDrawingArea::applyMask() {
  image->copy_area(maskMinX, maskMinY, maskMaxX - maskMinX + 1, maskMaxY - maskMinY + 1, maskedImage, maskMinX, maskMinY);
  guint8 *pixels = maskedImage->get_pixels();
  unsigned int channels = maskedImage->get_n_channels();
  unsigned int stride = maskedImage->get_rowstride();

  // Color pixels
  for (unsigned int i = 0; i < mask.size(); ++i) {
    for (unsigned int j = 0; j < mask[i].size(); ++j) {
      if (i >= maskMinX && i <= maskMaxX && j >= maskMinY && j <= maskMaxY && mask[i][j]) {
        guint8 *pixel = pixels + i * channels + j * stride;
        pixel[0] *= 0.3;
        pixel[1] *= 0.3;
        pixel[2] *= 0.3;
      }
    }
  }

  return true;
}

bool ImageBeforeDrawingArea::locateBrush(const unsigned int &x, const unsigned int &y) {
  brushX = x;
  brushY = y;

  // Redraw
  queue_draw();

  return true;
}

bool ImageBeforeDrawingArea::applyBrush() {
  int radius = brushScale->get_value() / 2;
  unsigned int brushMinX = brushX - radius;
  if (brushMinX >= 640) {
    brushMinX = 0;
  }
  unsigned int brushMinY = brushY - radius;
  if (brushMinY >= 640) {
    brushMinY = 0;
  }
  unsigned int brushMaxX = brushX + radius;
  if (brushMaxX >= 640) {
    brushMaxX = 639;
  }
  unsigned int brushMaxY = brushY + radius;
  if (brushMaxY >= 480) {
    brushMaxY = 479;
  }

  // maskedImage->copy_area(brushMinX, brushMinY, brushMaxX - brushMinX + 1, brushMaxY - brushMinY + 1, brushedImage, brushMinX, brushMinY);
  brushedImage = maskedImage->copy(); // TODO: Optimised copying (only the necessary area! Maybe should save brush mask matrix, too.)
  guint8 *pixels = brushedImage->get_pixels();
  unsigned int channels = brushedImage->get_n_channels();
  unsigned int stride = brushedImage->get_rowstride();

  // Color pixels
  unsigned int radiusSquared = radius * radius;
  for (int i = -radius; i < radius; ++i) {
    for (int j = -radius; j < radius; ++j) {
      unsigned int distanceSquared = i * i + j * j;
      if (distanceSquared <= radiusSquared) {
        unsigned int currentX = brushX + i;
        unsigned int currentY = brushY + j;
        if (currentX < 640 && currentY < 480) { // If the value overflows, it's already smaller than the maximum value
          guint8 *pixel = pixels + currentX * channels + currentY * stride;
          pixel[1] *= 0.5;
        }
      }
    }
  }
  return true;
}

bool ImageBeforeDrawingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
  if (!image)
    return false;

  Gdk::Cairo::set_source_pixbuf(cairo, brushedImage, 0, 0);

  return true;
}

void ImageBeforeDrawingArea::addToMask(const unsigned int &x, const unsigned int &y) {
  changeValueInMask(x, y, true);
}

void ImageBeforeDrawingArea::eraseFromMask(const unsigned int &x, const unsigned int &y) {
  changeValueInMask(x, y, false);
}

// TODO: Smooth behaviour on fast movements - calculate positions inbetween based on movement history
void ImageBeforeDrawingArea::changeValueInMask(const unsigned int &x, const unsigned int &y, const bool &value) {
  unsigned int brushSize = brushScale->get_value();
  int radius = brushSize / 2;
  unsigned int radiusSquared = radius * radius;
  for (int i = -radius; i < radius; ++i) {
    for (int j = -radius; j < radius; ++j) {
      unsigned int distanceSquared = i * i + j * j;
      if (distanceSquared <= radiusSquared) {
        unsigned int currentX = x + i;
        unsigned int currentY = y + j;
        if (currentX < 640 && currentY < 480) { // If the value overflows, it's already smaller than the maximum value
          mask[currentX][currentY] = value;
          if (currentX < maskMinX) {
            maskMinX = currentX;
          }
          if (currentY < maskMinY) {
            maskMinY = currentY;
          }
          if (currentX > maskMaxX) {
            maskMaxX = currentX;
          }
          if (currentY > maskMaxY) {
            maskMaxY = currentY;
          }
        }
      }
    }
  }

  // Redraw
  queue_draw();
}
