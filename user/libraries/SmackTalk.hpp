
#include <ctime>
#include <iostream>     // std::cout
#include <boost/filesystem.hpp>
#include <vector>

#include "tuum_platform.hpp"
#include "hal.hpp"

namespace rtx{ namespace Smacktalk {

      void init();
      void play(std::string);
      void random();
      void goalMiss();
      void goalHit();
}}
