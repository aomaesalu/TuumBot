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

  State::State(std::string name, STM* stm) {
    m_name = name;
    m_stm = stm;
    m_last = nullptr;
    m_next = nullptr;
  }

  STM::STM() {
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

  State* STM::createState(std::string name) {
    State* st = new State(name, this);
    m_states.push_back(st);
    return st;
  }

  State* State::getLastState() {
    return m_last;
  }

  State* State::getNextState() {
    return m_next;
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

  void STM::process() {
    if(m_state == nullptr) return;

    State* st_ptr;
    while(!m_state->canEnter()) {
      if(m_state->getLastState() == nullptr) return;

      st_ptr = m_state->getLastState();
      if(st_ptr != nullptr)
        setState(st_ptr);
    }

    m_state->process();

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
}
