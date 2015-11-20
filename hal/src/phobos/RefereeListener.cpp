
#include <iostream>

#include "RefereeListener.hpp"

using namespace rtx;

namespace rtx { namespace hal {

  RefereeListener::RefereeListener() {

  }

  void RefereeListener::init(std::string portname) {
    std::cout << "[RefereeListener]Init..." << std::endl;
    SerialPort::init(portname.c_str(), 9600);


    std::cout << "[RefereeListener]Ready." << std::endl;
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

  RefCommand RefereeListener::parseCommand(std::string cmd) {
    // Parse RefTarget

    auto it = refSigMap.find(cmd);

    if(it != refSigMap.end()){
      if((cmd.at(3)=='A') || (cmd.at(3) == 'B')){
        return RefCommand({it->second, {cmd.at(1), cmd.at(2), cmd.at(3)}});
      }
      else {
        return RefCommand({it->second, {cmd.at(1), cmd.at(2), '0'}});
      }
    }

    return RefCommand({REF_VOID, {'0', '0', '0'}});
  }


  void RefereeListener::on_receive_(const std::string &data) {
  	std::string message = data.substr(0, 12);
  	std::cout << "RefereeListener::on_receive_() : " << data << std::endl;

		if ((message[0] == 'a') && (message[1] == FIELD)) {
      //TODO
      message = message.substr(4);
      std::cout << "RefereeListener::on_receive_() : " << message << std::endl;
      this->signal(this->parseCommand(message));
      if (message[2] == ID) this->sendAck();
		}
  }

  void RefereeListener::sendAck(){
  	const std::string ack = "a"+std::string(1,FIELD)+std::string(1,ID)+"ACK------";
  	std::cout << ack;
  	this->write_some(ack.c_str(), ack.size());
  }

}}
