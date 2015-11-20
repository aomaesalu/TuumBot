/** @file Vec.hpp
 *  Mathemathical vectors.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 19. November 2015
 */

#ifndef RTX_VEC_H
#define RTX_VEC_H

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

    double getOrientation() {
      double o = atan2(y, x);
      return o;
    }

    Vec2D<T> operator*(double val) {
      return Vec2D<T>({(int)(x*val), (int)(y*val)});
    }

    Vec2D<T> operator-(double val) {
      return (*this) * (1.0 - val / this->getMagnitude());
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
}

#endif // RTX_VEC_H
