/**
 *  @file Vision.hpp
 *  Computer vision class using YUYV.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 29 November 2015
 */

#ifndef RTX_VISION_VISION_H
#define RTX_VISION_VISION_H

#include <vector>

#include "rtxhal.hpp"
#include "Feature.hpp"
#include "Blob.hpp"
#include "Line.hpp"
#include "Corner.hpp"


namespace rtx { namespace Vision {

  typedef std::vector<Blob*> BlobSet;
  typedef std::vector<Line*> LineSet;
  typedef std::vector<Corner*> CornerSet;

  typedef std::vector<std::pair<unsigned int, unsigned int>> SampleRay;
  typedef std::vector<SampleRay> Samples;

  // Vector of rays
  extern Samples flatSamples;
  extern Samples meshSamples;
  extern Samples radialSamples;

  extern BlobSet blobs;
  extern BlobSet blobsBuffer;

  extern LineSet lines;
  extern LineSet linesBuffer;

  extern CornerSet corners;
  extern CornerSet cornersBuffer;

  extern bool editingBlobs;
  extern bool editingLines;
  extern bool editingCorners;

  void setup();
  void initialiseFlatSamples();
  void initialiseMeshSamples();
  void initialiseRadialSamples();
  void process(const Frame&, const std::string&);
  void processCheckerboard(const Frame&, const std::string&);

  bool isColored(const Frame&, const std::string&, const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&);
  bool isColored(const Frame&, const std::string&, const unsigned int&, const unsigned int&);

  BlobSet getBlobs();
  LineSet getLines();
  CornerSet getCorners();

  void blobDetection(const Frame&, const std::string&, const std::vector<unsigned int>&);
  void blobDetection(const Frame&, const std::string&, const std::vector<unsigned int>&, const Samples&);

  void lineDetection(const Frame&, const std::string&);
  void lineDetection(const Frame&, const std::string&, const Samples&);

  void cornerDetection(const Frame&, const std::string&);
  void cornerDetection(const Frame&, const std::string&, const Samples&);

}}

#endif // RTX_VISION_VISION_H
