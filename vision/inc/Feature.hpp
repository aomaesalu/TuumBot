/**
 * @file Feature.hpp
 * Field feature seen on the camera.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_VISION_FEATURE_H
#define RTX_VISION_FEATURE_H

#include "Point2D.hpp"
#include <utility>


namespace rtx {

  class Feature {

    public:
      Feature(const Feature&);
      Feature(const unsigned int&, const double&);
      ~Feature();

      // Point2D* getPosition() const; // TODO
      std::pair<unsigned int, double> getDeltaVector() const; // (mm, rad)
      unsigned int getDistance() const;
      double getAngle() const;

      void setDistance(const unsigned int&);
      void setAngle(const double&);

    private:
      unsigned int distance;
      double angle;

  }

}

#endif // RTX_VISION_FEATURE_H
