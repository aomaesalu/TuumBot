/**
 * @file Field1vs1.hpp
 * 1vs1 football game field class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_FIELD_1VS1_H
#define RTX_ENTITIES_FIELD_1VS1_H

#include <vector>

#include "entityConstants.hpp"
#include "Field.hpp"
#include "Ball.hpp"
#include "RobotSelf.hpp"


namespace rtx {

  class Field1vs1: public Field {
    public:
      Field1vs1(const Field1vs1&);
      Field1vs1(const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field1vs1(RobotSelf*, const double& = FIELD_WIDTH,
                const double& = FIELD_LENGTH);
      Field1vs1(Goal*, Goal*, RobotSelf*, const double& = FIELD_WIDTH,
                const double& = FIELD_LENGTH);
      Field1vs1(RobotSelf*, const std::vector<Ball*>&,
                const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field1vs1(Goal*, Goal*, RobotSelf*, const std::vector<Ball*>&,
                const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);

      std::vector<Ball*> getBalls() const;

    private:
      std::vector<Ball*> balls;
  };

};

#endif // RTX_ENTITIES_FIELD_1VS1_H
