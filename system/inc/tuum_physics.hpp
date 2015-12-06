/** @file tuum_physics.hpp
 *  Physics system interface
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 3 December 2015
*/

#ifndef RTX_SYSTEM_PHYSICS_H
#define RTX_SYSTEM_PHYSICS_H

#include "tuum_visioning.hpp"
#include "Vision.hpp"


namespace rtx { namespace Physics {

  /** Returns all detected objects which lie
   *  on the line which parameters are given
   *  as inputs to this method.
   */
  Entity* rayCast(const double&, const double& = 0);

}}

#endif // RTX_SYSTEM_PHYSICS_H
