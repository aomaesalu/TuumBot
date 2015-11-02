
#include <iostream>

#include "rtxc.h"

#include "particlef.hpp"


namespace loc {

  ParticleFilter::ParticleFilter() {

  }

  void ParticleFilter::init(uint32_t N, int mn_x, int mx_x, int mn_y, int mx_y) {
    printf("[ParticleFilter::init]Loading...\n");
    m_particles.reserve(N);

    m_noise_fw = 0.1;
    m_noise_rot = 0.1;
    m_noise_sense = 5.0;

    int x, y; double o;
    uint32_t i;
    for(i=0; i < N; i++) {
      x = (int)random_uniform(mn_x, mx_x);
      y = (int)random_uniform(mn_x, mx_x);
      o = random_uniform(0.0, 2*PI);
      m_particles.push_back(Particle(x, y, o));
    }

    m_size = m_particles.size();
    printf("[ParticleFilter::init]Spawned %lu particles.\n", m_particles.size());
  }

  void ParticleFilter::calcStandardDeviation(double& x_dev, double& y_dev, double& o_dev) {
    int m_x = 0, m_y = 0; double m_o = 0;
    int N = m_particles.size();

    for(auto & p : m_particles) {
      m_x += p.x; m_y += p.y; m_o += p.o;
    }
    m_x /= N; m_y /= N; m_o /= N;

    x_dev = 0; y_dev = 0; o_dev = 0;
    for(auto & p : m_particles) {
      x_dev += pow(p.x - m_x, 2);
      y_dev += pow(p.y - m_y, 2);
      o_dev += pow(p.o - m_o, 2);
    }
    x_dev = sqrt(x_dev / N);
    y_dev = sqrt(y_dev / N);
    o_dev = sqrt(o_dev / N);
  }

  void ParticleFilter::update(MotionVec mvec) {
    int d = mvec.distance + random_gaussian(0.0, m_noise_fw);

    //printf("d_noise: %g\n", (double)random_gaussian(0.0, m_noise_fw));
    //printf("r_noise: %g\n", (double)random_gaussian(0.0, m_noise_rot));

    for(auto & p : m_particles) {
      p.o += (mvec.orientationDelta + random_gaussian(0.0, m_noise_rot));

      if(p.o >= 2*PI) p.o -= 2*PI;
      else if(p.o < 2*PI) p.o += 2*PI;

      p.x += cos(p.o) * mvec.distance;
      p.y += sin(p.o) * mvec.distance;
    }
  }

  void ParticleFilter::process(LandmarkSet landmarks) {
    double d;
    long double g_prob = 0, P = 0;

    for(auto & p : m_particles) {
      for(auto & lm : landmarks) {
        d = lm.distanceTo(p.x, p.y);
        g_prob = gaussian_probability(d, m_noise_sense, lm.getDistance());
        p.prob *= g_prob;
      }
      P += p.prob;
    }


    m_mx_probability = 0;
    for(auto & p : m_particles) {
      // p.prob /= P;
      if(p.prob > m_mx_probability) m_mx_probability = p.prob;
    }
  }

  void ParticleFilter::filter() {
    if(m_mx_probability == 0) {
      printf("[ParticleFilter::filter]Error: maximum probability is 0!\n");
      return;
    }

    ParticleSet f_particles;
    f_particles.reserve(m_size);

    uint32_t ix = (int)random_uniform(0, m_size); // FIXME: size - 1? or size?
    long double b = 0.0;
    uint32_t i;
    Particle* pp;
    long double mx_prob = 0;
    //printParticleInfo();
    for(i=0; i < m_size; i++) {
      b += random_uniform(0.0, 1.0) * 2 * m_mx_probability;

      pp = &m_particles[ix];
      if(pp->prob <= 0) {
        ix = (ix + 1) % m_size;
        continue;
      }

      while(b > pp->prob) {
        b -= pp->prob;
        ix = (ix + 1) % m_size;
        pp = &m_particles[ix];
      }

      Particle n_p = *pp;
      n_p.prob = 1;
      f_particles.push_back(n_p);

      if(pp->prob > mx_prob) mx_prob = pp->prob;
    }

    m_particles = f_particles;
    m_mx_probability = mx_prob;
  }

  SystemState ParticleFilter::evaluate() {
    SystemState st = {0, 0, 0.0};
    int n = 0;

    for(auto & p : m_particles) {
      st.x += p.x;
      st.y += p.y;
      st.orientation += p.o;
      n++;
    }

    if(st.x > 0) st.x /= n;
    if(st.y > 0) st.y /= n;
    if(st.orientation > 0) st.orientation /= n;
    return st;
  }

  double ParticleFilter::calcError(SystemState st) {
    double S = 0.0;
    int dx, dy;

    for(auto & p : m_particles) {
      dx = p.x - st.x; dy = p.y - st.y;
      S += sqrt(pow(dx, 2) + pow(dy, 2));
    }

    return S / m_size;
  }

  void ParticleFilter::printParticleInfo() {
    uint32_t i = 0;
    for(auto & particle : m_particles) {
      printf("[ParticleFilter]%i: %s ", i, particle.serialize().c_str());
      i++;
    }
    printf("\n");
  }

};
