/** @file tuum_navigation.hpp
 *  Navigation system interface.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 *  @date 2 December 2015
 */

#ifndef RTX_NAVIGATION_H
#define RTX_NAVIGATION_h

#include "rtxmath.hpp"

#include "Ball.hpp"
#include "Goal.hpp"
#include "Robot.hpp"

namespace rtx { namespace Navigation {

  void preProcess();

  int countValidBalls();

  //@{
  /** Calculates robot transform for a given ball
   *  such so that the ball could be picked up
   *  by simple forward motion.
   */
  Transform calcBallPickupPos(Transform*);

  /** Calculates robot transform for a given goal
   *  such so that a ball in the dribbler would
   *  be shot through the goal.
   */
  Vec2i calcGoalShootPos(Transform*);
  //@}

  Vec2i calcCenterFieldPos(Goal*);

  Ball* getNearestBall();

  Goal* getNearestGoal();
  Goal* getOpponentGoal();

  Robot* getAlly();


  int toShootPosition(Vec2i);
}}

#endif // RTX_NAVIGATION_H
