/**
 *  @file Feature.hpp
 *  Field feature seen on the camera.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 6 December 2015
 */

#ifndef RTX_VISION_FEATURE_H
#define RTX_VISION_FEATURE_H

#include "Point2D.hpp"

#include <utility>


namespace rtx { namespace Vision {

  class Feature {

    public:
      Feature(const Feature&);
      Feature(const unsigned int&, const double&, const unsigned int&);
      ~Feature();

      unsigned int getDistance() const;
      double getAngle() const;
      unsigned int getCameraID() const;

      std::pair<double, double> getRelativePoint() const;

      void setDistance(const unsigned int&);
      void setAngle(const double&);

    protected:
      unsigned int distance;
      double angle;
      unsigned int cameraID;

  };

}}

#endif // RTX_VISION_FEATURE_H
