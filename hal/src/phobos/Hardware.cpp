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
    //referee = new SerialPort();
    //referee2 = new SerialPort();

  }

  void Hardware::init() {
    printf("[Hardware::init]Loading hardware...\n");
    m_motorControl.init();
    usleep(10000);
    refereeListener.init("/dev/ttyACM0");
    refereeListener2.init("/dev/ttyACM1");
    //refereeListener.write_some("aAXASTART----");
    //referee->init("/dev/ttyACM0", 9600);
    //referee2->init("/dev/ttyACM1", 9600);
    //referee->write_some("aABKICKOFF----");
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
