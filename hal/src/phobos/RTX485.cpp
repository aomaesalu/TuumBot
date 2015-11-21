/** @file RTX485.hpp
 *  RS485 bus communications implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 20. November 2015
 */

#include <string>
#include <cstdlib>

#include "RTX485.hpp"

namespace rtx { namespace hal {

  static RTX485::SignalMap rs_callbacks;

  RTX485::RTX485() {

  }

  //TODO: some kind of data flush on timeout?
  void RTX485::on_receive_(const std::string &data) {
    char c;
    for(int i=0; i < data.size(); i++) {
      c = data[i];
      m_dataBuf += c;

      if(c == '\n') {
	Message msg;
	if(processData(m_dataBuf, msg) < 0) return;
        signal(msg);
	m_dataBuf = "";
      }
    }
  }

  void RTX485::sendCommand(DeviceID id, std::string cmd) {
    std::stringstream data;
    data << (unsigned int)id << ":" << cmd << '\n';
    write_some(data.str());
  }

  //FIXME: Refactor code
  enum RTX485_MSG_CHUNK {
    RMC_BEGIN,
    RMC_ID,
    RMC_DATA,
    RMC_END,
  };

  int RTX485::processData(const std::string &data, Message& msg) {
    char c;
    RTX485_MSG_CHUNK state = RMC_BEGIN;
    std::stringstream buf;

    for(int i=0; i < data.size(); i++) {
      c = data[i];

      switch(state) {
        case RMC_BEGIN:
          if(c == '<') state = RMC_ID;
	  break;
	case RMC_ID:
	  if(c == ':') {
	    msg.id = atoi(buf.str().c_str());
	    buf.str("");
	    state = RMC_DATA;
	    break;
	  }
          buf << c;
	  break;
	case RMC_DATA:
	  if(c == '>') {
	    msg.data = buf.str();
	    state = RMC_END;
	    break;
	  }
	  buf << c;
	  break;
      }

      if(state == RMC_END) break;
    }
    if(state != RMC_END) return -1;

    return 0;
  }

  void RTX485::registerDevice(SignalParams sp) {
    std::cout << "[RTX485::registerDevice]" << (unsigned int)(sp.id) << std::endl;
    rs_callbacks[sp.id] = sp.cb;
  }

  void RTX485::signal(Message msg) {
    auto it = rs_callbacks.find(msg.id);
    if(it != rs_callbacks.end()) {
      it->second(msg);
    } else {
      std::cout << "[RTX485::signal]Message to unknown device - "  << (unsigned int)msg.id << ", " << msg.data << std::endl;
    }
  }

}}
