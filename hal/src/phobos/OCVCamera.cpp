/**
 * @file OCVCamera.cpp
 * Generic OpenCV camera interface usage implementation.
 *
 * @authors Meelik Kiik
 * @version 0.1
 */

#include <iostream>

#include "OCVCamera.hpp"


namespace rtx {

  Camera::Camera(int device_id) {
    VISION_RUNNING = false;
    READY = false;
  }

  void Camera::init() {
    printf("CAM_INIT\n");
  }

  void Camera::process() {
    if (!cap->read(framebuffer)) {
      printf("[Vision]Read failed.\n");
      return;
    }

    if(frame_mutex.try_lock()) {
      frame = framebuffer;
      if(!READY) READY = true;
      frame_mutex.unlock();
    }
  }

  bool Camera::getFrame(int& out) {
    //TODO: mutex lock aquisition
    if(!READY)
      while(!READY) {};

    frame_mutex.lock();
    out = 123;
    frame_mutex.unlock();
    return true;
  }

  void Camera::begin() {
    printf("[Vision]Starting capture.\n");
    //cap = new VideoCapture(device_id);

    if ( !cap->isOpened() ) {
     printf("[Vision]Cannot open the video file\n");
     return;
    }

    VISION_RUNNING = true;
    loop();
  }

  void Camera::loop() {
    while(VISION_RUNNING) {
      process();
    }
  }

  void Camera::end() {
    VISION_RUNNING = false;
  }

};
