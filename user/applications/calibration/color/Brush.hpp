/**
 *  @file Brush.hpp
 *  Color calibration application brush class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 17 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_BRUSH_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_BRUSH_H

#include <gtkmm.h>


namespace rtx {

  class Brush {

    public:
      Brush(Gtk::Scale*);
      virtual ~Brush();

      Gtk::Scale* getScale() const;

      double getSize() const;
      unsigned int getRadius() const;

      unsigned int getUpperLimit() const;
      unsigned int getLowerLimit() const;
      unsigned int getStepSize() const;

      unsigned int getX() const;
      unsigned int getY() const;

      unsigned int getMinX() const;
      unsigned int getMinY() const;
      unsigned int getMaxX() const;
      unsigned int getMaxY() const;

      void setSize(const unsigned int&);
      void incrementSize();
      void decrementSize();

      void setX(const unsigned int&);
      void setY(const unsigned int&);

    private:
      Gtk::Scale *scale;
      unsigned int x;
      unsigned int y;

  };

}

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_BRUSH_H
