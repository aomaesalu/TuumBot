
#include "motion.hpp"


namespace rtx { namespace Motion {

  const ValueLevels<int> VLS_DIST = {5000, 1000, 600, 300, 200};
  const ValueLevels<double> VLS_ANGLE = {2*3.14, 0.38, 0.27, 0.20, 0.10};

  //TODO: Movement gears
  const Gears GRS_MOV = {{100, 200}, {60, 175}, {40, 100}, {35, 50}, {30, 50}};
  const Gears GRS_ROT = {{5, 0.25}, {5, 0.25}, {10, 0.15}, {15, 0.05}, {12, 0.08}};

}}
