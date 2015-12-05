#include "SmackTalk.hpp"


using namespace boost::filesystem;
namespace rtx { namespace Smacktalk {
  std::vector<std::string> random_sounds;
  std::vector<std::string> hit_sounds;
  std::vector<std::string> miss_sounds;
  std::string audiosink;
  Timer cooldownTimer;


  void init(){
    srand (time(NULL));
    cooldownTimer.setPeriod(5000);
    audiosink = "alsa_output.pci-0000_00_1b.0.analog-stereo";//gC.getStr("Sound.Sink");
    std::string sounddir = "/home/kristjan/Desktop/Soundid_Futurama/Bender/";//gC.getStr("Sound.Directory");
    std::string rsounds = sounddir + "randomsounds/";
    std::string hsounds = sounddir + "hitsounds/";
    std::string msounds = sounddir + "misssounds/";

    if (is_directory(rsounds))
    {
      for (directory_iterator itr(rsounds); itr!=directory_iterator(); ++itr)
      {
        random_sounds.push_back(itr->path().string());
      }
    }
    if (is_directory(hsounds))
    {
      for (directory_iterator itr(hsounds); itr!=directory_iterator(); ++itr)
      {
        hit_sounds.push_back(itr->path().string());
      }
    }
    if (is_directory(msounds))
    {
      for (directory_iterator itr(msounds); itr!=directory_iterator(); ++itr)
      {
        miss_sounds.push_back(itr->path().string());
      }
    }

  }

  void play(std::string path){
    if (cooldownTimer.isTime()) {
      std::string cmd = "pacmd play-file " + path + " " + audiosink;
      std::system(cmd.c_str());
      cooldownTimer.start();
    }
  }

  void random(){
    int rnd = rand() % (random_sounds.size()-1) + 0;
    play(random_sounds.at(rnd));
  }

  void goalHit(){
    int rnd = rand() % (hit_sounds.size()-1) + 0;
    play(hit_sounds.at(rnd));
  }

  void goalMiss(){
    int rnd = rand() % (miss_sounds.size()-1) + 0;
    play(miss_sounds.at(rnd));
  }

}}
