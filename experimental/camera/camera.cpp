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

Camera::Camera(const std::string name, const int& width, const int& height):
name{name},
width{width},
height{height}
{}

std::string Camera::getName() const {
  return name;
}

int Camera::getWidth() const {
  return width;
}

int Camera::getHeight() const {
  return height;
}
