/**
 * @file Camera.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef RTX_CAMERA_H
#define RTX_CAMERA_H

namespace rtx {

  class Camera;

  class CameraDevice {
    public:
      virtual void init() = 0;    // Initialize camera
      //virtual void dispose();
      virtual void process() = 0;  // Read camera data into framebuffer
      virtual bool getFrame(int&) = 0;  // Frame read function

      // Blocking functions for threading usage
      virtual void begin() = 0;
      virtual void loop() = 0;
      virtual void end() = 0;
  };

};

#endif // RTX_CAMERA_H
