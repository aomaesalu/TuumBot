/** @file Hardware.cpp
 *  Platform Hardware class implementation.
 *
 *  @authors Meelik Kiik
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 11. November 2015
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

  Camera* Hardware::getFrontCamera() {
    return m_frontCamera;
  }

  Camera* Hardware::getBackCamera() {
    return m_backCamera;
  }

  MotorControl* Hardware::getMotorControl() {
    return &m_motorControl;
  }

  bool Hardware::isBallInDribbler() {
    return false;
  }

}}
