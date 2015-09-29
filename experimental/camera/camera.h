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

class Camera {
private:
std::string name;
  int width;
  int height;
public:
  Camera(const std::string& = "/dev/video0", const int& = 480, const int& = 640);
  std::string getName() const;
  int getWidth() const;
  int getHeight() const;
};

#endif // CAMERA_H
