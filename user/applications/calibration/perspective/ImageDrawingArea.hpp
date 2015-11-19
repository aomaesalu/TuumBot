/**
 *  @file ImageDrawingArea.hpp
 *  Perspective calibration application image drawing area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 19 November 2015
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

      // Blob regression data
      unsigned int totalCount;
      std::map<Blob*, unsigned int> blobCounts;

      // Constant regression data
      double bestA, bestB, bestC;
      double bestHorisontalMSE, bestVerticalMSE;
      double maxError;
      unsigned int squareWidth;

      void initialiseProperties();
      void initialiseImage();
      void initialiseConstants();

      void resetConstants();

      void colorPixels(guint8*, const unsigned int&, const unsigned int&, guint8*, const unsigned int&, const unsigned int&);
      void regressBlobs();
      void colorBlobs(guint8*, const unsigned int&, const unsigned int&);
      void regressConstants();

      bool applyCalculations();
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_IMAGE_DRAWING_AREA_H
