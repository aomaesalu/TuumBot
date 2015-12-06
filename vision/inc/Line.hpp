/**
 *  @file Line.hpp
 *  Line seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 6 December 2015
 */

#ifndef RTX_VISION_LINE_H
#define RTX_VISION_LINE_H

#include "Feature.hpp"

#include <vector>


namespace rtx { namespace Vision {

  class Line: public Feature {

    public:
      Line(const Line&);
      Line(const double&, const double&, const unsigned int&);
      Line(const std::vector<std::pair<double, double>>&, const unsigned int&);
      ~Line();

      double getSlope() const;

      std::vector<std::pair<double, double>> getPoints() const; // TODO: Remove

    private:
      std::vector<std::pair<double, double>> points; // TODO: Remove

  };

}}

#endif // RTX_VISION_LINE_H
