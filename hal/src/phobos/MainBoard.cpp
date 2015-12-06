
#include <sstream>
#include <stdlib.h>
#include <stdint.h>

#include "MainBoard.hpp"

namespace rtx { namespace hal {

  const char CMD_BALL_SENSE[] = "bl";
  const char CMD_DRIBBLER[] = "dm";
  const char CMD_COIL[] = "c";
  const char CMD_KICK[] = "k";
  const char CMD_WKICK[] = "k3";

  MainBoard::MainBoard() {
    id = 255;

    m_ballSensorState = 0;
    m_dribblerState = 0;
    m_coilKickActive = 0;

    m_coilKickCharge.setPeriod(100);
    m_coilKickCooldown.setPeriod(1500);

    m_updateTimer.setPeriod(300);
    m_updateTimer.start();

    senseBall();
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
      if(m_coilChargeLevel > 4) {
        m_coilKickActive = false;
	m_coilKickCooldown.start();
      } else {
      	chargeCoil();
        m_coilChargeLevel++;
	if(m_coilChargeLevel == 2) releaseCoil();
	else chargeCoil();
	m_coilKickCharge.start();
      }

    }
  }

  bool MainBoard::getBallSensorState() {
    return m_ballSensorState;
  }

  void MainBoard::senseBall() {
    send({id, CMD_BALL_SENSE});
  }


  void MainBoard::chargeCoil() {
    send({id, CMD_COIL});
  }

  void MainBoard::releaseCoil() {
    if(m_coilKickStrong)
      send({id, CMD_KICK});
    else
      send({id, CMD_WKICK});
  }

  void MainBoard::coilKick() {
    if(!m_coilKickActive && m_coilKickCooldown.isTime()) {
      senseBall();
      chargeCoil();
      m_coilKickActive = true;
      m_coilChargeLevel = 0;
      m_coilKickCharge.start();
    }
  }

  void MainBoard::doCoilKick() {
    m_coilKickStrong = true;
    coilKick();
  }

  void MainBoard::doWeakCoilKick() {
    m_coilKickStrong = false;
    coilKick();
  }

  std::string getDribblerCmd(int v) {
    std::stringstream out;
    out << CMD_DRIBBLER;
    out << v;
    return out.str();
  }

  void MainBoard::startDribbler() {
    m_dribblerState = 1;
    send({id, getDribblerCmd(140)});
  }

  void MainBoard::stopDribbler() {
    m_dribblerState = 0;
    send({id, getDribblerCmd(0)});
  }



}}
