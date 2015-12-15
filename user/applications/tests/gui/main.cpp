
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "rtxgui.hpp"

#include "application.hpp"

using namespace rtx;

void sys_init() {
  rtx::hal::setup();
  Visioning::setup();
}

void sys_main() {
  while(1) {
    rtx::hal::process();
    Visioning::process();
  }
}

int main(int argc, char *argv[]) {
  rtx::init(argc, argv);
  sys_init();
  boost::thread workerThread(sys_main);

  return rtx::gui::run(argc, argv);
}
