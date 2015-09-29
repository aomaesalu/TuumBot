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

#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include "constants.h"

class Camera {
private:
  std::string device;
  int width;
  int height;
public:
  Camera(const std::string& = CAMERA_DEVICE, const int& = CAMERA_WIDTH, const int& = CAMERA_HEIGHT);
  std::string getDevice() const;
  int getWidth() const;
  int getHeight() const;
};

#endif // CAMERA_H
