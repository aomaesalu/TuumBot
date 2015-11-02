/**
 * @file MotorControl.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef RTX_MOTOR_CONTROL_H
#define RTX_MOTOR_CONTROL_H

#include <termios.h>
#include <string>
#include "MotorDriver.hpp"
//#include <boost/thread/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
#include <math.h>
#include <unistd.h>

#define n_motors 4
#define wheel_d 79
#define baudrate 19200

namespace rtx {

  class MotorControl {
    private:
      int serialPort;
      MotorDriver* motors[n_motors];

    public:
      MotorControl();
      ~MotorControl();
      void init();
      void forward(int newSpeed);
      void turn(int degrees);
      void turnsimple(int speed);
      void OmniDrive(float speed, float angle, float rot);


  };

};

#endif
