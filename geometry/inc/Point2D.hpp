/**
 * @file Point2D.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef RTX_POINT2D_H_
#define RTX_POINT2D_H_

#include <stdint.h>

namespace rtx {

  /** Placeholder
   *
   */
  class Point2D {
    private:
      double x;
      double y;

    public:
      Point2D(const Point2D&);
      Point2D(const double&, const double&);

      void setX(const double&);
      void setY(const double&);

      double distanceFrom(const Point2D*);
      double getX() const;
      double getY() const;
  };

};

#endif // RTX_POINT2D_H_
