/** @file tuum_localization.cpp
 *  Localization system module implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 */

#include <cstdlib>

#include "tuum_localization.hpp"

using namespace loc;


namespace rtx { namespace Localization {

  SystemState state_estimate;
  ParticleFilter particleFilter;

  void demo_measurements(LandmarkSet& landmarks, int x, int y) {
    double d;
    for(auto & lm : landmarks) {
      d = lm.distanceTo(x, y);
      double noise = random_gaussian(0, 0.0);
      d += noise; // Add uncertainty which the system should filter out
      lm.setDistanceEstimate(d);
      printf("Demo distance estimate to: (%i, %i) => %f, noise = %f. \n", lm.getX(), lm.getY(), d, noise);
    }
  }

  void setup() {
    srand(static_cast<unsigned>(time(0)));

    particleFilter.init(1000, 0, 100, 0, 100);

    printf("\033[1;32m");
    printf("[Localization::setup()]Ready.");
    printf("\033[0m\n");
  }

  void process() {
    // Localize through goals and features from 'Visioning' system
    /*particleFilter.update(MotionVec mvec);
    particleFilter.sense(Visioning::features);
    particleFilter.sense(Visioning::goals);
    particleFilter.filter();
    state_estimate = particleFilter.evaluate();*/
  }

  void testSequence() {
    LandmarkSet demoLandmarks;

    demoLandmarks.push_back(Landmark(20, 20));
    demoLandmarks.push_back(Landmark(80, 80));
    demoLandmarks.push_back(Landmark(20, 80));
    demoLandmarks.push_back(Landmark(80, 20));
    printf("[Localization]%lu demo landmarks created.\n", demoLandmarks.size());

    // Test sequence
    printf("Starting test sequence...\n");
    state_estimate = particleFilter.evaluate();

    int X = 20; int Y = 10; double ori = 0.0;
    demo_measurements(demoLandmarks, X, Y);

    double dvx, dvy, dvo;
    particleFilter.calcStandardDeviation(dvx, dvy, dvo);

    //particleFilter.printParticleInfo();
    printf("Simulation state: %i, %i.\n", X, Y);
    printf("Estimated state: %s. \n", state_estimate.serialize().c_str());
    printf("Standard deviation: dvx=%f, dvy=%f, dvo=%f. \n\n", dvx, dvy, dvo);

    std::vector<MotionVec> demoMotion;
    demoMotion.push_back({10, 0.0});
    demoMotion.push_back({30, 0.2});
    demoMotion.push_back({15, 0.4});
    demoMotion.push_back({15, 0.5});

    demoMotion.push_back({10, 0.6});
    demoMotion.push_back({30, 0.7});
    demoMotion.push_back({15, 0.9});
    demoMotion.push_back({15, 1.2});

    for(auto & mvec : demoMotion) {
      X += cos(mvec.orientationDelta) * mvec.distance;
      Y += sin(mvec.orientationDelta) * mvec.distance;
      ori = atan(Y / X);
      printf("Moved to %i, %i.\n", X, Y);
      demo_measurements(demoLandmarks, X, Y);

      for(int i=0; i < 10; i++) {
        particleFilter.update(mvec);
        particleFilter.process(demoLandmarks);
        particleFilter.filter();

        state_estimate = particleFilter.evaluate();

      }

      printf("Simulation state: %i, %i, %g.\n", X, Y, ori);
      printf("Estimated state: %s. \n", state_estimate.serialize().c_str());
      printf("Standard deviation: dvx=%f, dvy=%f, dvo=%f. \n", dvx, dvy, dvo);

      SystemState sim = {X, Y, ori};
      printf("\033[1;31mMean error: %g\033[0m\n\n", particleFilter.calcError(sim));
    }
    //particleFilter.printParticleInfo();
  }

  Vec2i getPosition() {
    return Vec2i({0, 0});
  }

};};
