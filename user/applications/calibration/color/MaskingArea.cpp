/**
 *  @file MaskingArea.cpp
 *  Color calibration application masking area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 17 November 2015
 */

#include "MaskingArea.hpp"

#include "cameraConstants.hpp"
#include "Application.hpp"

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <glibmm/fileutils.h>

#include <iostream> // TODO: Remove


namespace rtx {

  MaskingArea::MaskingArea(Application *application) {
    // Set properties
    this->application = application;
  }

  MaskingArea::~MaskingArea() {
    // TODO
  }

  void MaskingArea::initialise() {
    // Initialise area size
    //set_size_request(CAMERA_WIDTH, CAMERA_HEIGHT);
    std::cout << "E1" << std::endl;
    if (application->getImage()) {
      std::cout << "Masking Area +" << std::endl;
      set_size_request(application->getImage()->get_width(), application->getImage()->get_height());
    } else {
      std::cout << "Masking Area -" << std::endl;
    }
    std::cout << "E2" << std::endl;

    // Initialise brush
    brush = new Brush(application->getWindow()->getBrushScale());
    std::cout << "E3" << std::endl;

    // Initialise images
    maskedImage = application->getImage()->copy();
    brushedImage = maskedImage->copy();
    std::cout << "E4" << std::endl;

    // Initialise drawing modes
    resetDrawingModes();
    std::cout << "E6" << std::endl;
  }

  void MaskingArea::resetDrawingModes() {
    addingMode = false;
    removalMode = false;
  }

  void MaskingArea::redraw() {
    std::cout << "MaskingArea->redraw()" << std::endl;
    // TODO: maximiseMaskBoundaries();
    applyMask();
    queue_draw();
    // TODO: initialiseMaskBoundaries();
  }

  bool MaskingArea::applyMask() {
    std::cout << "MaskingArea applyMask started" << std::endl;

    // Note: We currently only apply addition mask
    Mask* mask = application->getMask();

    if (application->getImage()) {
      std::cout << "MaskingArea applyMask image +" << std::endl;
    } else {
      std::cout << "MaskingArea applyMask image -" << std::endl;
    }

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
    std::cout << "MaskingArea applyMask ended" << std::endl;

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
    std::cout << "MaskingArea applyBrush started" << std::endl;
    std::cout << "N1" << std::endl;
    // TODO: Optimise copying (only the necessary area! Maybe should save brush mask matrix, too.)
    brushedImage = maskedImage->copy();
    std::cout << "N2" << std::endl;
    guint8 *pixels = brushedImage->get_pixels();
    std::cout << "N3" << std::endl;
    unsigned int channels = brushedImage->get_n_channels();
    std::cout << "N4" << std::endl;
    unsigned int stride = brushedImage->get_rowstride();
    std::cout << "N5" << std::endl;

    // Color pixels
    unsigned int radius = brush->getRadius();
    std::cout << "N6" << std::endl;
    unsigned int radiusSquared = radius * radius;
    std::cout << "N7" << std::endl;
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
    std::cout << "N8" << std::endl;

    std::cout << "MaskingArea applyBrush ended" << std::endl;
    return true;
  }

  bool MaskingArea::drawImage(const Cairo::RefPtr<Cairo::Context> &cairo) {
    if (!brushedImage)
      return false;

    Gdk::Cairo::set_source_pixbuf(cairo, brushedImage, 0, 0);

    return true;
  }

  bool MaskingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cairo) {
    std::cout << "MaskingArea on_draw started" << std::endl;
    std::cout << "M1" << std::endl;
    if (application->isMasking() && !applyMask()) {
      std::cout << "M2" << std::endl;
      return false;
    }
    std::cout << "M3" << std::endl;

    if (!applyBrush()) {
      std::cout << "M4" << std::endl;
      return false;
    }
    std::cout << "M5" << std::endl;

    if (!drawImage(cairo)) {
      std::cout << "M6" << std::endl;
      return false;
    }
    std::cout << "M7" << std::endl;

    cairo->paint();
    std::cout << "M8" << std::endl;

    std::cout << "MaskingArea on_draw ended" << std::endl;
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
