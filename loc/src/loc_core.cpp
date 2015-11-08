/** @file loc_core.cpp
 *  Localization core functions implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 24. October 2015
 */

#include "loc_core.hpp"


namespace loc {

  double random_uniform(double mn, double mx) {
    return mn + static_cast<float>(rand()) / ( static_cast<float>(RAND_MAX/(mx - mn)) );
  }

  double random_uniform(int mn, int mx) {
    return rand() % (mx - mn) + mn;
  }

  Landmark::Landmark(int x, int y):
    m_pos({x, y})
  {

  }

  double Landmark::distanceTo(int x, int y) {
    return sqrt(pow(m_pos.x - x, 2) + pow(m_pos.y - y, 2));
  }

};
