/**
 * @file Vision.cpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
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

    void process(CameraDevice *camera) {
      blobDetection(camera);
      lineDetection(camera);
      cornerDetection(camera);
    }

    void lineDetection(CameraDevice *camera) {
      // TODO
    }

    void cornerDetection(CameraDevice *camera) {
      // TODO
    }

    void blobDetection(CameraDevice *camera) {
      // TODO
    }

  };

};
