
#include <cmath>
#include "lambdaCDM.hh"

namespace CDM {
constexpr float H0 = 70.0f;
constexpr float omega_lambda = 0.6842f;
constexpr float omega_m = 0.3158f;
constexpr float omega_k = 1.0f - omega_m - omega_lambda;
}

float compute_H(const float a) {
  return CDM::H0 * sqrtf(CDM::omega_lambda + CDM::omega_m / (a * a * a) + CDM::omega_k / (a * a));
}

float G() {
  return 3.0f * CDM::H0 * CDM::H0 * CDM::omega_m / 2.0f;
}

