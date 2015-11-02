/** @file application.cpp
 *  Default tuum system application.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "application.hpp"
#include <iostream>

using namespace std;
using namespace rtx;

int main() {
  printf("main(): Default tuum system application.\n");

  // Initialize hardware
  rtx::hal::setup();

  // Initialize system modules
  Visioning::setup();
  Localization::setup();

  bool running = true;
  while(running) {
    rtx::hal::process();

    Visioning::process();
    Localization::process();
  }

  return 0;
}
