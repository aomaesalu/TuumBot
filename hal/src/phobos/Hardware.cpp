/** @file Hardware.cpp
 *  Platform Hardware class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include <iostream>

#include "Hardware.hpp"


namespace rtx { namespace hal {

  Hardware::Hardware() {

  }

  void Hardware::init() {
    printf("[Hardware::init]Loading hardware...\n");
    m_motorControl.init();
  }

  CameraDevice* Hardware::getFrontCamera() {
    return nullptr;
  }

  CameraDevice* Hardware::getBackCamera() {
    return nullptr;
  }

  MotorControl* Hardware::getMotorControl() {
    return &m_motorControl;
  }

  bool Hardware::isBallInDribbler() {
    return false;
  }

}}
