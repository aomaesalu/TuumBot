/**
 *  @file Line.hpp
 *  Line seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#ifndef RTX_VISION_LINE_H
#define RTX_VISION_LINE_H

#include <vector>


namespace rtx {

  class Line {

    public:
      Line(const Line&);
      Line(const double&, const double&);
      Line(const std::vector<std::pair<double, double>>&);
      ~Line();

      double getDistance() const;
      double getAngle() const;

      void setDistance(const double&);
      void setAngle(const double&);

    private:
      double distance;
      double angle;

  };

}

#endif // RTX_VISION_LINE_H
