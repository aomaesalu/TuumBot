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
#include "Color.hpp"

#include <vector>


namespace rtx {

  class Blob {

    public:
      Blob(const Blob&);
      Blob(const std::vector<Point2D*>&, const Color&);
      Blob(const std::vector<std::pair<unsigned int, unsigned int>>&, const Color&);
      ~Blob();

      Point2D* getPosition() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getMinX() const;
      unsigned int getMaxX() const;
      unsigned int getMinY() const;
      unsigned int getMaxY() const;
      unsigned int getNumberOfPoints() const;
      Color getColor() const;
      unsigned int getBoxArea() const;
      double getDensity() const;

    private:
      // TODO: Save a list of points, too, for further analysis?
      // Further analysis could take into account different densities in different areas of the blob, different line angles, shape, ...
      Point2D *position;
      unsigned int minX;
      unsigned int maxX;
      unsigned int minY;
      unsigned int maxY;
      unsigned int numberOfPoints;
      Color color;

  };

};

#endif // RTX_VISION_BLOB_H
