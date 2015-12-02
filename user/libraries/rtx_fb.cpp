/** @file rtx_fb.cpp
 *  Robotex 1vs1 football logic implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#include "STM.hpp"
#include "LogicManager.hpp"

#include "rtx_fb.hpp"

using namespace rtx::hal;

namespace rtx { namespace FBLogic {

  enum GameState {
    STOP,
    RUN,
  };

  enum GamePhase {
    GAME,
    PENALTY,
  };

  GameState gmState;
  GamePhase gmPhase;


  // Preload logic
  STM lg_offense = LogicManager::loadOffensivePlay();


  STM* logicProcess = nullptr;


  void start() {
    std::cout << "Start game." << std::endl;
    gmState = GameState::RUN;
  }

  void stop() {
    std::cout << "Stop game." << std::endl;
    Motion::stop();
    hw.getMainBoard()->stopDribbler();
    gmState = GameState::STOP;
  }

  void updateGamePhase(GamePhase gp, bool to_our_team=false) {
    gmPhase = gp;

    switch(gmPhase) {
      case GamePhase::GAME:
        logicProcess = &lg_offense;
        //FIXME: reset state machine
        //logicProcess.init();
        break;
      case GamePhase::PENALTY:
      default:
        logicProcess = nullptr;
        break;
    }
  }

  void updateGameState(GameState gs) {
    gmState = gs;
  }

  void init_referee_signals() {
    RefereeListener* ref = hw.getRefListener();

    ref->registerCallback(REF_START, [=](RefCommand rcmd){
      start();
    });

    ref->registerCallback(REF_STOP, [=](RefCommand rcmd){
      stop();
    });
  }

  void setup() {
    stop();
    updateGameState(GameState::STOP);
    updateGamePhase(GamePhase::GAME);
    init_referee_signals();
  }

  void process() {
    switch(gmState) {
      case GameState::STOP:
        break;
      case GameState::RUN:
        if(logicProcess != nullptr) logicProcess->process();
        break;
      default:
        break;
    }
  }

}}
