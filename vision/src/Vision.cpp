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
      // TODO
    }

    void process() {
      blobDetection();
      lineDetection();
      cornerDetection();
    }

    void lineDetection() {
      // TODO
    }

    void cornerDetection() {
      // TODO
    }

    void blobDetection() {
      // TODO
    }

  };

};
