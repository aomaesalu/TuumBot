/**
 * @file Vision.hpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 * @date 11. November 2015
 */

#ifndef RTX_VISION_VISION_H
#define RTX_VISION_VISION_H

#include <vector>

#include "rtxhal.hpp"
#include "Feature.hpp"
#include "Blob.hpp"


namespace rtx {

  namespace Vision {

    typedef std::vector<Blob> BlobSet;
    typedef std::vector<Feature> LineSet;
    typedef std::vector<Feature> CornerSet;

    extern BlobSet blobs;
    extern LineSet lines;
    extern CornerSet corners;

    void setup();
    void process(const Frame&, const std::string&);

    bool isColored(const Frame&, const std::string&, const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&);
    bool isColored(const Frame&, const std::string&, const unsigned int&, const unsigned int&);

    void blobDetection(const Frame&, const std::string&);
    void blobDetection(const Frame&, const std::string&, const std::vector<Point2D>&);

    void lineDetection(const Frame&, const std::string&);
    void lineDetection(const Frame&, const std::string&, const std::vector<Point2D>&);

    void cornerDetection(const Frame&, const std::string&);
    void cornerDetection(const Frame&, const std::string&, const std::vector<Point2D>&);

  };

};

#endif // RTX_VISION_VISION_H
