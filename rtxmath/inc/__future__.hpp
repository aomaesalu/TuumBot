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
    uint32_t _start;
    uint32_t period;
    uint32_t _end;

    void start() {
      // _start = ?
      _end = _start + period;
    }

    bool isTime() {
      return _end > 0; // Replace '0' with some 'getTime' function
    }

    void setPeriod(uint32_t T) {
      period = T;
    }

  };
};


//TODO: update Geometry module
namespace rtx {

  template<typename T>
  struct Vec2D {
    T x; T y;

    void rotate(double alpha) {
      Vec2D v;
      v.x = x * cos(alpha) + y * -sin(alpha);
      v.y = x * sin(alpha) + y * cos(alpha);
      x = v.x; y = v.y;
    }

    void normalizeTo(double value) {
      double s = abs(x) + abs(y);
      if(s == 0) return;
      x /= s;
      y /= s;
    }

    double getMagnitude() {
      if(x == 0.0 && y == 0.0) return 0.0;
      return sqrt(pow(x, 2) + pow(y, 2));
    }

    double distanceTo(Vec2D<T> t) {
      Vec2D<T> vec = {t.x - x, t.y - y};
      return vec.getMagnitude();
    }
  };

  template<typename T>
  struct Vec3D {
    T x; T y; T z;
  };

  typedef Vec2D<int> Vec2i;
  typedef Vec3D<int> Vec3i;

  typedef Vec2D<double> Vec2f;
  typedef Vec3D<double> Vec3f;

  struct Transform {
    Vec2i pos;
    double o;

    Vec2i getPosition() {
      return pos;
    }

    int getX() { return pos.x; }
    int getY() { return pos.y; }
  };
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
