
#include <iostream>
#include <cmath>

#include "MotorControl.hpp"

namespace rtx { namespace hal {

  const char CMD_MOTOR_SPEED[] = "sd";

  MotorControl::MotorControl():
    RTX485::Device::Device()
  {

  }

  void MotorControl::init(RTX485::WriteHandle wHandle) {
    int id_seq, ix;
    for(ix=0, id_seq=1; ix < MOTOR_COUNT; ix++, id_seq++) {
      m_motorIDs[ix] = id_seq;
    }

    RTX485::Device::init(wHandle);

    std::cout << "[MotorControl::init]Ready." << std::endl;
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
      send({m_motorIDs[ix], getSpeedCmd(speeds[ix])});
      usleep(10);
    }
  }

}}
