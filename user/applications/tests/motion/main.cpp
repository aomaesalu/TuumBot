#include <cstdlib>
#include <iostream>
#include <string>

#include "tuum_motion.hpp"

using namespace std;
using namespace rtx;

int main(int argc, char* argv[]) {
  printf("Running motion tests...\n");

  Motion::setup();


  while(1) {
    Motion::process();
  };

  printf("Motion tests done.\n");
  return 0;
}
