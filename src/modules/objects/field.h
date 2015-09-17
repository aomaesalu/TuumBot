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

 * Field class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef FIELD_H
#define FIELD_H

#include "constants.h"
#include "goal.h"
#include "robotSelf.h"
#include "../mathematics/rectangle.h"

class Field: public Rectangle {
private:
  int currentGoal;
  Goal** goals;
  RobotSelf* robotSelf;
public:
  Field(const Field&);
  Field(Goal*, Goal*, RobotSelf*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH); // TODO: Add default field dimensions
  void swapGoals();
  int getCurrentGoal() const;
  Goal* getFriendlyGoal() const;
  Goal* getEnemyGoal() const;
  RobotSelf* getRobotSelf() const;
};

#endif // FIELD_H
