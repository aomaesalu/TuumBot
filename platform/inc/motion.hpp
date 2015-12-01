
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


  extern const ValueLevels<int> VLS_DIST;
  extern const ValueLevels<double> VLS_ANGLE;

  //TODO: Movement gears
  extern const Gears GRS_MOV;
  extern const Gears GRS_ROT;

}}

#endif // RTX_MOTION_H
