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
#include "Feature.hpp"


namespace rtx {

  class Vision {

    public:
      Vision();
      ~Vision();

      std::vector<Feature*> getBalls() const;
      std::vector<Feature*> getGoals() const;
      std::vector<Feature*> getCorners() const;
      std::vector<Feature*> getRobots() const;

      std::vector<Feature*> getStaticFeatures() const;
      std::vector<Feature*> getMovingFeatures() const;
      std::vector<Feature*> getAllFeatures() const;

      void process();

    private:
      std::vector<Feature*> balls;
      std::vector<Feature*> goals;
      std::vector<Feature*> corners;
      std::vector<Feature*> robots;

      std::vector<Feature*> staticFeatures;
      std::vector<Feature*> movingFeatures;
      std::vector<Feature*> allFeatures;

      void lineDetection();
      void blobDetection();
      void ballDetection();
      void goalDetection();
      void cornerDetection();
      void robotDetection();

  };

};

#endif // RTX_VISION_BALL_DETECTION_H
