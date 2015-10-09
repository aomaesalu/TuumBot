/**
 * @file Point2D.hpp
 * Description placeholder.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_GEOMETRY_POINT2D_H
#define RTX_GEOMETRY_POINT2D_H


namespace rtx {

  /** Placeholder
   *
   */
  class Point2D {
    public:
      Point2D(const Point2D&);
      Point2D(const double&, const double&);

      void setX(const double&);
      void setY(const double&);

      double distanceFrom(const Point2D*);
      double getX() const;
      double getY() const;

    private:
      double x;
      double y;
  };

};

#endif // RTX_GEOMETRY_POINT2D_H_
