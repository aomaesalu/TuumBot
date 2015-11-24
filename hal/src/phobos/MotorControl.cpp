
#include <iostream>
#include <cmath>

#include "MotorControl.hpp"

namespace rtx { namespace hal {

  const char CMD_MOTOR_SPEED[] = "sd";

  MotorControl::MotorControl() {
    write = nullptr;
  }

  MotorControl::~MotorControl() {

  }

  void MotorControl::init(RTX485::WriteHandle wHandle) {
    int id_seq, ix;

    for(ix=0, id_seq=1; ix < MOTOR_COUNT; ix++, id_seq++) {
      m_motorIDs[ix] = id_seq;
    }

    write = wHandle;

    std::cout << "[MotorControl]Ready." << std::endl;
  }


  std::string getSpeedCmd(int v) {
    std::stringstream out;
    out << CMD_MOTOR_SPEED;
    out << v;
    return out.str();
  }

  void MotorControl::OmniDrive(double speed, double angle, double rot) {
    int spd1 = speed * sin(angle + M_PI / 4.0) + rot;
    int spd2 = speed * -sin(angle - M_PI / 4.0) + rot;
    int spd3 = speed * -sin(angle + M_PI / 4.0) + rot;
    int spd4 = speed * sin(angle - M_PI / 4.0) + rot;

    int speeds[4] = {spd1, spd2, spd3, spd4};
    for (int ix=0; ix < MOTOR_COUNT; ix++) {
      write({m_motorIDs[ix], getSpeedCmd(speeds[ix])});
      usleep(10);
    }
  }

}}


    //serialPort=open(device, O_RDWR | O_NOCTTY | O_NDELAY);

    /*
    struct termios portcfg;
    tcgetattr(serialPort, &portcfg);
    tcflush(serialPort, TCIOFLUSH);
    cfsetispeed(&portcfg, baudrate);
    cfsetospeed(&portcfg, baudrate);

    portcfg.c_cflag |= CLOCAL;
    portcfg.c_cflag |= CREAD;
    portcfg.c_cflag &= ~CSIZE;
    portcfg.c_cflag |= CS8;
    portcfg.c_cflag &= ~PARENB;
    portcfg.c_cflag &= ~CSTOPB;

    tcsetattr(serialPort, TCSANOW, &portcfg);
    tcflush(serialPort, TCIOFLUSH);

    for(int i=1; i < (n_motors+1); i++) {
      motors[i] = new MotorDriver(motorids[i-1], serialPort);
    }

    mainboard = new MotorDriver(0, serialPort);
    */

  /*
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
  */


  /*
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
    mainboard->sendcmd(runcmd);
  }
  void MotorControl::stopDribbler() {
    std::string stopcmd = std::to_string(0) + ":dr" + std::to_string(0) + "\n";
    mainboard->sendcmd(stopcmd);
  }

  void MotorControl::kick(int ms) {
    std::string kickcmd = std::to_string(0) + ":k" + std::to_string(ms) + "\n";
    mainboard->sendcmd(kickcmd);
  }

  void MotorControl::charge() {
    std::string chargecmd = std::to_string(0) + ":c" + "\n";
    mainboard->sendcmd(chargecmd);
  }
  */


