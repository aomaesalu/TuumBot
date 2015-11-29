/**
 * @file Field2vs2.hpp
 * 2vs2 football game field class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_FIELD_2VS2_H
#define RTX_ENTITIES_FIELD_2VS2_H

#include "entityConstants.hpp"
#include "Field.hpp"
#include "Ball.hpp"
#include "RobotSelf.hpp"
#include "RobotAlly.hpp"
#include "RobotEnemy.hpp"


namespace rtx {

  class Field2vs2: public Field {
    public:
      Field2vs2(const Field2vs2&);
      Field2vs2(RobotSelf*, const double& = FIELD_WIDTH,
                const double& = FIELD_WIDTH);
      Field2vs2(RobotSelf*, RobotAlly*, RobotEnemy**,
                const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field2vs2(Goal*, Goal*, RobotSelf*, RobotAlly*, RobotEnemy**,
                const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field2vs2(RobotSelf*, RobotAlly*, RobotEnemy**, Ball*,
                const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);
      Field2vs2(Goal*, Goal*, RobotSelf*, RobotAlly*, RobotEnemy**, Ball*,
                const double& = FIELD_WIDTH, const double& = FIELD_LENGTH);

      RobotAlly* getAlly() const;
      RobotEnemy** getEnemies() const;
      Ball* getBall() const;

    private:
      Ball *ball;
      RobotAlly *ally;
      RobotEnemy **enemies;
  };

};

#endif // RTX_ENTITIES_FIELD_2VS2_H
