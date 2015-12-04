/** @file tfb_logic.hpp
 *  Team football logic module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 2. November 2015
 */

#ifndef RTX_TFB_LOGIC_H
#define RTX_TFB_LOGIC_H

#include "application.hpp"

namespace rtx { namespace TFBLogic {

  enum GameState {
    STOP,
    PLACEDBALL,
    START,
  };

  enum GamePhase {
    NONE,
    GAME,

    KICKOFF,
    KICKOFF_GOAL,        // Väravaesine lahtilöök

    FREEKICK_INDIRECT,   // Vabalöök
    FREEKICK_DIRECT,     // Karistuslöök
    THROWIN,             // Küljesissevise

    // ...?
  };

  void setup();
  void process();

  void stop();
  void start();

  void init_referee_signals();
  void load_logic();

  void updateGameState(GameState);
  void updateGamePhase(GamePhase, bool = false);

}}

#endif
