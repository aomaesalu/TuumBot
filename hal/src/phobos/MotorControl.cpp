#include "MotorControl.hpp"
#include <iostream>

using namespace rtx;

MotorControl::MotorControl(){

}

MotorControl::MotorControl(const char *device, int baudrate){
  serialPort=open(device, O_RDWR | O_NOCTTY | O_NDELAY); //open port

  struct termios portcfg; // struct for settings

  // current settings
  tcgetattr(serialPort, &portcfg);

  // Flush the port's buffers (in and out) before we start using it
  tcflush(serialPort, TCIOFLUSH);

  // Set the input and output baud rates
  cfsetispeed(&portcfg, baudrate);
  cfsetospeed(&portcfg, baudrate);

  // c_cflag contains a few important things- CLOCAL and CREAD, to prevent
  //   this program from "owning" the port and to enable receipt of data.
  //   Also, it holds the settings for number of data bits, parity, stop bits,
  //   and hardware flow control.
  portcfg.c_cflag |= CLOCAL;
  portcfg.c_cflag |= CREAD;
  // Set up the frame information.
  portcfg.c_cflag &= ~CSIZE; // clear frame size info
  portcfg.c_cflag |= CS8;    // 8 bit frames
  portcfg.c_cflag &= ~PARENB;// no parity
  portcfg.c_cflag &= ~CSTOPB;// one stop bit

  // Now that we've populated our options structure, let's push it back to the
  //   system.
  tcsetattr(serialPort, TCSANOW, &portcfg);

  // Flush the buffer one more time.
  tcflush(serialPort, TCIOFLUSH);

  for(int i=1; i < (n_motors+1); i++) {
    motors[i] = new MotorDriver((i+4), serialPort);
  }
}

MotorControl::~MotorControl(){

  close(serialPort);
}

void MotorControl::forward(int newSpeed){
  for(int i=1; i < (n_motors+1); i++) {
    if (i >= 3){
      motors[i]->setSpeed(-newSpeed);
    }
    else {
      motors[i]->setSpeed(newSpeed);
    }
  }
}

void MotorControl::turn(int degrees){
  float wheel_perim = M_PI * wheel_d;
  int speed = 15;
  float cyc = (62.5 / (18.75 * 64)) * 15;
  float wheelspeed = wheel_perim * cyc; //mm/s
  float time_full_ms = (254 / wheelspeed) * 1000;
  float time_deg = time_full_ms / 360;
  int turntime = time_deg * degrees;

  for(int i=1; i < (n_motors+1); i++) {
    motors[i]->setSpeed(15);
  }
  std::cout << "Started" << turntime << std::endl;
  //boost::this_thread::sleep(boost::posix_time::milliseconds(turntime));
  usleep(turntime*1000);
  for(int i=1; i < (n_motors+1); i++) {
    motors[i]->stop();
  }
  std::cout << "Stopped" << std::endl;
}
