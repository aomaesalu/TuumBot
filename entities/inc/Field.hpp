/**
 * @file Field.hpp
 * Football 2vs2 application main module.
 *
 * @authors
 * @version 0.1
 */

#ifndef FIELD_H_
#define FIELD_H_

#include "rtxc.h"

#include "Rectangle.hpp"

#include "Goal.hpp"
#include "Robot.hpp"


namespace rtx {

  class Field: public Rectangle {
    private:
      int currentGoal;

      Goal** goals;
      Robot* robotSelf;

    public:
      Field(const Field&);
      Field(Robot*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field(Goal*, Goal*, Robot*, const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);

      void swapGoals();
      int getCurrentGoal() const;

      Goal* getFriendlyGoal() const;
      Goal* getEnemyGoal() const;

      Robot* getRobotSelf() const;
  };

};

#endif // FIELD_H_
