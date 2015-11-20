/** @file Hardware.hpp
 *  Platform Hardware class for consolidating hardware control.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#ifndef HAL_HARDWARE_H
#define HAL_HARDWARE_H

#include "CameraDevice.hpp"
#include "MotorControl.hpp"
#include "RefereeListener.hpp"
#include "SerialPort.hpp"

namespace rtx { namespace hal {

  class Hardware {
    private:
      MotorControl m_motorControl;

      //SerialPort *referee;
      //SerialPort *referee2;

      CameraDevice* m_frontCamera;
      CameraDevice* m_backCamera;

      int m_dribbler;
      int m_coilGun;
      int m_ballSensor;

    public:
      RefereeListener refereeListener;
      RefereeListener refereeListener2;
      Hardware();
      void init();

      CameraDevice* getFrontCamera();
      CameraDevice* getBackCamera();
      MotorControl* getMotorControl();

      bool isBallInDribbler();

      //...?
  };

}}

#endif // HAL_HARDWARE_H
