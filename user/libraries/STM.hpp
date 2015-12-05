/** @file STM.hpp
 *  Simple state machine for behaviour control.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#ifndef RTX_STM_H
#define RTX_STM_H

#include <vector>
#include <map>
#include <functional>
#include <string>
#include <iostream>
#include <stdint.h>

//TODO: Implement separate dedicated event emitter base class
namespace rtx {

  typedef uint32_t EventID;
  typedef std::string EventName;
  typedef void (*EventHandler)();

  class EventEmitter {
  protected:
    std::map<EventName, uint32_t> m_eventMap;
    std::map<uint32_t, EventHandler> m_eventListeners;

    EventID m_EventListenerIDSeq;
  public:
    void init();

    EventID registerEvent(EventName);
    EventID getEventID(EventName);

    EventHandler getEventHandler(EventID);

    void registerEventListener(EventID, EventHandler);
    void deregisterEventListener(EventID);

    void emitEvent(EventName);
  };

  class STM;
  class State;
  class Controller;

  typedef std::function<void()> Functor;

  typedef std::function<bool()> ConditionFunc;
  typedef std::vector<ConditionFunc> ConditionSet;

  typedef std::vector<Controller*> CtrlSet;
  typedef std::vector<State*> StateSet;


  enum CtrlPhase {
    CP_INIT,
    CP_RUN,
    CP_DONE,
  };

  struct Context {
    CtrlPhase phase = CP_INIT;
    State* st;
  };

  class State {
  private:
    static int priority_seq;

    std::string m_name;
    int m_priority;

    CtrlSet m_controllers;

    State* m_last;
    State* m_next;
  public:
    STM* m_stm;

    State(std::string name, STM* stm);

    std::string getName() { return m_name; }
    int getPriority() { return m_priority; }

    void setLastState(State* st) { m_last = st; }
    void setNextState(State* st) { m_next = st; }

    void addController(Controller*);

    State* getLastState();
    State* getNextState();

    bool canEnter();
    bool canExit();

    void setup();
    void process();
  };

  class STM : public EventEmitter {
  public:
    STM();

    void setState(State* st);
    void addRootState(State* st);

    State* getState() { return m_state; }
    State* getLastState();
    State* stateStackPeek();

    State* createState(std::string);

    void setup();
    void process();
  private:
    StateSet m_states;
    StateSet m_rootStates;

    State* m_state;
  };


  class Controller {
  protected:
    State* m_st = nullptr;
    STM* m_stm = nullptr;

  public:
    virtual int run() {}
    virtual void init() {}
    virtual bool isRunnable() { return true; }
    virtual bool isInterruptable() { return true; }

    //m_stm->registerEvent("DONE")
    //m_stm->emitEvent("DONE")
    //m_stm->addEventListener(EventName, EventHandler)
    virtual void onStateAttach(State* st, STM* stm) {
      m_st = st;
      m_stm = stm;
    };

    virtual void emit(EventName evn) {
      if(m_stm != nullptr) m_stm->emitEvent(evn);
    }

    virtual void addHandler(EventName evn, EventHandler evh) {
      m_stm->registerEventListener(m_stm->getEventID(evn), evh);
    }
  };

}

#endif
