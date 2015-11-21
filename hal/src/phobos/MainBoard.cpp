
#include "MainBoard.hpp"

namespace rtx { namespace hal {

  const char CMD_BALL_SENSE[] = "bl";

  MainBoard::MainBoard() {
    id = 255;
  }

  void MainBoard::init(RTX485::WriteHandle wHandle, RTX485::SignalService sigRegister) {
    RTX485::Device::init(wHandle, sigRegister);
  }

  void signal(RTX485::Message m) {

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

