#include "MotorDriver.hpp"
#include <iostream>

using namespace rtx;

MotorDriver::MotorDriver(int mtrid, int usedPort){
  motorid = mtrid;
  port = usedPort;
}

MotorDriver::~MotorDriver() {
}

void MotorDriver::sendcmd(std::string cmd){
  write(port, cmd.c_str(), cmd.size());
}

std::string MotorDriver::readLine(){
  int res;
  char buf[255];
  res = read(port,buf,255);
  return buf;
}

void MotorDriver::setSpeed(int newSpeed){
  std::string speedcmd = std::to_string(motorid) + ":sd" + std::to_string(newSpeed) + "\n";
  write(port, speedcmd.c_str(), speedcmd.size());
  std::cout << speedcmd << std::endl;
}

void MotorDriver::stop(){
  std::string speedcmd = std::to_string(motorid) + ":sd" + std::to_string(0) + "\n";
  write(port, speedcmd.c_str(), speedcmd.size());
}
