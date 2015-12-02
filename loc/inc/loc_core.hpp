/** @file loc_core.hpp
 *  Core localization functions.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 24. October 2015
 */

#include <cstdlib>
#include <vector>
#include <string>

#include "rtxmath.hpp"

#ifndef RTX_LOC_CORE_H
#define RTX_LOC_CORE_H

namespace loc {

  struct SystemState {
    int x; int y; double orientation;

    std::string serialize() {
      std::string output = "<SystemState ";
      output += std::to_string(x);
      output += ", ";
      output += std::to_string(y);
      output += ", ";
      output += std::to_string(orientation);
      output += ">";
      return output;
    }
  };

  double random_uniform(double mn, double mx);
  double random_uniform(int mn, int mx);

  class Landmark {
    private:
      rtx::Vec2i m_pos;
      double m_distanceEstimate;
    public:
      Landmark(int, int);

      int getX() { return m_pos.x; }
      int getY() { return m_pos.y; }

      double getDistance() { return m_distanceEstimate; }
      void setDistanceEstimate(double v) { m_distanceEstimate = v; }

      double distanceTo(int x, int y);
  };

  typedef std::vector<Landmark> LandmarkSet;
};

#endif // RTX_LOC_CORE_H
