/**
 * Tuum
 * Robotex 2015
 * University of Tartu
 * Team:
 *   Kristjan Kanarbik
 *   Meelik Kiik
 *   Ants-Oskar Mäesalu
 *   Mart Müllerbeck
 *   Kaur Viiklaid
 *   Indrek Virro
 *   Mihkel Väljaots

 * Camera class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#include "camera.h"

Camera::Camera(const std::string &device, const int &width, const int &height):
device{device},
width{width},
height{height}
{}

std::string Camera::getDevice() const {
  return device;
}

int Camera::getWidth() const {
  return width;
}

int Camera::getHeight() const {
  return height;
}
