/**
 * @file Vision.hpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_VISION_VISION_H
#define RTX_VISION_VISION_H

#include <vector>

#include "Feature.hpp"


namespace rtx {

  namespace Vision {

    typedef std::vector<Blob> BlobSet;
    typedef std::vector<Feature> LineSet;
    typedef std::vector<Feature> CornerSet;

    extern BlobSet blobs;
    extern LineSet lines;
    extern CornerSet corners;

    void setup();
    void process();

    void lineDetection();
    void blobDetection();
    void cornerDetection();
    
  };

};

#endif // RTX_VISION_VISION_H
