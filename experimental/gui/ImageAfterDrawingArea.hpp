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
#include <gtkmm.h>
#include <gdkmm/pixbuf.h>


class ImageAfterDrawingArea: public ImageDrawingArea {

  public:
    ImageAfterDrawingArea(Gtk::Scale*);
    virtual ~ImageAfterDrawingArea();

  protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
    virtual bool on_scroll_event(GdkEventScroll*);

  private:
    Glib::RefPtr<Gdk::Pixbuf> image;

    Gtk::Scale *deltaScale;

};

#endif // GUI_IMAGE_AFTER_DRAWING_AREA_H
