/**
 * @file ImageDrawingArea.hpp
 * Color calibration application image drawing area.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_IMAGE_DRAWING_AREA_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_IMAGE_DRAWING_AREA_H

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

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_IMAGE_DRAWING_AREA_H
