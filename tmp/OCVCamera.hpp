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

#include "opencv2/highgui/highgui.hpp"

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

using namespace cv;
using namespace boost;

namespace rtx {

  class Camera : public CameraDevice {
    private:
      VideoCapture* cap;

      volatile boost::atomic<bool> VISION_RUNNING;
      static bool READY; // Initial frame aquisition fix

      boost::mutex frame_mutex;
      Mat frame;
      Mat framebuffer;

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
