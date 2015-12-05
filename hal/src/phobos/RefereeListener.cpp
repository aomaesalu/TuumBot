
#include <iostream>

#include "tuum_platform.hpp"

#include "RefereeListener.hpp"

using namespace rtx;

namespace rtx { namespace hal {

  RefereeListener::RefereeListener() {

  }

  void RefereeListener::init(std::string portname, int baud) {
    std::cout << "[RefereeListener::init]Port " << portname
              << ", " << baud
              << std::endl;
    SerialPort::init(portname.c_str(), baud);
    write_some("+++");

    m_field = gC.getStr("Game.Field")[0];
    m_team  = gC.getStr("Game.Team")[0];
    m_robot = gC.getStr("Game.Robot")[0];
    std::cout << "[RefereeListener]Ready. "
              << "Field " << m_field
              << ", Team " << m_team
              << ", Robot " << m_robot
              << std::endl;
  }

  void RefereeListener::registerCallback(const RefereeSignal sig, VoidFn callback) {
    callbacks[sig] = callback;

    std::cout << sig << std::endl;
  }

  void RefereeListener::signal(RefCommand cmd) {
    //TODO: filter out foreign commands?

    auto it = callbacks.find(cmd.sig);
    if(it != callbacks.end()) {
      it->second(cmd);
      return;
    }
  }

  /**
   *  Command syntax:
   *    Byte 0 - 'a'
   *    Byte 1,2 - Target robots selector: field+teams (AX, BD, ...)
   *    Bytes 3, ... - Command
   *    Byte 3 (Optional) - Target team
   */
  RefCommand RefereeListener::parseCommand(std::string msg) {
    std::string cmd;
    char team = msg.at(3);
    if((team == 'A') || (team == 'B')){
      cmd = msg.substr(4);
    } else {
      team = 0;
      cmd = msg.substr(3);
    }

    std::cout << cmd << std::endl;

    auto it = refSigMap.find(cmd);

    if(it != refSigMap.end()) {
      if( team != 0 ) {
        // Specific command
        return RefCommand({it->second, {msg.at(1), msg.at(2), team}});
      } else {
        // General command
        return RefCommand({it->second, {msg.at(1), msg.at(2), '0'}});
      }
    }

    std::cout << "[RefereeListener::parseCommand]Warning: Unknown signal "
              << cmd << std::endl;

    return RefCommand({REF_VOID, {'0', '0', '0'}});
  }

  void RefereeListener::on_receive_(const std::string &data) {
    std::string message = data.substr(0, 12);
    //std::cout << "Received " << data << std::endl;
    if(message[0] == 'a') {
      if(message[1] == m_field) {
        std::cout << data << std::endl;
        this->signal(this->parseCommand(message));
      }
    } else if(data[0] == comm::TMS_PREFIX) {
      comm::handleTuumMessage(data);
    }
  }

  void RefereeListener::sendTuumMessage(comm::TuumMessage tms) {
    try { comm::popResponse(tms.id); } catch (int) {}
    std::string data = tms.str();
    //std::cout << "Sending " << data << std::endl;
    this->write_some(data.c_str(), data.size());
  }

  void RefereeListener::sendAck() {
    const std::string ack = "a"+std::string(1,m_field)+std::string(1,m_robot)+"ACK------";
    std::cout << ack;
    this->write_some(ack.c_str(), ack.size());
  }

}}
