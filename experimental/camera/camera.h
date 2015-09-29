/**
  Tuum
  Robotex 2015
  University of Tartu
  Team:
    Kristjan Kanarbik
    Meelik Kiik
    Ants-Oskar Mäesalu
    Mart Müllerbeck
    Kaur Viiklaid
    Indrek Virro
    Mihkel Väljaots

  Camera class
    Class for communication with the robot's cameras, based on the V4L2 API.
    The video capture example provided with the V4L2 API has been used as a
    model for the class.

  Contributors:
    Ants-Oskar Mäesalu

  Copyright (c) 2015 Ants-Oskar Mäesalu
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "constants.h"

class Camera {

private:
  std::string device;
  int width;
  int height;

  int fileDescriptor;

  /**
    // TODO
  */
  void openDevice();

  /**
    Closes the camera device. All I/O progress is terminated and resources
    associated with the file descriptor are freed. However, data format
    parameters, current input or output, control values or other properties
    remain unchanged.
  */
  void closeDevice();

public:

  /**
    Camera constructor. Receives the camera device location and the preferred
    resolution dimensions as parameters.
    Opens the device.
  */
  Camera(const std::string& = CAMERA_DEVICE, const int& = CAMERA_WIDTH,
         const int& = CAMERA_HEIGHT);

  /**
    Camera destructor.
    Closes the device.
  */
  ~Camera();

  /**
    Returns the camera's device path.
  */
  std::string getDevice() const;

  /**
    Returns the camera's resolution width.
  */
  int getWidth() const;

  /**
    Returns the camera's resolution height.
  */
  int getHeight() const;

};

#endif // CAMERA_H
