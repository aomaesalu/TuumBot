/** @file rtx_ctl.hpp
 *  Tuum Robotex logic controllers.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#ifndef RTX_CTL_H
#define RTX_CTL_H

#include "__future__.hpp"
#include "hal.hpp"

#include "STM.hpp"

#include "Ball.hpp"

namespace rtx { namespace ctl {

  class LSInit : public Controller {
  public:
    LSInit(Context _ctx) : ctx(_ctx) {}

    void run();
    bool isRunnable();
    bool isInterruptable();

  private:
    Context ctx;
  };


  class LSBallLocate : public Controller {
  public:
    LSBallLocate(Context _ctx) : ctx(_ctx) {

    }

    void init();
    void run();
    bool isRunnable();

  private:
    Context ctx;
  };


  class LSBallRetrieve : public Controller {
  public:
    LSBallRetrieve(Context _ctx) : ctx(_ctx) {}

    void init();
    void run();
    bool isRunnable();

  private:
    Context ctx;

    Ball* targetBall;

    Timer targetUpdate;
    Timer ballPickupTimeout;

    bool m_catchingBall;
  };


  class LSGoalLocate : public Controller {
  public:
    LSGoalLocate(Context _ctx) : ctx(_ctx) {}

    void init();
    void run();
    bool isRunnable();

  private:
    Context ctx;
  };


  class LSGoalShoot : public Controller {
  public:
    LSGoalShoot(Context _ctx) : ctx(_ctx) {}

    void init();
    void run();
    bool isRunnable();

  private:
    Context ctx;

    Goal* targetGoal;
  };

}}

#endif // RTX_CTL_H
