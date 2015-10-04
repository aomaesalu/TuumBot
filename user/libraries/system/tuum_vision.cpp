/**
 * @file tuum_vision.cpp
 * Visioning interface implementation.
 *
 * @authors Meelik Kiik
 * @version 0.1
 */

#include <unistd.h>
#include <boost/thread.hpp>

#include "tuum_vision.hpp"

namespace rtx {

  Camera CAM(RTX_INTERFACE_CAM0);
  Camera CAM1(RTX_INTERFACE_CAM1);

  namespace Vision {

    void init() {
      CAM.init();
    }

  };

};
