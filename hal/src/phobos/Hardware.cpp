/** @file Hardware.cpp
 *  Platform Hardware class implementation.
 *
 *  @authors Ants-Oskar Mäesalu, Kristjan Kanarbik, Meelik Kiik
 *  @version 0.1
 *  @date 11. November 2015
 */

#include <iostream>

#include "Hardware.hpp"

namespace rtx { namespace hal {

  const RTX485::DeviceID RTX_MAIN_BOARD_ID = 0;

  Hardware::Hardware():
    m_frontCamera(CAMERA_DEVICE, CAMERA_WIDTH, CAMERA_HEIGHT)//,
    //m_backCamera(CAMERA_DEVICE, CAMERA_WIDTH, CAMERA_HEIGHT) // TODO: Detect correct camera device for back camera; currently the cameras are the same.
  {
    //referee = new SerialPort();
    //referee2 = new SerialPort();
  }

  void Hardware::init() {
    printf("[Hardware::init]Loading hardware...\n");
    HWBus.init("/dev/ttyUSB0", 19200);

    m_motorControl.init(hw_bus_write);
    m_mainBoard.init(hw_bus_write, hw_bus_register);

    //refereeListener.init("/dev/ttyACM0");
    //refereeListener2.init("/dev/ttyACM1");
    //refereeListener.write_some("aAXASTART----");
    //referee->init("/dev/ttyACM0", 9600);
    //referee2->init("/dev/ttyACM1", 9600);
    //referee->write_some("aABKICKOFF----");
  }

  void Hardware::run() {
    m_mainBoard.run();
  }

  Camera* Hardware::getFrontCamera() {
    return &m_frontCamera;
  }

  Camera* Hardware::getBackCamera() {
    return nullptr; //&m_backCamera;
  }

  MainBoard* Hardware::getMainBoard() {
    return &m_mainBoard;
  }

  MotorControl* Hardware::getMotorControl() {
    return &m_motorControl;
  }

}}
