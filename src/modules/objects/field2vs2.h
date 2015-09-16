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

 * 2vs2 field class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef FIELD_2VS2_H
#define FIELD_2VS2_H

#include "field.h"
#include "ball.h"

class Field2vs2: public Field {
private:
  Ball* ball;
  RobotFriend* friend;
  RobotEnemy** enemies;
public:
  Field2vs2(const Field2vs2&);
  Field2vs2(const Goal*, const Goal*, const RobotSelf*, const RobotFriend*, const RobotEnemy**);
  Field2vs2(const Goal*, const Goal*, const RobotSelf*, const RobotFriend*, const RobotEnemy**, const Ball*);
  RobotFriend* getFriend() const;
  RobotEnemy** getEnemies() const;
  Ball* getBall() const;
}

#endif // FIELD_2VS2_H
