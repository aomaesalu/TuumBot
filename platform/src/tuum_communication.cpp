
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

#include <boost/algorithm/string/predicate.hpp>

#include "tuum_communication.hpp"

namespace rtx { namespace comm {

  uint32_t TuumMessage::id_seq = 1;

  std::string comm_buffer = "";
  TMSResponseMap g_TuumCommData;

  TuumMessage::TuumMessage() {
    aquireID();
  }

  TuumMessage::TuumMessage(std::string data) {
    if(!(data[0] == TMS_PREFIX)) throw(-1);

    int start_ix, end_ix;
    start_ix = data.find(TMS_PREFIX);
    //end_ix = data.find(TMS_TAG_BEGIN);
    if(start_ix == -1) throw(-2);
    //if(end_ix == -1) throw(-2);

    int i;
    std::stringstream buf;
    std::string segment;
    char c;
    ParsingPipeline pp = ParsingPipeline::ID;

    for(i = start_ix + 1; i <= data.size(); i++) {
      if(i == data.size()) {
        pp = ParsingPipeline::SIGNAL;
        goto PROC;
      }

      c = data[i];
      if(c == TMS_DELIMITER || c == '-') {
PROC:   segment = buf.str();
        buf.str("");

        switch(pp) {
          case ParsingPipeline::ID:
            std::cout << "ID" << std::endl;
            id = atoi(segment.c_str());
            pp = ParsingPipeline::TARGET;
            break;
          case ParsingPipeline::TARGET:
            if(segment.size() < 3) throw(-3);
            std::cout << segment << "TARGET" << std::endl;
            field = segment[0];
            team = segment[1];
            robot = segment[2];
            pp = ParsingPipeline::SIGNAL;
            break;
          case ParsingPipeline::SIGNAL:
            {
              auto it = tuumSigMap.find(segment);
              if(it == tuumSigMap.end()) throw(-4);
              sig = it->second;
            }
            pp = ParsingPipeline::DONE;
            break;
          default:
            throw(-5);
        }

        if(pp == ParsingPipeline::DONE) break;
      }
      else buf << c;
    }
  }

  void TuumMessage::aquireID() {
    id = TuumMessage::id_seq;
    TuumMessage::id_seq = (TuumMessage::id_seq + 1) % 10;
  }

  std::string TuumMessage::str() {
    std::stringstream buf;
    buf << TMS_PREFIX
        << id << TMS_DELIMITER
        << field << team << robot << TMS_DELIMITER
        << TuumMessage::getSignalStr(sig);

    std::string out = buf.str();
    while(out.size() < 12) out += '-';
    return out;
  }

  std::string TuumMessage::getSignalStr(TuumSignal sig) {
    for(auto& it : tuumSigMap) {
      if(it.second == sig) return it.first;
    }
  }

  TuumMessage TuumMessage::toAlly(TuumSignal sig) {
    TuumMessage tms;
    tms.aquireID();
    tms.sig = sig;
    //TODO:
    tms.field = 'A';
    tms.team = 'B';
    tms.robot = 'A';

    return tms;
  }

  void handleTuumMessage(std::string data) {
    try {
      TuumMessage tms(data);

      //TODO: Check target condition
      g_TuumCommData[tms.id] = tms;
      std::cout << "[rtx::comm::handleTuumMessage]Message " << tms.id << " received." << std::endl;
      comm_buffer.clear();
    } catch (int err) {
      std::cout << "[rtx::comm::handleTuumMessage]Error "
                << err << ": Malformed packet." << std::endl;
    }
  }

  bool pollResponse(uint32_t msg_id) {
    auto it = g_TuumCommData.find(msg_id);
    if(it == g_TuumCommData.end()) return false;
    return true;
  }

  comm::TuumMessage popResponse(uint32_t msg_id) {
    auto it = g_TuumCommData.find(msg_id);
    if(it == g_TuumCommData.end()) throw(-1);
    TuumMessage tms = it->second;
    g_TuumCommData.erase(tms.id);
    return tms;
  }

}}
