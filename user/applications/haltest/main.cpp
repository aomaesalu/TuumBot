#include "application.h"

#include <iostream>
#include "MotorDriver.hpp"
#include "MotorControl.hpp"










int main(){
  printf("haltest\n");
  const char* dev = "/dev/ttyACM0";
  rtx::MotorControl* test = new rtx::MotorControl(dev, 115200);
  //test->forward(19);
  test->turn(180);
}
