/** @file tfb_logic.cpp
 *  Team football logic module implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#include "RefereeListener.hpp"
#include "STM.hpp"
#include "LogicManager.hpp"

#include "rtx_tfb.hpp"

using namespace rtx::hal;

namespace rtx { namespace Logic {

  enum GameState {
    GS_STOP,
    GS_PLACEDBALL,
    GS_START,
  };

  enum GamePhase {
    GP_NONE,

    GP_KICKOFF,
    GP_KICKOFF_GOAL,        // Väravaesine lahtilöök

    GP_FREEKICK_INDIRECT,   // Vabalöök
    GP_FREEKICK_DIRECT,     // Karistuslöök
    GP_THROWIN,             // Küljesissevise

    // ...
  };


  GameState gmState;
  GamePhase gmPhase;

  // Preload logic trees
  STM lg_kickoff = LogicManager::loadKickoff();


  STM* logicProcess;

  void start() {
    std::cout << "Start game." << std::endl;
    gmState = GS_START;
  }

  void stop() {
    std::cout << "Stop game." << std::endl;
    gmState = GS_STOP;
  }

  void updateGamePhase(GamePhase gp, bool to_our_team=false) {
    gmPhase = gp;

    switch(gmPhase) {
      case GP_KICKOFF:
        logicProcess = &lg_kickoff;

        std::cout << "Transition to ";
        if(to_our_team) std::cout << "our";
        else std::cout << "opposing";
        std::cout << " team's kickoff." << std::endl;
        break;
      case GP_KICKOFF_GOAL:
        break;
      case GP_FREEKICK_DIRECT:
        break;
      case GP_FREEKICK_INDIRECT:
        break;
      case GP_THROWIN:
        break;
      case GP_NONE:
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

    ref->registerCallback(REF_KICKOFF, [=](RefCommand rcmd){
      RefereeListener* r = hw.getRefListener();
      updateGamePhase(GP_KICKOFF, rcmd.target.team == r->m_team);
    });
  }

  void setup() {
    stop();
    updateGameState(GS_STOP);
    updateGamePhase(GP_NONE);
    init_referee_signals();
  }

  void process() {
    switch(gmState) {
      case GS_STOP:
        break;
      case GS_PLACEDBALL:
        //TODO: prepare for game phase?
        break;
      case GS_START:
        if(logicProcess != nullptr) logicProcess->process();
        break;
    }
  }

}}
