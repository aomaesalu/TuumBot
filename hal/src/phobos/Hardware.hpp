/** @file Hardware.hpp
 *  Platform Hardware class for consolidating hardware control.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#ifndef HAL_HARDWARE_H
#define HAL_HARDWARE_H

#include "CameraDevice.hpp"

namespace rtx { namespace hal {

  class Hardware {
    private:
      int m_motorControl;

      CameraDevice* m_frontCamera;
      CameraDevice* m_backCamera;

      int m_dribbler;
      int m_coilGun;
      int m_ballSensor;
    public:
      Hardware();
      void init();

      CameraDevice* getFrontCamera();
      CameraDevice* getBackCamera();
      int* getMotorControl();

      bool isBallInDribbler();

      //...?
  };

}}

#endif // HAL_HARDWARE_H
