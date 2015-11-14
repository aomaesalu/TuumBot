/**
 *  @file PreviewArea.hpp
 *  Color calibration application preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#ifndef RTX_APPLIXATIONS_CALIBRATION_COLOR_PREVIEW_AREA_H
#define RTX_APPLIXATIONS_CALIBRATION_COLOR_PREVIEW_AREA_H

#include <gtkmm/drawingarea.h>


namespace rtx {

  class Application;

  class PreviewArea: public Gtk::DrawingArea {

    public:
      PreviewArea(Application*);
      virtual ~PreviewArea();

    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);
      virtual bool on_scroll_event(GdkEventScroll*);

    private:
      Application *application;

      Glib::RefPtr<Gdk::Pixbuf> filteredImage;

      Gtk::Scale *deltaScale;

      bool applyFilter();
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

  };

}

#endif // RTX_APPLIXATIONS_CALIBRATION_COLOR_PREVIEW_AREA_H
