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

#include "camera.h"

#include "unistd.h"             // Operating system API header
#include <linux/videodev2.h>    // V4L2 header

Camera::Camera(const std::string &device, const int &width, const int &height):
device{device},
width{width},
height{height}
{
  openDevice();
}

Camera::~Camera() {
  closeDevice();
}

void Camera::openDevice() {
  // TODO
}

void Camera::closeDevice() {
  if (close(fileDescriptor) == -1)
    throw runtime_error("Error upon closing device.");
  fileDescriptor = -1;
}

std::string Camera::getDevice() const {
  return device;
}

int Camera::getWidth() const {
  return width;
}

int Camera::getHeight() const {
  return height;
}
