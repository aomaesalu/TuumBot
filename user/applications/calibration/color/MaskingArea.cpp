/**
 *  @file MaskingArea.cpp
 *  Color calibration application masking area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#include "MaskingArea.hpp"

#include "cameraConstants.hpp"
#include "Application.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>


namespace rtx {

  MaskingArea::MaskingArea(Application *application) {
    // Set properties
    this->application = application;

    // Initialise area size
    set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);

    // Initialise brush
    brush = new Brush(application->getWindow()->getBrushScale());

    // Initialise drawing modes
    resetDrawingModes();
  }

  MaskingArea::~MaskingArea() {
    // TODO
  }

  void MaskingArea::resetDrawingModes() {
    addingMode = false;
    removalMode = false;
  }

  void MaskingArea::redraw() {
    // TODO: maximiseMaskBoundaries();
    applyMask();
    queue_draw();
    // TODO: initialiseMaskBoundaries();
  }

  bool MaskingArea::applyMask() {
    // Note: We currently only apply addition mask
    Mask* mask = application->getMask();

    application->getImage()->copy_area(mask->getMinX(), mask->getMinY(), mask->getMaxX() - mask->getMinX() + 1, mask->getMaxY() - mask->getMinY() + 1, maskedImage, mask->getMinX(), mask->getMinY());

    unsigned int mode = application->getMode();

    guint8 *pixels = maskedImage->get_pixels();
    unsigned int channels = maskedImage->get_n_channels();
    unsigned int stride = maskedImage->get_rowstride();

    // Color pixels
    for (unsigned int x = 0; x < CAMERA_WIDTH; ++x) {
      for (unsigned int y = 0; y < CAMERA_HEIGHT; ++y) {
        if (x >= mask->getMinX() && x <= mask->getMaxX() && y >= mask->getMinY() && y <= mask->getMaxY() && mask->has(mode, x, y)) {
          guint8 *pixel = pixels + x * channels + y * stride;
          for (unsigned int k = 0; k < channels; ++k) {
            pixel[k] = 255 - pixel[k];
          }
        }
      }
    }

    return true;
  }

  bool MaskingArea::locateBrush(const unsigned int &x, const unsigned int &y) {
    // Set new coordinates to the brush
    brush->setX(x);
    brush->setY(y);

    // Redraw
    queue_draw();

    return true;
  }

  bool MaskingArea::applyBrush() {
    // TODO: Optimise copying (only the necessary area! Maybe should save brush mask matrix, too.)
    brushedImage = maskedImage->copy();
    guint8 *pixels = brushedImage->get_pixels();
    unsigned int channels = brushedImage->get_n_channels();
    unsigned int stride = brushedImage->get_rowstride();

    // Color pixels
    unsigned int radius = brush->getRadius();
    unsigned int radiusSquared = radius * radius;
    for (int x = -radius; x < radius; ++x) {
      for (int y = -radius; y < radius; ++y) {
        unsigned int distanceSquared = x * x + y * y;
        if (distanceSquared <= radiusSquared) {
          unsigned int currentX = brush->getX() + x;
          unsigned int currentY = brush->getY() + y;
          // If the value overflows, it's already smaller than the maximum value
          if (currentX < CAMERA_WIDTH && currentY < CAMERA_HEIGHT) {
            guint8 *pixel = pixels + currentX * channels + currentY * stride;
            pixel[1] *= 0.5;
          }
        }
      }
    }

    return true;
  }

  bool MaskingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!brushedImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, brushedImage, 0, 0);

    return true;
  }

  bool MaskingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (application->isMasking() && !applyMask())
      return false;

    if (!applyBrush())
      return false;

    if (!drawImage(cairo))
      return false;

    cairo->paint();

    return true;
  }

  bool MaskingArea::on_button_press_event(GdkEventButton *buttonEvent) {
    if (buttonEvent->type == GDK_BUTTON_PRESS) {
      application->setMasking();
      // TODO: initialiseMaskBoundaries();
      // Left mouse button
      if (buttonEvent->button == 1) {
        if (!removalMode) {
          addingMode = true;
          application->getMask()->add(buttonEvent->x, buttonEvent->y, application->getMode(), brush->getRadius());
        }
      // Right mouse button
      } else if (buttonEvent->button == 3) {
        if (!addingMode) {
          removalMode = true;
          application->getMask()->remove(buttonEvent->x, buttonEvent->y, application->getMode(), brush->getRadius());
        }
      }
    }
    return true;
  }

  bool MaskingArea::on_button_release_event(GdkEventButton *buttonEvent) {
    if (buttonEvent->type == GDK_BUTTON_RELEASE) {
      if (buttonEvent->button == 1) { // Left mouse button
        if (addingMode) {
          addingMode = false;
        }
      } else if (buttonEvent->button == 3) { // Right mouse button
        if (removalMode) {
          removalMode = false;
        }
      }
      application->sendMasksToFilter();
      // TODO: initialiseMaskBoundaries();
    }
    return true;
  }

  bool MaskingArea::on_motion_notify_event(GdkEventMotion *motionEvent) {
    if (!locateBrush(motionEvent->x, motionEvent->y))
      return false;
    if (addingMode) {
      application->getMask()->add(motionEvent->x, motionEvent->y, application->getMode(), brush->getRadius());
    } else if (removalMode) {
      application->getMask()->remove(motionEvent->x, motionEvent->y, application->getMode(), brush->getRadius());
    }
    return true;
  }

  bool MaskingArea::on_scroll_event(GdkEventScroll *scrollEvent) {
    if (scrollEvent->direction == GDK_SCROLL_UP) {
      if (brush->getSize() < brush->getUpperLimit()) {
        brush->incrementSize();
      }
    } else if (scrollEvent->direction == GDK_SCROLL_DOWN) {
      if (brush->getSize() > brush->getLowerLimit()) {
        brush->decrementSize();
      }
    }
    locateBrush(scrollEvent->x, scrollEvent->y);
    return true;
  }

}
