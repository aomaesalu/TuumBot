/** @file tuum_vision.hpp
 *  Visioning interface declaration.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include "__future__.hpp"

#include "Camera.hpp"
#include "rtxent.hpp"

using namespace vis;

#ifndef RTX_VISIONING_H
#define RTX_VISIONING_H

namespace rtx { namespace Visioning {

  typedef std::vector<Feature> FeatureSet;
  typedef std::vector<Ball> BallSet;
  typedef std::vector<Robot> RobotSet;
  typedef std::vector<Goal> GoalSet;

  extern FeatureSet features;
  extern BallSet balls;
  extern RobotSet robots;
  extern GoalSet goals;

  void setup();
  void process();

  void featureDetection(CameraDevice*);
  void ballsDetection(CameraDevice*);
  void robotsDetection(CameraDevice*);
  void goalDetection(CameraDevice*);
}}

#endif // RTX_VISIONING_H
