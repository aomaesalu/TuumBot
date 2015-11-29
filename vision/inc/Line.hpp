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

#include "Feature.hpp"

#include <vector>


namespace rtx {

  class Line: public Feature {

    public:
      Line(const Line&);
      Line(const double&, const double&);
      Line(const std::vector<std::pair<double, double>>&);
      ~Line();

      std::pair<double, double> getRelativePoint() const;

  };

}

#endif // RTX_VISION_LINE_H
