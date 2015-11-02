/** @file __future__.hpp
 *  Contains funcionalities that are yet to be
 *  properly implemented in a module but are required.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 24. October 2015
 */

#include <random>

#include "rtxc.h"

#ifndef RTX_FUTURE_H
#define RTX_FUTURE_H

// TODO: these belong in loc module? or create dedicated system module?
namespace rtx {

  struct MotionVec {
    int distance;
    double orientationDelta;
  };

  long double gaussian_probability(double mu, double sig, double x);
  long double random_gaussian(double mu, double sig);


  struct Timer {
    uint32_t start;
    uint32_t period;
    uint32_t _end;

    void set() {
      _end = start + period;
    }

    bool isTime() {
      return _end > 0; // Replace '0' with some 'getTime' function
    }

  };
};


//TODO: update Geometry module
namespace rtx {

  template<typename T>
  struct Vec2D {
    T x; T y;
  };

  template<typename T>
  struct Vec3D {
    T x; T y; T z;
  };

  typedef Vec2D<int> Vec2i;
  typedef Vec3D<int> Vec3i;

  typedef Vec2D<double> Vec2f;
  typedef Vec3D<double> Vec3f;
};

namespace vis {

  class Feature {
    private:
      int m_placeholder;
    public:
      Feature() {

      }
  };

};

#endif // RTX_FUTURE_H
