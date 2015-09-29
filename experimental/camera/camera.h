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

#include "constants.h"    // Camera constants

class Camera {

public:

  /**
    Camera constructor. Receives the camera device location and the preferred
    resolution dimensions as parameters.
    Opens the device.
    // TODO: Initialise device
    // TODO: Start capturing
  */
  Camera(const std::string& = CAMERA_DEVICE, const int& = CAMERA_WIDTH,
         const int& = CAMERA_HEIGHT);

  /**
    Camera destructor.
    Closes the device.
    // TODO: Uninitialise device
    // TODO: Stop capturing
  */
  ~Camera();

  /**
    Returns the camera device's path.
  */
  std::string getDevice() const;

  /**
    Returns the camera resolution's width.
  */
  int getWidth() const;

  /**
    Returns the camera resolution's height.
  */
  int getHeight() const;

private:

  /**
    The camera device's path. The default value for this variable is described
    in the camera constants file. The value actually used in the program can be
    set upon class initialisation.
    In Linux operating systems, this is usually /dev/videoN, where N is the
    video device's number.
  */
  std::string device;

  /**
    The camera resolution's width. The default value for this variable is
    described in the camera constants file. The value actually used in the
    program can be set upon class initialisation.
  */
  int width;

  /**
    The camera resolution's height. The default value for this variable is
    described in the camera constants file. The value actually used in the
    program can be set upon class initialisation.
  */
  int height;

  /**
    The camera device's file descriptor. This specific value is set upon opening
    the device and is used in input-output operations performed on the device,
    and upon closing the device.
  */
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
    Throws a runtime error if the device cannot be closed. This can only happen
    if the file descriptor used upon closing the device is not a valid open file
    descriptor of the device.
  */
  void closeDevice();

};

#endif // CAMERA_H
