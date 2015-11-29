/** @file tfb_logic.cpp
 *  Team football logic module implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "RefereeListener.hpp"
#include "STM.hpp"

#include "rtx_tfb.hpp"

using namespace rtx::hal;

namespace rtx { namespace TFBLogic {

  enum GameState {
    GS_NONE,

    GS_KICKOFF,
    GS_KICKOFF_GOAL,        // Väravaesine lahtilöök

    GS_FREEKICK_INDIRECT,   // Vabalöök
    GS_FREEKICK_DIRECT,     // Karistuslöök
    GS_THROWIN,             // Küljesissevise

    // ...
  };

  GameState gameState;

  /*

  STM kickoff_kicker;
  STM game_defender;


  STM kickoff_receiver;
  STM game_attacker;

  */

  bool logicRunning;
  STM* logicProcess;

  void start() {
    std::cout << "Logic resume" << std::endl;
    logicRunning = true;
  }

  void stop() {

    std::cout << "Logic pause" << std::endl;
    logicRunning = false;
  }

  void updateGameState(GameState gs, bool to_our_team=false) {
    gameState = gs;

    switch(gameState) {
      case GS_KICKOFF:
        // logicProcess = X
        std::cout << "Transition to ";
        if(to_our_team) std::cout << "our";
        else std::cout << "opposing";
        std::cout << " team's kickoff." << std::endl;
        break;
      case GS_KICKOFF_GOAL:
        break;
      case GS_FREEKICK_DIRECT:
        break;
      case GS_FREEKICK_INDIRECT:
        break;
      case GS_THROWIN:
        break;
      case GS_NONE:
      default:
      logicProcess = nullptr;
        break;
    }
  }

  void init_referee_signals() {
    RefereeListener* ref = hw.getRefListener();

    ref->registerCallback(REF_START, [=](RefCommand rcmd){
      start();
    });

    ref->registerCallback(REF_STOP, [=](RefCommand rcmd){
      stop();
    });

    ref->registerCallback(REF_KICKOFF, [=](RefCommand rcmd){
      RefereeListener* r = hw.getRefListener();
      updateGameState(GS_KICKOFF, rcmd.target.team == r->m_team);
    });
  }

  void setup() {
    stop();
    updateGameState(GS_NONE);
    init_referee_signals();
  }

  void process() {
    if(logicRunning) {
      if(logicProcess != nullptr) logicProcess->process();
    }
  }

}}
