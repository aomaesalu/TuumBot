/**
 * @file Line.hpp
 * Line class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_GEOMETRY_LINE_H
#define RTX_GEOMETRY_LINE_H

#include "Point2D.hpp"


namespace rtx {

  class Line {
    public:
      Line(const Line&);
      Line(const Point2D*, const Point2D*);
      Line(const double&, const double&, const double&, const double&);

      void setPointA(const Point2D*);
      void setPointA(const double&, const double&);
      void setPointB(const Point2D*);
      void setPointB(const double&, const double&);

      double getLength() const;
      Point2D* getPointA() const;
      Point2D* getPointB() const;

    private:
      Point2D *pointA;
      Point2D *pointB;
  };

};

#endif // RTX_GEOMETRY_LINE_H
