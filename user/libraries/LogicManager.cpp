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
  STM* LogicManager::loadKickoffPasserPrepare() {
    STM* stm = new STM();
    State *st, *st2;
    Context ctx;

    st = stm->createState("STBallLocate");
    ctx.st = st;
    st->addController(new ctl::LSBallLocate(ctx));

    st2 = stm->createState("STBallNavigator");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallNavigator(ctx));

    return stm;
  }

  STM* LogicManager::loadKickoffReceiverPrepare() {
    STM* stm = new STM();
    State *st, *st2;
    Context ctx;

    st = stm->createState("STAllyFind");
    ctx.st = st;
    st->addController(new ctl::LSAllyFind(ctx));


    st2 = stm->createState("STAllyBallReceive");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSAllyAim(ctx));
    st->addController(new ctl::LSAllyReceive(ctx));

    return stm;
  }


  /**
   *  Controllers:
   *    LSAllyLocate: Find ally robot
   *    LSAllyPass: Pass to ally robot
   */
  STM* LogicManager::loadKickoffPasser() {
    STM* stm = LogicManager::loadKickoffPasserPrepare();
    State *st2, *st = stm->stateStackPeek();
    Context ctx;

    st2 = stm->createState("STBallPicker");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallPicker(ctx));

    st2 = stm->createState("STAllyLocate");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSAllyLocate(ctx));

    st2 = stm->createState("STAllyPass");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSAllyPass(ctx));

    return stm;
  }


  /**
   *  Controllers:
   *    LSAllyLocate: Find ally robot
   *    LSAllyRecv: Pass to ally robot
   */
  STM* LogicManager::loadKickoffReceiver() {
    STM* stm = LogicManager::loadKickoffReceiverPrepare();
    State *st2, *st = stm->stateStackPeek();
    Context ctx;

    return stm;
  }


  /**
   *  Controllers:
   *    LSBallLocate: Find ball
   *    LSBallNavigator: Move in front of ball
   *    LSBallPicker: Pickup ball
   *    LSGoalLocate (+rootstate): Find opposing goal
   *    LSGoalShoot: Aim & kick at goal
   */
  STM* LogicManager::loadOffensivePlay() {
    STM* stm = new STM();
    State* st, *st2;
    Context ctx;

    st = stm->createState("STInit");
    stm->setState(st);
    ctx.st = st;
    st->addController(new ctl::LSInit(ctx));

    st2 = stm->createState("STBallLocate");
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallLocate(ctx));

    st2 = stm->createState("STBallNavigator");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallNavigator(ctx));


    st2 = stm->createState("STBallPicker");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallPicker(ctx));

    st2 = stm->createState("STGoalLocate");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSGoalLocate(ctx));
    stm->addRootState(st);


    st2 = stm->createState("STGoalShoot");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSGoalShoot(ctx));

    return stm;
  }

}
