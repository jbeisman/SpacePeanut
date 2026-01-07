
#pragma once

#include <array>
#include <cmath>
#include <vector>

// Backend parallel interface
void enforce_periodic_boundary_conditions_par(
    const float &GMAX, std::vector<std::array<float, 3>> &pPos);

// Periodic BC logic
inline float boundary_condition(float GMAX, float pX) {
  return  (pX < 0.0)  ? fmod(GMAX + fmod(pX, GMAX), GMAX) :
          (pX > GMAX) ? fmod(GMAX + fmod(pX, GMAX), GMAX) : pX;
}
