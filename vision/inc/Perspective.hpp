/**
 *  @file Perspective.hpp
 *  Perspective class for coordinate mapping.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 26 November 2015
 */

#ifndef RTX_VISION_PERSPECTIVE_H
#define RTX_VISION_PERSPECTIVE_H

#include <utility>


namespace rtx { namespace Vision { namespace Perspective {

  extern double A;
  extern double B;
  extern double C;

  // TODO: Maybe move samples here?

  std::pair<double, double> virtualToReal(const unsigned int&, const unsigned int&);
  std::pair<unsigned int, unsigned int> realToVirtual(const double&, const double&);

}}}

#endif // RTX_VISION_PERSPECTIVE_H
