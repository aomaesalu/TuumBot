/**
 *  @file Corner.hpp
 *  Corner seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#ifndef RTX_VISION_CORNER_H
#define RTX_VISION_CORNER_H

#include "Feature.hpp"

#include <utility>


namespace rtx { namespace Vision {

  class Corner: public Feature {

    public:
      Corner(const Corner&);
      Corner(const double&, const double&);
      Corner(const std::pair<double, double>&);
      ~Corner();

  };

}}

#endif // RTX_VISION_CORNER_H
