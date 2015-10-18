/**
 * @file Vision.hpp
 * Computer vision class using YUYV.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_VISION_VISION_H
#define RTX_VISION_VISION_H

#include <vector>
#include "Point2D.hpp"


namespace rtx {

  class Vision {

    public:
      Vision();
      ~Vision();
      std::vector<Point2D*> getBalls() const;
      std::vector<Point2D*> getGoals() const;
      std::vector<Point2D*> getCorners() const;
      std::vector<Point2D*> getRobots() const;

    private:
      std::vector<Point2D*> balls;
      std::vector<Point2D*> goals;
      std::vector<Point2D*> corners;
      std::vector<Point2D*> robots;

      void analyse();
      void lineDetection();
      void blobDetection();
      void ballDetection();
      void goalDetection();
      void cornerDetection();
      void robotDetection();

  };

};

#endif // RTX_VISION_BALL_DETECTION_H
