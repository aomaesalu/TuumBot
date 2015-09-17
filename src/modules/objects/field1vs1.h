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

 * 1vs1 field class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

#ifndef FIELD_1VS1_H
#define FIELD_1VS1_H

#include "constants.h"
#include "field.h"
#include "ball.h"
#include <vector>

class Field1vs1: public Field {
private:
  std::vector<Ball*> balls;
public:
  Field1vs1(const Field1vs1&);
  Field1vs1(Goal*, Goal*, RobotSelf*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
  Field1vs1(Goal*, Goal*, RobotSelf*, const std::vector<Ball*>&, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
  std::vector<Ball*> getBalls() const;
};

#endif // FIELD_1VS1_H
