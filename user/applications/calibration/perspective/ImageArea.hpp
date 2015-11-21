/**
 *  @file ImageArea.hpp
 *  Perspective calibration application GUI preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#ifndef RTX_APPLIXATIONS_CALIBRATION_PERSPECTIVE_IMAGE_AREA_H
#define RTX_APPLIXATIONS_CALIBRATION_PERSPECTIVE_IMAGE_AREA_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

#include "Color.hpp"


namespace rtx {

  class GUI;

  class ImageArea: public Gtk::DrawingArea {

    public:
      ImageArea(GUI*);
      virtual ~ImageArea();

      void initialise();

    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
      virtual bool on_button_press_event(GdkEventButton *buttonEvent);

    private:
      GUI *gui;

      Glib::RefPtr<Gdk::Pixbuf> filteredImage;

      bool applyFilter();
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

  };

}

#endif // RTX_APPLIXATIONS_CALIBRATION_PERSPECTIVE_IMAGE_AREA_H
