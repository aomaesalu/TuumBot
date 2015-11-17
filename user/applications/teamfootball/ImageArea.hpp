/**
 *  @file ImageArea.hpp
 *  Team football application GUI preview area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#ifndef RTX_APPLIXATIONS_TEAMFOOTBALL_IMAGE_AREA_H
#define RTX_APPLIXATIONS_TEAMFOOTBALL_IMAGE_AREA_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>


namespace rtx {

  class Interface;

  class ImageArea: public Gtk::DrawingArea {

    public:
      ImageArea(Application*);
      virtual ~ImageArea();

      void initialise();

    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);

    private:
      Interface *interface;

      Glib::RefPtr<Gdk::Pixbuf> filteredImage;

      bool applyFilter();
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

  };

}

#endif // RTX_APPLIXATIONS_TEAMFOOTBALL_IMAGE_AREA_H
