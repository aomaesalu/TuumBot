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

#include "constants.h"
#include "field.h"
#include "ball.h"
#include "robotAlly.h"
#include "robotEnemy.h"

class Field2vs2: public Field {
private:
  Ball* ball;
  RobotAlly* ally;
  RobotEnemy** enemies;
public:
  Field2vs2(const Field2vs2&);
  Field2vs2(RobotSelf*, RobotAlly*, RobotEnemy**, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
  Field2vs2(Goal*, Goal*, RobotSelf*, RobotAlly*, RobotEnemy**, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
  Field2vs2(RobotSelf*, RobotAlly*, RobotEnemy**, Ball*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
  Field2vs2(Goal*, Goal*, RobotSelf*, RobotAlly*, RobotEnemy**, Ball*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
  RobotAlly* getAlly() const;
  RobotEnemy** getEnemies() const;
  Ball* getBall() const;
};

#endif // FIELD_2VS2_H
