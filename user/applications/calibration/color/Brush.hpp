/**
 *  @file Brush.hpp
 *  Color calibration application brush class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 14 November 2015
 */

#ifndef RTX_APPLICATIONS_CALIBRATION_COLOR_BRUSH_H
#define RTX_APPLICATIONS_CALIBRATION_COLOR_BRUSH_H


namespace rtx {

  class Brush {

    public:
      Brush(Gtk::Scale*);
      virtual ~Brush();

      Gtk::Scale* getScale() const;
      unsigned int getSize() const;
      unsigned int getX() const;
      unsigned int getY() const;

      void setX(const unsigned int&);
      void setY(const unsigned int&);

    private:
      Gtk::Scale *scale;
      unsigned int x;
      unsigned int y;

  }

}

#endif // RTX_APPLICATIONS_CALIBRATION_COLOR_BRUSH_H
