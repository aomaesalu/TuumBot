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

  class Vision {

    public:
      Vision();
      ~Vision();

      std::vector<Blob*> getBlobs() const;
      std::vector<Feature*> getLines() const;
      std::vector<Feature*> getCorners() const;

      void process();

    private:
      std::vector<Blob*> blobs;
      std::vector<Feature*> lines;
      std::vector<Feature*> corners;

      void lineDetection();
      void blobDetection();
      void cornerDetection();

  };

};

#endif // RTX_VISION_VISION_H
