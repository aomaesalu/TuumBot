/**
 * @file ImageDrawingArea.hpp
 * Experimental GUI color calibration application "after" image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_IMAGE_AFTER_DRAWING_AREA_H
#define GUI_IMAGE_AFTER_DRAWING_AREA_H

#include "ImageDrawingArea.hpp"


class ImageAfterDrawingArea: public ImageDrawingArea {

  public:
    ImageAfterDrawingArea();
    virtual ~ImageAfterDrawingArea();

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;

};

#endif // GUI_IMAGE_AFTER_DRAWING_AREA_H
