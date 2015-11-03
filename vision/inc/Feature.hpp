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

  enum FeatureType {
    CORNER,
    GOAL,
    BALL,
    ROBOT
  };

  class Feature {

    public:
      Feature(const Feature&);
      Feature(const unsigned int&, const double&, const FeatureType&);
      ~Feature();

      // Point2D* getPosition() const; // TODO
      std::pair<unsigned int, double> getDeltaVector() const; // (mm, rad)
      unsigned int getDistance() const;
      double getAngle() const;
      FeatureType getType() const;

      void setDistance(const unsigned int&);
      void setAngle(const double&);
      void setType(const FeatureType&);

      bool isCorner() const;
      bool isGoal() const;
      bool isBall() const;
      bool isRobot() const;

      bool isStatic() const;
      bool isMovable() const;

    private:
      unsigned int distance;
      double angle;
      FeatureType type;

  };

};

#endif // RTX_VISION_FEATURE_H
