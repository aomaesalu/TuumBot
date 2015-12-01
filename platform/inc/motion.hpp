
#ifndef RTX_MOTION_H
#define RTX_MOTION_H

namespace rtx { namespace Motion {

  struct Gear {
    int v; double step;
  };

  struct Gears {
    Gear mx;
    Gear high;
    Gear med;
    Gear low;
    Gear mn;
  };

  template<typename T>
  struct ValueLevels {
    T mx;
    T high;
    T med;
    T low;
    T mn;
  };


  const ValueLevels<int> VLS_DIST = {5000, 850, 600, 210, 100};
  const ValueLevels<double> VLS_ANGLE = {2*3.14, 0.38, 0.27, 0.20, 0.10};

  //TODO: Movement gears
  const Gears GRS_MOV = {{150, 275}, {80, 175}, {40, 100}, {25, 50}, {15, 50}};
  const Gears GRS_ROT = {{9, 0.3}, {9, 0.30}, {10, 0.20}, {12, 0.05}, {0, 0.0}};

}}

#endif // RTX_MOTION_H
