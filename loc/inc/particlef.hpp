
#ifndef RTX_LOC_PARTICLE_FILTER_H
#define RTX_LOC_PARTICLE_FILTER_H

#include <sstream>
#include <vector>
#include <string>
#include <stdint.h>

#include "loc_core.hpp"

using namespace rtx;


namespace loc {

  struct Particle {
    int x; int y; double o;
    long double prob;

    Particle(int _x, int _y, double _o) {
      x = _x; y = _y; o = _o;
      prob = 1;
    }

    std::string serialize() {
      std::stringstream ss;
      ss << "<Particle ";
      ss << x << ", " << y << ", " << o <<", " << prob << ">";
      return ss.str();
    }
  };

  typedef std::vector<Particle> ParticleSet;

  class ParticleFilter {
    private:
      ParticleSet m_particles;
      uint32_t m_size;
      long double m_mx_probability;

      double m_noise_fw;
      double m_noise_rot;
      double m_noise_sense;

    public:
      ParticleFilter();

      void init(uint32_t, int, int, int, int);

      ParticleSet* getParticles() { return &m_particles; }

      void calcStandardDeviation(double&, double&, double&);

      void update(MotionVec);
      void process(LandmarkSet);
      void filter();
      SystemState evaluate();

      double calcError(SystemState);

      void printParticleInfo();
  };
};

#endif
