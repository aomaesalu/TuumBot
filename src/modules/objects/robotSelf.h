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

 * Robot self class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef ROBOT_SELF_H
#define ROBOT_SELF_H

#include "robot.h"

class RobotSelf: public Robot {
private:
  // TODO: Add motors, coilgun, tribbler, cameras, ...
public:
  RobotSelf(const RobotSelf&);
  RobotSelf(const Point2D*, const double&); // TODO: Add default robot radius; add motors, coilgun, tribbler, cameras parameters
  RobotSelf(const double&, const double&, const double&); // TODO: Add default robot radius; add motors, coilgun, tribbler, cameras parameters
  // TODO: Add communication and strategy methods
  // TODO: Add movement methods
  // TODO: Add computer vision methods
}

#endif // ROBOT_SELF_H
