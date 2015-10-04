/**
 * @file tuum_vision.hpp
 * Visioning interface declaration.
 *
 * @authors Meelik Kiik
 * @version 0.1
 */

#ifndef VISIOH_H
#define VISION_H

#include "Camera.hpp"

using namespace rtx;

#define RTX_INTERFACE_CAM0 0
#define RTX_INTERFACE_CAM1 1

extern Camera CAM;
extern Camera CAM1;


/**
 *  Vision namespace manages
 *  separate threads which
 *  are constantly reading corresponding
 *  camera feeds into memory buffers.
 */
namespace Vision {
  void init();
};

#endif // VISION_H
