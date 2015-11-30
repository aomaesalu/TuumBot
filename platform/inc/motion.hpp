
#ifndef RTX_MOTION_H
#define RTX_MOTION_H

namespace rtx { namespace Motion {

  struct Gear {
    int v; double step;
  };

  struct Gears {
    Gear high;
    Gear mid;
    Gear low;
  };

  template<typename T>
  struct ValueLevels {
    T mx;
    T high;
    T mid;
    T low;
    T mn;
  };


  const ValueLevels<int> VLS_DIST = {5000, 2000, 1400, 800, 300};
  const ValueLevels<double> VLS_ANGLE = {2*3.14, 3.14, 3.14/2.0, 3.14/4.0, 3.14/8.0};

  //TODO: Movement gears
  const Gears GRS_MOV = {{15, 50}, {15, 50}, {15, 50}};
  const Gears GRS_ROT = {{20, 0.25}, {15, 0.2}, {10, 0.05}};

}}

#endif // RTX_MOTION_H
