/** @file rtx_fb.cpp
 *  Robotex 1vs1 football logic implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#include "STM.hpp"

#include "rtx_ctl.hpp"
#include "rtx_fb.hpp"

namespace rtx { namespace FBLogic {

  STM stm;

  void setup() {
    State* st, *st2;
    Context ctx;

    st = stm.createState("STInit");
    stm.setState(st);
    ctx.st = st;
    st->addController(new ctl::LSInit(ctx));

    st2 = stm.createState("STBallLocate");
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallLocate(ctx));

    st2 = stm.createState("STBallRetrieve");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSBallRetrieve(ctx));

    st2 = stm.createState("STGoalLocate");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSGoalLocate(ctx));
    stm.addRootState(st);


    st2 = stm.createState("STGoalShoot");
    st2->setLastState(st);
    st->setNextState(st2);
    st = st2;
    ctx.st = st;
    st->addController(new ctl::LSGoalShoot(ctx));

  }

  void process() {
    stm.process();
  }

}}
