/**
 * @file ImageDrawingArea.hpp
 * Color calibration application image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef GUI_IMAGE_DRAWING_AREA_H
#define GUI_IMAGE_DRAWING_AREA_H

#include <gtkmm/drawingarea.h>


namespace rtx {

  class MainWindow;

  class ImageDrawingArea: public Gtk::DrawingArea {

    public:
      ImageDrawingArea(MainWindow*);
      virtual ~ImageDrawingArea();

      bool isPlaying() const;

      void setPlaying(const bool& = true);

    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&) = 0;

      MainWindow *mainWindow;

  };

};

#endif // GUI_IMAGE_DRAWING_AREA_H
