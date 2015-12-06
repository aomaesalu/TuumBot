/** @file rtx_ctl.hpp
 *  Tuum Robotex logic controllers.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 17. November 2015
 */

#ifndef RTX_CTL_H
#define RTX_CTL_H

#include "rtxmath.hpp"
#include "tuum_communication.hpp"

#include "hal.hpp"

#include "STM.hpp"

#include "Ball.hpp"
#include "Goal.hpp"


namespace rtx {

  namespace Motion {
    class TwitchScan {
    private:
      Timer motionTimer;
      Timer visionTimer;
      bool wait_for_vision;

      int m_spv = 5;
      int m_sps = 40;

      void _init();
    public:
      void init();
      void init(int, int);

      void run();
    };


  }

}

namespace rtx { namespace ctl {

  class LSInit : public Controller {
    public:
      LSInit(Context _ctx) : ctx(_ctx) {}

      int run();
      bool isRunnable();
      bool isInterruptable();

    private:
      Context ctx;
  };


  /**
   *
   *  Ball handling logic controllers
   *
   */
  class LSBallLocate : public Controller {
    public:
      LSBallLocate(Context _ctx) : ctx(_ctx) {

      }

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;

      Motion::TwitchScan twitchScanner;

      Timer positionTimeout;
      Timer positionUpdate;
      Vec2i positionOffset;
      bool move;
  };


  class LSBallNavigator : public Controller {
    public:
      LSBallNavigator(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;
  };

  class LSBallPicker : public Controller {
    public:
      LSBallPicker(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;
  };


  /**
   *
   *  Goal handling logic controllers
   *
   */
  class LSGoalLocate : public Controller {
    public:
      LSGoalLocate(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;
      Motion::TwitchScan twitchScanner;
  };


  class LSGoalShoot : public Controller {
    public:
      LSGoalShoot(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;

      Goal* targetGoal;
  };

  class LSGoalee : public Controller {
    public:
      LSGoalee(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();

    private:
      Context ctx;
  };


  /**
   *
   *  Team interaction logic controllers
   *
   */
  class LSAllyFind : public Controller {
    public:
      LSAllyFind(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;
      Motion::TwitchScan twitchScanner;
  };

  class LSAllyLocate : public Controller {
    public:
      LSAllyLocate(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;
      Motion::TwitchScan twitchScanner;
  };

  class LSAllyAim : public Controller {
    public:
      LSAllyAim(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;
      Motion::TwitchScan twitchScanner;
  };

  class LSAllyPass : public Controller {
    public:
      LSAllyPass(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

    private:
      Context ctx;

      comm::TuumMessage tms;
      Timer commTimeout;
      bool finish;
  };

  class LSAllyReceive : public Controller {
    public:
      LSAllyReceive(Context _ctx) : ctx(_ctx) {}

      void init();
      int run();
      bool isRunnable();

      void onPassSignal(comm::TuumMessage);

    private:
      Context ctx;

    public:
      bool finish;
  };

}}

#endif // RTX_CTL_H
