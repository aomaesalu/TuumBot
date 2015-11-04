/**
 * @file Blob.hpp
 * Blob seen in the camera frame.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_VISION_BLOB_H
#define RTX_VISION_BLOB_H

#include "Point2D.hpp"

#include <vector>


namespace rtx {

  class Blob {

    public:
      Blob(const Blob&);
      Blob(const std::vector<Point2D*>&);
      Blob(const std::vector<std::pair<unsigned int, unsigned int>>&);
      ~Blob();

      Point2D* getPosition() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getNumberOfPoints() const;
      Color getColor() const;
      unsigned int getBoxArea() const;
      double getDensity() const;


    private:
      Point2D *position;
      unsigned int width;
      unsigned int height;
      unsigned int numberOfPoints;
      Color color;

  };

};

#endif // RTX_VISION_BLOB_H
