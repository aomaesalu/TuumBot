#include <ctime>
#include "SmackTalk.hpp"

using namespace std;

int main(){
  printf("Soundtest\n");
  rtx::Smacktalk::SmackTalk *smacktalk = new rtx::Smacktalk::SmackTalk();
  smacktalk->init();

  bool running = true;
  while(running) {
    usleep(1000000);
    printf("Calling goalHit!");
    clock_t begin = clock();
    smacktalk->goalHit();

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("Time elapsed: %G", elapsed_secs);
  }

  return 0;
}
