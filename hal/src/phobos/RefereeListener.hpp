#ifndef RTX_REFEREE_LISTENR_H
#define RTX_REFEREE_LISTENR_H

#include <functional>
#include <string>
#include <map>

#include "SerialPort.hpp"

namespace rtx { namespace hal {

  enum RefereeSignal {
    REF_VOID,
    REF_START,
    REF_STOP,
    REF_PLACEDBAL,
    REF_ENDHALF

  };

  typedef std::map<std::string, RefereeSignal> RefSigMap;

  const RefSigMap refSigMap = {
    {"START---", REF_START},
    {"STOP----", REF_STOP}
  };

  // Field, Robot ID, Team
  struct RefTarget {
    char field;
    char team;
    char id;
  };

  struct RefCommand {
    RefereeSignal sig;
    RefTarget target;
  };

  typedef void (*VoidFn)(RefCommand);
  typedef std::pair<RefereeSignal, VoidFn> SignalCallback;

  inline void test(RefereeSignal s) {

  }

  //FIXME: This is not regularly working over different threads.
  static std::map<RefereeSignal, VoidFn> callbacks;

  class RefereeListener : public SerialPort {
  private:

  public:
    RefereeListener();

    void init(std::string portname);

    void on_receive_(const std::string &data);

    void registerCallback(const RefereeSignal, VoidFn);
    void signal(RefCommand);

    RefCommand parseCommand(std::string);

    void sendAck();

    const char FIELD = 'A';
    const char ID = 'B';
    const char TEAM = 'A';
  };

}}

#endif
