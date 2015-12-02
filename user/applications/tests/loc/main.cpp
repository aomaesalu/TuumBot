#include <cstdlib>
#include <iostream>
#include <string>

#include "tuum_localization.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  printf("Running localization tests...\n");

  rtx::Localization::setup();
  /*
  Mat m(4, 4);
  int test = m.getValue(0, 0);
  printf("%i\n", test);
  */

  printf("Entering main loop...\n");
  bool running = true;
  while(running) {
    rtx::Localization::process();
  }


  printf("Localization tests done.\n");
  return 0;
}
