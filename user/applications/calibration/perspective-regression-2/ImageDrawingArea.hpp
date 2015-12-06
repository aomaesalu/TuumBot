/**
 *  @file ImageDrawingArea.hpp
 *  Perspective calibration application image drawing area.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
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

      double getB() const;
      double getC() const;

      void resetConstants();
      void resetBlobRegression();

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
      std::map<Vision::Blob*, unsigned int> blobCounts;

      // Constant regression data // TODO: Move regression calculations to separate class
      double bestA, bestB, bestC;
      double lowerBound, upperBound;
      double bestVerticalMSE, bestHorisontalMSE;
      double maxError;
      unsigned int squareWidth;
      unsigned int numberOfDivisions;
      unsigned int numberOfBestDivisions;
      // <B value>
      std::vector<double> BList;
      // <C value>
      std::vector<double> CList;
      // <<B value, B next value>, MSE value>
      std::vector<std::pair<std::pair<double, double>, double>> verticalResultsList;
      // <<C value, C next value>, MSE value>
      std::vector<std::pair<std::pair<double, double>, double>> horisontalResultsList;

      void initialiseProperties();
      void initialiseImage();
      void initialiseConstants();
      void initialiseBlobRegression();

      void colorPixels(guint8*, const unsigned int&, const unsigned int&, guint8*, const unsigned int&, const unsigned int&);
      void regressBlobs();
      void colorBlobs(guint8*, const unsigned int&, const unsigned int&);
      void regressConstants(guint8*, const unsigned int&, const unsigned int&);

      bool applyCalculations();
      std::pair<unsigned int, unsigned int> realToPixel(const double&, const double&);
      bool drawPerspective(guint8*, const unsigned int&, const unsigned int&);
      bool drawImage(const Cairo::RefPtr<Cairo::Context>&);

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_PERSPECTIVE_IMAGE_DRAWING_AREA_H
