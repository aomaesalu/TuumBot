
#include "__future__.hpp"

namespace rtx {

  long double gaussian_probability(double mu, double sig, double x) {
    return exp(- pow(mu - x, 2) / pow(sig, 2) / 2.0) / sqrt(2.0 * PI * pow(sig, 2));
  }

  long double random_gaussian(double mu, double sig) {
    std::random_device rd;
    std::mt19937 e2(rd());

    std::normal_distribution<> g(mu, sig);
    return g(e2);
  }

};
