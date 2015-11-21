/**
 *  @file Blob.hpp
 *  Blob seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
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

      const std::vector<std::pair<unsigned int, unsigned int>>& getPoints();
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
      double getBoxRatio() const;
      double getDensity() const;

      bool isOrange() const;
      bool isBlue() const;
      bool isYellow() const;

      bool isSameColor(const Blob&) const;
      bool isAbove(const Blob&) const;
      bool isBelow(const Blob&) const;

      bool overlaps(const Blob&) const;
      bool isClose(const Blob&) const;
      void join(Blob&);

      void setColor(const Color&);

    private:
      // Further analysis could take into account different densities in different areas of the blob, different line angles, shape, ...
      std::vector<std::pair<unsigned int, unsigned int>> points;
      Point2D *position;
      unsigned int minX;
      unsigned int maxX;
      unsigned int minY;
      unsigned int maxY;
      unsigned int numberOfPoints;
      Color color;

  };

}

#endif // RTX_VISION_BLOB_H
