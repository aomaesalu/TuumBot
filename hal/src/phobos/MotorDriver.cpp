/** @file MotorDriver.hpp
 *  Description placeholder.
 *
 *  @authors Kristjan Kanarbik
 *  @version 0.1
 */

#include <iostream>

#include "MotorDriver.hpp"

namespace rtx { namespace hal {

  MotorDriver::MotorDriver(int mtrid, int usedPort) {
    motorid = mtrid;
    port = usedPort;
  }

  MotorDriver::~MotorDriver() {
  }

  void MotorDriver::sendcmd(std::string cmd) {
    write(port, cmd.c_str(), cmd.size());
  }

  std::string MotorDriver::readLine() {
    int res;
    char buf[255];
    res = read(port,buf,255);
    return buf;
  }

  void MotorDriver::setSpeed(int newSpeed) {
    std::string speedcmd = std::to_string(motorid) + ":sd" + std::to_string(newSpeed) + "\n";
    write(port, speedcmd.c_str(), speedcmd.size());
  }

  void MotorDriver::stop() {
    std::string speedcmd = std::to_string(motorid) + ":sd" + std::to_string(0) + "\n";
    write(port, speedcmd.c_str(), speedcmd.size());
  }

}}
