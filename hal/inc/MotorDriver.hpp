/** @file MotorDriver.hpp
 *  Description placeholder.
 *
 *  @authors Kristjan Kanarbik
 *  @version 0.1
 */

#ifndef RTX_MOTOR_DRIVER_H
#define RTX_MOTOR_DRIVER_H

#include <string>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

namespace rtx { namespace hal {

  class MotorDriver {
    private:
      int motorid;
      int port;

    public:
      MotorDriver(int mtrid, int usedPort);
      ~MotorDriver();

      void sendcmd(std::string cmd);
      void setSpeed(int newSpeed);

      std::string readLine();

      void stop();

  };

}}

#endif
