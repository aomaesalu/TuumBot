#include "SmackTalk.hpp"


using namespace boost::filesystem;
namespace rtx { namespace Smacktalk {
SmackTalk::SmackTalk(){

}
void SmackTalk::init(){
  srand (time(NULL));
  started = time(NULL);
  audiosink = gC.getStr("Sound.Sink");
  std::string sounddir = gC.getStr("Sound.Directory");
  std::string rsounds = sounddir + "/randomsounds";
  std::string hsounds = sounddir + "/hitsounds";
  std::string msounds = sounddir + "/misssounds";
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

void SmackTalk::play(std::string path){
  if (difftime(time(NULL), started) > 5){
    started = time(NULL);
    std::string cmd = "pacmd play-file " + path + audiosink;
    std::system(cmd.c_str());
  }
}

void SmackTalk::random(){
  int rnd = rand() % (random_sounds.size()-1) + 0;
  this->play(random_sounds.at(rnd));
}

void SmackTalk::goalHit(){
  int rnd = rand() % (hit_sounds.size()-1) + 0;
  this->play(hit_sounds.at(rnd));
}

void SmackTalk::goalMiss(){
  int rnd = rand() % (miss_sounds.size()-1) + 0;
  this->play(miss_sounds.at(rnd));
}

}}
