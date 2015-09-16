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

#include "../mathematics/rectangle.h"
#include "goal.h"
#include "robotSelf.h"

class Field: public Rectangle {
private:
  int currentGoal;
  Goal** goals;
  RobotSelf* self;
public:
  Field(const Field&);
  Field(const Goal*, const Goal*, const RobotSelf*);
  void swapGoals();
  int getCurrentGoal() const;
  Goal* getFriendlyGoal() const;
  Goal* getEnemyGoal() const;
  RobotSelf* getSelf() const;
};

#endif // FIELD_H
