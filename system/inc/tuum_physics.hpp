/**
 *  @file tuum_physics.hpp
 *  Physics system interface
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
*/

#ifndef RTX_SYSTEM_PHYSICS_H
#define RTX_SYSTEM_PHYSICS_H

#include "Color.hpp"


namespace rtx { namespace Physics {

  std::pair<Color, double> rayCast(const double&);
  //std::pair<Color, double> rayCast(const double&, const double&)

}}

#endif // RTX_SYSTEM_PHYSICS_H
