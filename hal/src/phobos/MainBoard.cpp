
#include <sstream>
#include <stdlib.h>
#include <stdint.h>

#include "MainBoard.hpp"

namespace rtx { namespace hal {

  const char CMD_BALL_SENSE[] = "bl";
  const char CMD_DRIBBLER[] = "dm";
  const char CMD_COIL[] = "c";
  const char CMD_KICK[] = "k";

  MainBoard::MainBoard() {
    id = 255;

    m_ballSensorState = 0;
    m_dribblerState = 0;
    m_coilKickActive = 0;

    m_coilKickCharge.setPeriod(200);
    m_coilKickCooldown.setPeriod(1000);

    m_updateTimer.setPeriod(300);
    m_updateTimer.start();
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

  void MainBoard::run() {
    if(m_updateTimer.isTime()) {
      if(m_dribblerState) {
        startDribbler();
      }
      m_updateTimer.start();
    }

    if(m_coilKickActive && m_coilKickCharge.isTime()) {
      if(m_coilChargeLevel >= 4) {
        m_coilKickActive = false;
	m_coilKickCooldown.start();
      } else {
      	chargeCoil();
        m_coilChargeLevel++;
	if(m_coilChargeLevel == 3) releaseCoil();
	else chargeCoil();
	m_coilKickCharge.start();
      }

    }
  }

  bool MainBoard::getBallSensorState() {
    return m_ballSensorState;
  }

  void MainBoard::senseBall() {
    if(write == nullptr) {
      std::cout << "[MainBoard::senseBall]Error: No communication bus." << std::endl;
      return;
    }
    write({id, CMD_BALL_SENSE});
  }


  void MainBoard::chargeCoil() {
    write({id, CMD_COIL});
  }

  void MainBoard::releaseCoil() {
	std::cout << "KICK" << std::endl;
    write({id, CMD_KICK});
  }

  void MainBoard::doCoilKick() {
    if(!m_coilKickActive && m_coilKickCooldown.isTime()) {
      stopDribbler();
      chargeCoil();
      m_coilKickActive = true;
      m_coilChargeLevel = 0;
      m_coilKickCharge.start();
    }
  }

  std::string getDribblerCmd(int v) {
    std::stringstream out;
    out << CMD_DRIBBLER;
    out << v;
    return out.str();
  }

  void MainBoard::startDribbler() {
    m_dribblerState = 1;
    write({id, getDribblerCmd(140)});
  }

  void MainBoard::stopDribbler() {
    m_dribblerState = 0;
    write({id, getDribblerCmd(0)});
  }



}}
