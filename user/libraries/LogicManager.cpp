/** @file LogicManager.cpp
 *  Logic machines loading class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 28. November 2015
 */

#include "rtx_ctl.hpp"

#include "LogicManager.hpp"

namespace rtx {

  /**
   *  Controllers:
   *    LSBallScan: Find ball
   *    LSBallKickPrepare: Move in front of ball
   */
  STM LogicManager::loadKickoffPrepare() {
    STM stm;
    State *st, *st2;
    Context ctx;

    st = stm.createState("STBallLocate");
    ctx.st = st;
    st->addController(new ctl::LSBallLocate(ctx));

    st2 = stm.createState("STBallPrepare");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallPrepare(ctx));

    return stm;
  }

  /**
   *  Controllers:
   *    LSAllyLocate: Find ally robot
   *    LSAllyPass: Pass to ally robot
   */
  STM LogicManager::loadKickoff() {
    STM stm = LogicManager::loadKickoffPrepare();
    State *st2, *st; // = stm.getLastState();
    Context ctx;

    st2 = stm.createState("STBallRetrieve");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallRetrieve(ctx));

    st2 = stm.createState("STAllyLocate");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSAllyLocate(ctx));

    st2 = stm.createState("STAllyPass");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSAllyPass(ctx));

    return stm;
  }

}
