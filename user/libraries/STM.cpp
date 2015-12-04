/** @file STM.cpp
 *  State machine implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#include <string>

#include "STM.hpp"

namespace rtx {

  int State::priority_seq = 0;

  State::State(std::string name, STM* stm) {
    m_name = name;
    m_stm = stm;
    m_last = nullptr;
    m_next = nullptr;

    m_priority = State::priority_seq++;
  }

  State* State::getLastState() {
    return m_last;
  }

  State* State::getNextState() {
    return m_next;
  }

  void State::addController(Controller* ctrl) {
    m_controllers.push_back(ctrl);
    ctrl->onStateAttach(this, m_stm);
  }

  bool State::canEnter() {
    for(auto& ctrl : this->m_controllers) {
      if(ctrl->isRunnable() == false) return false;
    }
    return true;
  }

  bool State::canExit() {
    for(auto& ctrl : this->m_controllers) {
      if(ctrl->isInterruptable() == false) return false;
    }
    return true;
  }

  void State::setup() {
    for(auto& ctrl : this->m_controllers) {
      ctrl->init();
    }
  }

  void State::process() {
    for(auto& ctrl : this->m_controllers) {
      ctrl->run();
    }
  }

  /**
   *  State Machine methods implementation.
   */
  STM::STM():
    EventEmitter()
  {
    m_state = nullptr;
  }

  void STM::setState(State* st) {
    std::cout << "[STM]Transition: ";
    if(m_state != nullptr) {
      std::cout << m_state->getName() << " => ";
    }
    std::cout << st->getName() << std::endl;

    m_state = st;
    st->setup();
  }

  void STM::addRootState(State* st) {
    m_rootStates.push_back(st);
  }

  State* STM::createState(std::string name) {
    State* st = new State(name, this);
    m_states.push_back(st);
    return st;
  }

  void STM::setup() {
    EventEmitter::init();
    //if(m_states.size() > 0) setState(m_states[0]);
  }

  void STM::process() {
    // Process root states
    for(auto& tmp_st_ptr : m_rootStates) {
      if(tmp_st_ptr->getPriority() < m_state->getPriority()) continue;
      if(tmp_st_ptr->canEnter() && tmp_st_ptr != m_state) {
        setState(tmp_st_ptr);
        return;
      }
    }

    if(m_state == nullptr) return;

    // Process current state conditions
    State* st_ptr;
    while(!m_state->canEnter()) {
      if(m_state->getLastState() == nullptr) return;

      st_ptr = m_state->getLastState();
      if(st_ptr != nullptr)
        setState(st_ptr);
    }

    // Process current state
    m_state->process();

    // Process next state conditions
    if(m_state->canExit()) {
      st_ptr = m_state->getNextState();
      if(st_ptr != nullptr) {
        if(st_ptr->canEnter()) {
          setState(st_ptr);
          m_state->process();
        }
      }
    }

    return;
  }


  /**
   *  Event emitter methods implementation.
   *  TODO: refactor code into separate library
   */
  void EventEmitter::init() {
    m_eventMap.clear();
    m_eventListeners.clear();
    m_EventListenerIDSeq = 1;
  }

  EventID EventEmitter::registerEvent(EventName evn) {
    m_eventMap[evn] = m_EventListenerIDSeq++;
  }

  EventID EventEmitter::getEventID(EventName evn) {
    auto it = m_eventMap.find(evn);
    if(it != m_eventMap.end()) return it->second;
    return registerEvent(evn);
  }

  EventHandler EventEmitter::getEventHandler(EventID id) {
    auto it = m_eventListeners.find(id);
    if(it != m_eventListeners.end()) return it->second;
    throw(-1);
  }

  void EventEmitter::registerEventListener(EventID id, EventHandler evh) {
    if(id == 0) return;
    m_eventListeners[id] = evh;
  }

  void EventEmitter::emitEvent(EventName evn) {
    EventID id = getEventID(evn);
    if(id == 0) return;

    EventHandler evh;
    try { evh = getEventHandler(id); }
    catch (int err) {
      std::cout << "[EventEmitter]Error " << err
                << ": Unknown handler id " << id << std::endl;
      return;
    }

    evh();
  }

}
