/**
 *  @file ImageDrawingArea.hpp
 *  Perspective calibration application image drawing area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 18 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_IMAGE_DRAWING_AREA_H
#define RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_IMAGE_DRAWING_AREA_H

#include <gtkmm.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

#include <map>

#include "Camera.hpp"
#include "tuum_visioning.hpp"
#include "rtxvision.h"


namespace rtx {

  class MainWindow;

  class ImageDrawingArea: public Gtk::DrawingArea {

    public:
      ImageDrawingArea(MainWindow*);
      virtual ~ImageDrawingArea();

      bool isPlaying() const;
      bool isCalculating() const;

      void setPlaying(const bool& = true);
      void setCalculating(const bool& = true);

      void updateFrame(Frame*, Frame*);

    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>&);

      MainWindow *mainWindow;

    private:
      Glib::RefPtr<Gdk::Pixbuf> image;
      Glib::RefPtr<Gdk::Pixbuf> filteredImage;

      Frame *frame;
      Frame *rgbFrame;

      unsigned int totalCount;
      std::map<Blob*, unsigned int> blobCounts;

      void initialiseProperties();
      void initialiseImage();
      void initialiseConstants();

      void resetConstants();

      bool applyCalculations();
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

  };

};

#endif // RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_IMAGE_DRAWING_AREA_H
