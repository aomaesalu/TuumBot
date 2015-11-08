/**
 * @file OCVCamera.cpp
 * Generic OpenCV camera interface usage implementation.
 *
 * @authors Meelik Kiik
 * @version 0.1
 */

#ifndef RTX_OCVCAMERA_H
#define RTX_OCVCAMERA_H

#include "CameraDevice.hpp"

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

using namespace boost;

namespace rtx {

  class Camera : public CameraDevice {
    private:
      int m_placeholder;

    public:
      Camera(int device_id);

      void init();
      void process();
      bool getFrame(int&);

      void begin();
      void loop();
      void end();
  };

};

#endif
