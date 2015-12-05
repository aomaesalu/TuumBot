/**
 *  @file Vision.hpp
 *  Computer vision class using YUYV.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 5 December 2015
 */

#ifndef RTX_VISION_VISION_H
#define RTX_VISION_VISION_H

#include <vector>

#include "rtxhal.hpp"
#include "Feature.hpp"
#include "Blob.hpp"


namespace rtx {

  namespace Vision {

    typedef std::vector<Blob*> BlobSet;
    typedef std::vector<Feature> LineSet;
    typedef std::vector<Feature> CornerSet;

    typedef std::vector<std::pair<unsigned int, unsigned int>> SampleRay;
    typedef std::vector<SampleRay> Samples;

    // Vector of rays
    extern std::vector<Samples> flatSamples;
    extern std::vector<Samples> meshSamples;
    extern std::vector<Samples> radialSamples;

    extern BlobSet blobs;
    extern BlobSet blobsBuffer;

    extern LineSet lines;
    extern LineSet linesBuffer;

    extern CornerSet corners;
    extern CornerSet cornersBuffer;

    extern bool editingBlobs;
    extern bool editingLines;
    extern bool editingCorners;

    void setup(const unsigned int&);
    void initialiseFlatSamples(const unsigned int&);
    void initialiseMeshSamples(const unsigned int&);
    void initialiseRadialSamples(const unsigned int&);
    void process(const std::vector<Frame*>&, const std::vector<std::string>&);
    void processCheckerboard(const Frame&, const std::vector<std::string>&, const unsigned int&);

    bool isColored(const Frame&, const std::vector<std::string>&, const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&);
    bool isColored(const Frame&, const std::vector<std::string>&, const unsigned int&, const unsigned int&, const unsigned int&);

    BlobSet getBlobs();
    LineSet getLines();
    CornerSet getCorners();

    void blobDetection(const Frame&, const std::vector<std::string>&, const unsigned int&, const std::vector<unsigned int>&);
    void blobDetection(const Frame&, const std::vector<std::string>&, const unsigned int&, const std::vector<unsigned int>&, const std::vector<Samples>&);
    void blobDetection(const std::vector<Frame*>&, const std::vector<std::string>&, const std::vector<unsigned int>&, const std::vector<Samples>&);

    void lineDetection(const Frame&, const std::vector<std::string>&, const unsigned int&);
    void lineDetection(const Frame&, const std::vector<std::string>&, const unsigned int&, const std::vector<Samples>&);
    void lineDetection(const std::vector<Frame*>&, const std::vector<std::string>&, const std::vector<Samples>&);

    void cornerDetection(const Frame&, const std::vector<std::string>&, const unsigned int&);
    void cornerDetection(const Frame&, const std::vector<std::string>&, const unsigned int&, const std::vector<Samples>&);
    void cornerDetection(const std::vector<Frame*>&, const std::vector<std::string>&, const std::vector<Samples>&);

  };

}

#endif // RTX_VISION_VISION_H
