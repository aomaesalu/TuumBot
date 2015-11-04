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
    uint32_t start_at;
    uint32_t period;
    uint32_t _end_at;

    void start() {
      start_at = 0; // Replace with current time
      _end_at = start_at + period;
    }

    void setPeriod(uint32_t p) {
      period = p;
    }

    bool isTime() {
      return 0 > _end_at; // Replace with current time
    }

  };
};


//TODO: update Geometry module
namespace rtx {

  template<typename T>
  struct Vec2D {
    T x; T y;

    double distanceTo(Vec2D<T> t) {
      return distanceTo(t.x, t.y);
    }

    double distanceTo(T _x, T _y) {
      return sqrt(pow(x - _x, 2) + pow(y - _y, 2));
    }

    double getMagnitude() {
      return sqrt(pow(x, 2) + pow(y, 2));
    }

    void rotate(double alpha) {
      T X = x * cos(alpha) + y * -sin(alpha);
      T Y = x * sin(alpha) + y * cos(alpha);
      x = X; y = Y;
    }

    void normalizeTo(double factor) {
      if(x == 0.0 && y == 0.0) return;
      double s = sqrt(pow(x, 2) + pow(y, 2)) / factor;
      x /= s; y /= s;
    }
  };

  template<typename T>
  struct Vec3D {
    T x; T y; T z;
  };

  template<typename A, typename B, typename C>
  struct Vec3 {
    A x; B y; C z;
  };

  template<typename A, typename B, typename C>
  struct Vec4 {
    A x; B y; C z; C a;
  };

  typedef Vec2D<int> Vec2i;
  typedef Vec3D<int> Vec3i;

  typedef Vec2D<double> Vec2f;
  typedef Vec3D<double> Vec3f;

  struct Transform {
    Vec2i p;
    double o;

    Vec2i getPosition() {
      return p;
    }

    int getX() { return p.x; }
    int getY() { return p.y; }
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
