/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 *  @date 11. November 2015
 */

#include "Vision.hpp"


namespace rtx {

  namespace Vision {

    BlobSet blobs;
    LineSet lines;
    CornerSet corners;

    /*void emptyVector(std::vector<Feature*> &vector) {
      for (std::vector<Feature*>::iterator i = vector.begin(); i != vector.end();
           ++i) {
        delete *i;
      }
      vector.clear();
    }*/

    void setup() {
      printf("\033[1;32m");
      printf("[Vision::setup()]Ready.");
      printf("\033[0m\n");
    }

    void process(const Frame &frame) {
      blobDetection(frame);
      lineDetection(frame);
      cornerDetection(frame);
    }

    void lineDetection(const Frame &frame) {
      // TODO
    }

    void cornerDetection(const Frame &frame) {
      // TODO
    }

    void blobDetection(const Frame &frame) {
      // TODO
    }

  };

};
