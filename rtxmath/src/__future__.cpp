
#include "__future__.hpp"

namespace rtx {

  long double gaussian_probability(double mu, double sig, double x) {
    return exp(- pow(x - mu, 2) / ( 2 * sig ) ) / ( sqrt(sig) * sqrt(2.0 * PI) ) ;
  }

  long double gauss_prob2(double mu, double t, double x) {
    double _t = 1.0 / t;
    return _t / sqrt(2*PI) * exp( -1 * pow(_t, 2) * pow(x - mu, 2) / 2 );
  }

  long double random_gaussian(double mu, double sig) {
    std::random_device rd;
    std::mt19937 e2(rd());

    std::normal_distribution<> g(mu, sig);
    return g(e2);
  }

};
