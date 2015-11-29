#include <cstdlib>
#include <iostream>
#include <string>

#include "__future__.hpp"
#include "STM.hpp"

using namespace std;
using namespace rtx;

int main(int argc, char* argv[]) {
  printf("Running STM tests...\n");

  STM stm;
  State* st, *st2;
  Context ctx;

  st = stm.createState("LSInit");
  stm.setState(st);

  // Logic warmup
  class LSInit : public Controller {
  public:
    void run() {
      switch(m_ctx.phase) {
        case CP_INIT:
          std::cout << m_ctx.st->getName() << ": Initializing" << std::endl;
          std::cout << m_ctx.st->getName() << ": Set motion target" << std::endl;
          m_ctx.phase = CP_RUN;
          break;
        case CP_RUN:
          std::cout << m_ctx.st->getName() << ": Check motion" << std::endl;
          break;
        default:
          break;
      }
    }

    bool isRunnable() {
      return true;
    }

    LSInit(Context ctx) : m_ctx(ctx) {}
  private:
    Context m_ctx;
  };

  ctx.st = st;
  st->addController(new LSInit(ctx));


  // Ball search
  st2 = stm.createState("LSBallLocate");
  st->setNextState(st2);
  st = st2;

  class LSBallLocate : public Controller {
  public:
    void run() {
      std::cout << ctx.st->getName() << " RUN()" << std::endl;
    }

    bool isRunnable() {
      std::cout << ctx.st->getName() << " CHECK()" << std::endl;
      return true;
    }

    LSBallLocate(Context _ctx) : ctx(_ctx) {}
  private:
    Context ctx;
  };

  ctx.st = st;
  st->addController(new LSBallLocate(ctx));


  // Ball retrieval
  st2 = stm.createState("LSBallRetrieve");
  st2->setLastState(st);
  st->setNextState(st2);
  st = st2;

  class LSBallRetrieve : public Controller {
  public:
    void run() {
      std::cout << ctx.st->getName() << " RUN()" << std::endl;
    }

    bool isRunnable() {
      std::cout << ctx.st->getName() << " CHECK()" << std::endl;
      if(ctx.st->m_stm->getState() == ctx.st) return false;
      return true;
    }

    LSBallRetrieve(Context _ctx) : ctx(_ctx) {}
  private:
    Context ctx;
  };

  ctx.st = st;
  st->addController(new LSBallRetrieve(ctx));


  bool running = true;
  for(int i=0; i < 4; i++) {
    stm.process();
  }


  printf("STM tests done.\n");
  return 0;
}
