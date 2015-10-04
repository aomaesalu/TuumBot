/**
 * @file MotorControl.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef RTX_MOTOR_CONTROL_H
#define RTX_MOTOR_CONTROL_H

#include "MotorDriver.hpp"


namespace rtx {

  class MotorDriver {
    private:
      MotorDriver* motors;
      uint8_t n_motors;

    public:
      void init();

  };

};

#endif
