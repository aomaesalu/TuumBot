#ifndef RTX_REF_H
#define RTX_REF_H

#include <functional>
#include <string>
#include <map>

#include "tuum_communication.hpp"

#include "SerialPort.hpp"

namespace rtx { namespace hal {

  enum RefereeSignal {
    REF_VOID,
    REF_START,
    REF_STOP,
    REF_PLACEDBAL,
    REF_ENDHALF,
    REF_KICKOFF,
    REF_IFREEK,
    REF_DFREEK,
    REF_GOALK,
    REF_THROWIN,
    REF_CORNERK,
    REF_PENALTY,
    REF_GOAL,
    REF_CARDY,

  };

  typedef std::map<std::string, RefereeSignal> RefSigMap;

  const RefSigMap refSigMap = {
    {"START----", REF_START},
    {"STOP-----", REF_STOP},
    {"PLACEDBAL", REF_PLACEDBAL},
    {"ENDHALF--", REF_ENDHALF},
    {"KICKOFF-", REF_KICKOFF},
    {"IFREEK--", REF_IFREEK},
    {"DFREEK--", REF_DFREEK},
    {"GOALK---", REF_GOALK},
    {"THROWIN-", REF_THROWIN},
    {"CORNERK-", REF_CORNERK},
    {"PENALTY-", REF_PENALTY},
    {"GOAL---- ", REF_GOAL},
    {"CARDY---", REF_CARDY}

  };

  // Field, Robot ID, Team
  struct RefTarget {
    char field;
    char teams;
    char team;
    char id;
  };

  struct RefCommand {
    RefereeSignal sig;
    RefTarget target;
  };

  typedef void (*VoidFn)(RefCommand);
  typedef std::pair<RefereeSignal, VoidFn> SignalCallback;

  //FIXME: This is not regularly working over different threads.
  static std::map<RefereeSignal, VoidFn> callbacks;

  class RefereeListener : public SerialPort {
  public:
    char m_field;
    char m_team;
    char m_robot;

  private:

  public:
    RefereeListener();

    void init(std::string, int);

    void on_receive_(const std::string&);

    void registerCallback(const RefereeSignal, VoidFn);
    void signal(RefCommand);

    RefCommand parseCommand(std::string);

    void sendTuumMessage(comm::TuumMessage);
    void handleTuumMessage(std::string);

    void sendAck();
  };

}}

#endif // RTX_REF_H
