
#include <iostream>
#include <cmath>

#include "MotorControl.hpp"

namespace rtx { namespace hal {

  MotorControl::MotorControl() {

  }

  MotorControl::~MotorControl() {

    close(serialPort);
  }

  void MotorControl::init() {
    int motorids[4] = {5,6,7,8};
    const char* device = "/dev/ttyUSB0";
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
      motors[i] = new MotorDriver(motorids[i-1], serialPort);
    }

    // mainboard = new MotorDriver(0, serialPort);
  }

  void MotorControl::forward(int newSpeed) {
    for(int i=1; i < (n_motors+1); i++) {
      if (i >= 3){
	motors[i]->setSpeed(-newSpeed);
      }
      else {
	motors[i]->setSpeed(newSpeed);
      }
      usleep(1000);
    }
  }

  void MotorControl::turn(int degrees) {
    double wheel_perim = M_PI * wheel_d;
    int speed = 15;
    double cyc = (62.5 / (18.75 * 64)) * 15;
    double wheelspeed = wheel_perim * cyc; //mm/s
    double time_full_ms = (254 / wheelspeed) * 1000;
    double time_deg = time_full_ms / 360;
    int turntime = time_deg * degrees;

    for(int i=1; i < (n_motors+1); i++) {
      motors[i]->setSpeed(15);
    }

    //boost::this_thread::sleep(boost::posix_time::milliseconds(turntime));
    usleep(turntime*1000);
    for(int i=1; i < (n_motors+1); i++) {
      motors[i]->stop();
    }
  }

  void MotorControl::turnsimple(int speed) {
    for (int i=1; i < (n_motors+1); i++){
      motors[i]->setSpeed(speed);
      usleep(100);
    }
  }

  void MotorControl::OmniDrive(double speed, double angle, double rot) {
    int spd1 = speed * sin(angle + M_PI / 4.0) + rot;
    int spd2 = speed * -sin(angle - M_PI / 4.0) + rot;
    int spd3 = speed * -sin(angle + M_PI / 4.0) + rot;
    int spd4 = speed * sin(angle - M_PI / 4.0) + rot;

    int speeds[4] = {spd1, spd2, spd3, spd4};
    for (int i=1; i < (n_motors+1); i++){
      motors[i]->setSpeed(speeds[i-1]);
      usleep(100);
    }
  }

  void MotorControl::Move(double x, double y, double db) {
    double spd, a;
    if(x != 0 || y != 0) {
      spd = sqrt(pow(x, 2) + pow(y, 2));
      a = atan(y / x);
    } else {
      spd = 0.0;
      a = 0.0;
    }


    double v_rot;
    if(db == 0) v_rot = 0;
    else v_rot = M_PI * spd / (2*db);

    if(x < 0 || y < 0) spd = spd * -1;
    printf("%g, %g, %g => ", x, y, db);
    printf("%g, %g, %g\n", spd, a, v_rot);

    OmniDrive(spd, a, v_rot);
  }

  void MotorControl::testSequence() {
    double x, y, db, S = 20;

    x = 0.0; y = 0.0; db = 0.0;
    Move(x*S, y*S, db);
    usleep(1000 * 1000);

    x = 1.0; y = 0.0; db = 0.0;
    Move(x*S, y*S, db);
    usleep(1000 * 1000);
    x = 0.5; y = 0.5; db = 0.0;
    Move(x*S, y*S, db);
    usleep(1000 * 1000);
    x = -1.0; y = 0.0; db = 0.0;
    Move(x*S, y*S, db);
    usleep(1000 * 1000);
    x = -0.5; y = -0.5; db = 0.0;
    Move(x*S, y*S, db);
    usleep(1000 * 1000);

    x = 0.0; y = 0.0; db = 0.0;
    Move(x*S, y*S, db);
    usleep(1000 * 1000);


  }

  void MotorControl::runDribbler(int speed) {
    std::string runcmd = std::to_string(0) + ":dr" + std::to_string(speed) + "\n";
    // mainboard->sendcmd(runcmd);
  }
  void MotorControl::stopDribbler() {
    std::string stopcmd = std::to_string(0) + ":dr" + std::to_string(0) + "\n";
    // mainboard->sendcmd(stopcmd);
  }

  void MotorControl::kick(int ms) {
    std::string kickcmd = std::to_string(0) + ":k" + std::to_string(ms) + "\n";
    // mainboard->sendcmd(kickcmd);
  }

  void MotorControl::charge() {
    std::string chargecmd = std::to_string(0) + ":c" + "\n";
    // mainboard->sendcmd(chargecmd);
  }

}}
