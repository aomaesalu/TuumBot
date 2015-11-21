
#include <chrono>

#include "hal.hpp"

using namespace std::chrono;


unsigned int millis() {
  auto ep = high_resolution_clock::from_time_t(0);
  auto now = high_resolution_clock::now();
  auto msec = duration_cast<milliseconds>(now - ep).count();
  return msec;
}
