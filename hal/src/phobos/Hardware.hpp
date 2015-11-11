/** @file Hardware.hpp
 *  Platform Hardware class for consolidating hardware control.
 *
 *  @authors Meelik Kiik
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 11. November 2015
 */

#ifndef HAL_HARDWARE_H
#define HAL_HARDWARE_H

#include "Camera.hpp"
#include "MotorControl.hpp"

namespace rtx { namespace hal {

  class Hardware {
    private:
      MotorControl m_motorControl;

      Camera* m_frontCamera;
      Camera* m_backCamera;

      int m_dribbler;
      int m_coilGun;
      int m_ballSensor;
    public:
      Hardware();
      void init();

      Camera* getFrontCamera();
      Camera* getBackCamera();
      MotorControl* getMotorControl();

      bool isBallInDribbler();

      //...?
  };

}}

#endif // HAL_HARDWARE_H
