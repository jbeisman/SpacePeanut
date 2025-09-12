
#pragma once

#include <array>
#include <vector>

// User-facing interface
void enforce_periodic_boundary_conditions(
    const float &GMAX, std::vector<std::array<float, 3>> &pPos);
