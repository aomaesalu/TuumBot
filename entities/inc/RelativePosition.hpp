/**
 * @file RelativePosition.hpp
 * Relative position class.
 * TODO: Implement elsewhere.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_RELATIVE_POSITION_H
#define RTX_ENTITIES_RELATIVE_POSITION_H

#include "Point2D.hpp"

#include <vector>


namespace rtx {

  class RelativePosition {
    public:
      RelativePosition(const RelativePosition&);
      RelativePosition(const unsigned int&, const double&);
      ~RelativePosition();

      std::pair<unsigned int, double> getDeltaVector() const; // (mm, rad)
      unsigned int getDistance() const;
      double getAngle() const;

      void setDistance(const unsigned int&);
      void setAngle(const double&);

    private:
      unsigned int distance;
      double angle;

  };

};

#endif // RTX_ENTITIES_RELATIVE_POSITION_H
