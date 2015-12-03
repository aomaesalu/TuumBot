
#include <ctime>
#include <iostream>     // std::cout
#include <boost/filesystem.hpp>
#include <vector>

#include "tuum_platform.hpp"

namespace rtx{ namespace Smacktalk {
  class SmackTalk{
    private:
      std::vector<std::string> random_sounds;
      std::vector<std::string> hit_sounds;
      std::vector<std::string> miss_sounds;
      time_t started;
      std::string audiosink;

    public:
      SmackTalk();
      void init();
      void play(std::string);
      void random();
      void goalHit();
      void goalMiss();
  };
}}
