/**
 * @file MotorControl.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef RTX_MOTOR_CONTROL_H
#define RTX_MOTOR_CONTROL_H

#include <string>

#include <math.h>
#include <unistd.h>
#include <termios.h>

//#include <boost/thread/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

#include "MotorDriver.hpp"

#define n_motors 4
#define wheel_d 79
#define baudrate 19200

namespace rtx { namespace hal {

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
      void OmniDrive(double speed, double angle, double rot);

      void Move(double, double, double);

      void testSequence();

      void runDribbler(int speed);
      void stopDribbler();
      void kick(int ms);
      void charge();

  };

}}

#endif  // RTX_MOTOR_CONTROL_H
