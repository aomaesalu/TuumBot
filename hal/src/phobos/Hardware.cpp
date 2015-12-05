/** @file Hardware.cpp
 *  Platform Hardware class implementation.
 *
 *  @authors Ants-Oskar Mäesalu, Kristjan Kanarbik, Meelik Kiik
 *  @version 0.1
 *  @date 4 December 2015
 */

#include <iostream>

#include "Hardware.hpp"

namespace rtx { namespace hal {

  const RTX485::DeviceID RTX_MAIN_BOARD_ID = 0;

  Hardware::Hardware():
    m_frontCamera(CAMERA_DEVICE, CAMERA_WIDTH, CAMERA_HEIGHT),
    m_backCamera(CAMERA_DEVICE_2, CAMERA_WIDTH, CAMERA_HEIGHT)
  {

  }

  void Hardware::init() {
    if(gC.getStr("HW.Active") == "Y") {
      printf("[Hardware::init]Loading hardware...\n");
      HWBus.init(gC.getStr("HWBus.Port").c_str(), gC.getInt("HWBus.Baud"));
      m_refereeListener.init(gC.getStr("RefModule.Port").c_str(), gC.getInt("RefModule.Baud"));

      m_motorControl.init(hw_bus_write);
      m_mainBoard.init(hw_bus_write, hw_bus_register);
    } else {
      printf("[Hardware::init]Hardware not active.\n");
    }
  }

  void Hardware::run() {
    m_mainBoard.run();
  }

  Camera* Hardware::getFrontCamera() {
    return &m_frontCamera;
  }

  Camera* Hardware::getBackCamera() {
    return &m_backCamera;
  }

  MainBoard* Hardware::getMainBoard() {
    return &m_mainBoard;
  }

  MotorControl* Hardware::getMotorControl() {
    return &m_motorControl;
  }

  RefereeListener* Hardware::getRefListener() {
    return &m_refereeListener;
  }

}}
