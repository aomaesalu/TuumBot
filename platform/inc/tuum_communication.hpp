
#ifndef RTX_TUUM_COMMUNICATION_H
#define RTX_TUUM_COMMUNICATION_H

#include <string>
#include <map>
#include <stdint.h>

namespace rtx { namespace comm {

  enum TuumSignal {
    PING,
    PONG,
    PASS,
    RECV
  };

  typedef std::map<std::string, TuumSignal> SignalMap;

  const SignalMap tuumSigMap = {
    {"PING", TuumSignal::PING},
    {"PONG", TuumSignal::PONG},
    {"PASS", TuumSignal::PASS},
    {"RECV", TuumSignal::RECV},
  };

  // Tuum Message syntax constants
  const char TMS_PREFIX = '$';
  const char TMS_TAG_BEGIN = '<';
  const char TMS_TAG_END = '>';
  const char TMS_DELIMITER = ':';

  // Tuum signal syntax: "tm<id_seq:FTR:DATA>"
  // Examples: "tm<0:ABB:PING>", "tm<0:ABA:PONG>"; "tm<1:ABB:PASS>", "tm<1:ABA:RECV>" )
  struct TuumMessage {
    static uint32_t id_seq;

    static std::string getSignalStr(TuumSignal);
    static TuumMessage toAlly(TuumSignal);

    enum ParsingPipeline {
      ID = 0,
      TARGET,
      SIGNAL,
      DONE,
    };

    // Command id
    uint32_t id;

    // Target selectors
    char field;
    char team;
    char robot;

    // Type identifier
    TuumSignal sig;

    TuumMessage();
    TuumMessage(std::string);

    void aquireID();

    std::string str();
  };


  typedef void (*Handler)(TuumMessage);
  typedef std::map<uint32_t, TuumMessage> TMSResponseMap;

  //TODO: cleanup old responses
  extern TMSResponseMap g_TuumCommData;

  void handleTuumMessage(std::string);

  bool pollResponse(uint32_t);
  TuumMessage popResponse(uint32_t);
}}

#endif // RTX_TUUM_COMMUNICATION_H
