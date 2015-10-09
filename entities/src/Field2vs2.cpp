/**
 * @file Field2vs2.cpp
 * 2vs2 football game field class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#include "Field2vs2.hpp"


namespace rtx {

  Field2vs2::Field2vs2(const Field2vs2 &other):
    Field(other),
    ball{other.getBall()},
    ally{other.getAlly()},
    enemies{other.getEnemies()}
  {

  }

  Field2vs2::Field2vs2(RobotSelf *robotSelf, const double &width,
                       const double& length):
    Field(robotSelf, width, length)
  {
    ally = new RobotAlly();
    // TODO: Initialise enemies and ball
  }

  Field2vs2::Field2vs2(RobotSelf *robotSelf, RobotAlly *ally,
                       RobotEnemy **enemies, const double &width,
                       const double &length):
    Field(robotSelf, width, length),
    ally{ally}
  {
    this->enemies = enemies; // Causes warning when in initialization list
    // TODO: Initialise ball
  }

  Field2vs2::Field2vs2(Goal *friendlyGoal, Goal *enemyGoal,
                       RobotSelf *robotSelf, RobotAlly *ally,
                       RobotEnemy **enemies, const double &width,
                       const double &length):
    Field(friendlyGoal, enemyGoal, robotSelf, width, length),
    ally{ally}
  {
    this->enemies = enemies; // Causes warning when in initialization list
    // TODO: Initialise ball
  }

  Field2vs2::Field2vs2(RobotSelf *robotSelf, RobotAlly *ally,
                       RobotEnemy **enemies, Ball *ball, const double &width,
                       const double &length):
    Field(robotSelf, width, length),
    ally{ally}
  {
    this->enemies = enemies; // Causes warning when in initialization list
    this->ball = ball; // Causes warning when in initialization list
  }

  Field2vs2::Field2vs2(Goal *friendlyGoal, Goal *enemyGoal,
                       RobotSelf *robotSelf, RobotAlly *ally,
                       RobotEnemy **enemies, Ball *ball, const double &width,
                       const double &length):
    Field(friendlyGoal, enemyGoal, robotSelf, width, length),
    ally{ally}
  {
    this->enemies = enemies; // Causes warning when in initialization list
    this->ball = ball; // Causes warning when in initialization list
  }

  RobotAlly* Field2vs2::getAlly() const {
    return ally;
  }

  RobotEnemy** Field2vs2::getEnemies() const {
    return enemies;
  }

  Ball* Field2vs2::getBall() const {
    return ball;
  }

};
