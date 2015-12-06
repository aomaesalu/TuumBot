/**
 *  @file Corner.hpp
 *  Corner seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 6 December 2015
 */

#ifndef RTX_VISION_CORNER_H
#define RTX_VISION_CORNER_H

#include "Feature.hpp"

#include <utility>


namespace rtx { namespace Vision {

  class Corner: public Feature {

    public:
      Corner(const Corner&);
      Corner(const double&, const double&, const unsigned int&);
      Corner(const std::pair<double, double>&, const unsigned int&);
      ~Corner();

  };

}}

#endif // RTX_VISION_CORNER_H
