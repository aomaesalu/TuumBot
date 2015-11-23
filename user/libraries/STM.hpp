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
#include <functional>
#include <string>
#include <iostream>

namespace rtx {

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

  class Controller {
  public:
    virtual int run() {}
    virtual void init() {}
    virtual bool isRunnable() { return true; }
    virtual bool isInterruptable() { return true; }
  };

  class State {
  private:
    std::string m_name;

    CtrlSet m_controllers;

    State* m_last;
    State* m_next;
  public:
    STM* m_stm;

    State(std::string name, STM* stm);

    std::string getName() { return m_name; }

    void addController(Controller* ctrl) {
      m_controllers.push_back(ctrl);
    }

    void setLastState(State* st) { m_last = st; }
    void setNextState(State* st) { m_next = st; }

    State* getLastState();
    State* getNextState();

    bool canEnter();
    bool canExit();

    void setup();
    void process();
  };

  class STM {
  public:
    STM();

    void setState(State* st);
    void addRootState(State* st);

    State* getState() { return m_state; }

    State* createState(std::string);

    void process();
  private:
    StateSet m_states;
    StateSet m_rootStates;

    State* m_state;
  };

}

#endif
