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
#include "RTX485.hpp"

#define MOTOR_COUNT 4
#define WHEEL_DIAMETER 79
#define BAUDRATE 19200

namespace rtx { namespace hal {

  class MotorControl {
  private:
    RTX485::DeviceID m_motorIDs[MOTOR_COUNT];
    RTX485::WriteHandle write;

  public:
    MotorControl();
    ~MotorControl();

    void init(RTX485::WriteHandle);

    //void forward(int newSpeed);
    //void turn(int degrees);
    //void turnsimple(int speed);
    void OmniDrive(double speed, double angle, double rot);

    //void Move(double, double, double);

    //void testSequence();

    //void runDribbler(int speed);
    //void stopDribbler();
    //void kick(int ms);
    //void charge();

  };

}}

#endif  // RTX_MOTOR_CONTROL_H
