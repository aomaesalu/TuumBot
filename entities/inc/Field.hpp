/**
 * @file Field.hpp
 * Football 2vs2 application main module.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef FIELD_H_
#define FIELD_H_

#include "constants.h"
#include "Rectangle.hpp"

#include "Goal.hpp"
#include "Robot.hpp"


namespace rtx {

  class Field: public Rectangle {
    public:
      Field(const Field&);
      Field(Robot*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field(Goal*, Goal*, Robot*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);

      void swapGoals();
      int getCurrentGoal() const;

      Goal* getFriendlyGoal() const;
      Goal* getEnemyGoal() const;

      Robot* getRobotSelf() const;

    private:
      int currentGoal;

      Goal** goals;
      Robot* robotSelf;
  };

};

#endif // FIELD_H_
