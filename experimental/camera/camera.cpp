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

#include "camera.h"             // The class header

#include <fcntl.h>              // File control operations (open)
#include <sys/stat.h>           // File characteristics header (stat)
#include <unistd.h>             // OS API header (close)
#include <cstring>              // C string header (memset, strerror)
#include <stdexcept>            // Exception header (runtime_error)
#include <linux/videodev2.h>    // V4L2 header

Camera::Camera(const std::string &device, const int &width, const int &height):
device{device},
width{width},
height{height}
{
  fileDescriptor = -1;
  openDevice();
}

Camera::~Camera() {
  closeDevice();
}

void Camera::openDevice() {
  // File status structure
  struct stat status;

  // Get device file status.
  // Upon successful completion, 0 shall be returned. Otherwise, -1 shall be
  // returned and errno set to indicate the error.
  if (stat(device.c_str(), &status) == -1)
    throw std::runtime_error(device + ": cannot identify! " +
                             std::to_string(errno) + ": " +
                             std::strerror(errno));

  // Test if the device file is a character special file. The test returns a
  // non-zero value if the test is true; 0 if the test is false.
  if (!S_ISCHR(status.st_mode))
    throw std::runtime_error(device + " is no device");

  // Try to open the device.
  // Upon successful completion, the open function shall open the file and
  // return a non-negative integer representing the lowest numbered unused file
  // descriptor. Otherwise, -1 shall be returned and errno set to indicate the
  // error. No files shall be created or modified if the function returns -1.
  // O_RDWR sets the file to be open for reading and writing.
  // O_NONBLOCK sets the function to return without blocking for the device to
  // be ready or available.
  if ((fileDescriptor = open(device.c_str(), O_RDWR | O_NONBLOCK, 0)) == -1)
    throw std::runtime_error(device + ": cannot open! " + std::to_string(errno)
                             + ": " + std::strerror(errno));
}

void Camera::closeDevice() {
  // Try to close the device, based on its file descriptor
  // The close function returns -1 if the file descriptor is not a valid open
  // device file descriptor of the device.
  if (close(fileDescriptor) == -1)
    throw std::runtime_error("Error upon closing device");

  // Reset the file descriptor to ensure it doesn't refer to any device anymore.
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
