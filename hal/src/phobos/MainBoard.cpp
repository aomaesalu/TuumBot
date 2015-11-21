
#include <sstream>
#include <stdint.h>

#include "MainBoard.hpp"

namespace rtx { namespace hal {

  const char CMD_BALL_SENSE[] = "bl";

  MainBoard::MainBoard() {
    id = 255;
  }

  void MainBoard::init(RTX485::WriteHandle wHandle, RTX485::SignalService sigRegister) {
    RTX485::DeviceCallback cb = std::bind1st(std::mem_fun(&MainBoard::signal), this);
    RTX485::Device::init(wHandle, sigRegister, cb);
  }

  void MainBoard::signal(RTX485::Message m) {
    uint32_t ix = m.data.find(":");
    if(ix != -1) {
      std::string buf;
      buf = m.data.substr(0, ix);

      if(buf == CMD_BALL_SENSE) {
        m_ballSensorState = atoi(m.data.substr(ix+1).c_str());
      }
    }
  }

  void MainBoard::senseBall() {
    if(write == nullptr) {
      std::cout << "[MainBoard::senseBall]Error: No communication bus." << std::endl;
      return;
    }
    write({id, CMD_BALL_SENSE});
  }

  bool MainBoard::getBallSensorState() {
    return m_ballSensorState;
  }


}}

