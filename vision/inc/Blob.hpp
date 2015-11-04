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


namespace rtx {

  class Blob {

    public:
      Blob(); // TODO: Form actual necessary constructor
      Blob(const Blob&);
      ~Blob();

      Point2D* getPosition() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getNumberOfPoints() const;
      double getDensity() const;
      // TODO: Add getter for color

      // TODO: Do we need separate setters if we have the necessary constructor?
      void setPosition(const Point2D*);
      void setPosition(const unsigned int&, const unsigned int&);
      void setWidth(const unsigned int&);
      void setHeight(const unsigned int&);
      void setNumberOfPoints(const unsigned int&);
      // TODO: Add setter for color

    private:
      Point2D *position;
      unsigned int width;
      unsigned int height;
      unsigned int numberOfPoints;
      // TODO: Add Color

  };

};

#endif // RTX_VISION_BLOB_H
