#include <iostream>

#include "application.hpp"
#include "tfb_logic.hpp"

using namespace std;
using namespace rtx;

int main() {
  printf("main(): starting Team Football Application (tfb)\n");

  // Initialize hardware
  rtx::hal::setup();

  // Initialize system modules
  Visioning::setup();
  Localization::setup();

  Logic::setup();

  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Localization::process();

    Logic::process();
  }
  return 0;
}
